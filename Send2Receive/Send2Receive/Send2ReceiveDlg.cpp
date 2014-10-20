// Send2ReceiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Send2Receive.h"
#include "Send2ReceiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSend2ReceiveDlg 对话框

CSend2ReceiveDlg::CSend2ReceiveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSend2ReceiveDlg::IDD, pParent)
	, m_Send(_T(""))
	, m_Receive(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSend2ReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEND_EDIT, m_Send);
	DDX_Text(pDX, IDC_RECEIVE_EDIT, m_Receive);
	DDX_Control(pDX, IDC_PORT, m_Port);
	DDX_Control(pDX, IDC_DATA, m_Data);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_PARITY, m_Parity);
	DDX_Control(pDX, IDC_BAUD, m_Baud);
}

BEGIN_MESSAGE_MAP(CSend2ReceiveDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, &CSend2ReceiveDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SEND, &CSend2ReceiveDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_CLEAR_S, &CSend2ReceiveDlg::OnBnClickedClearS)
	ON_BN_CLICKED(IDC_CLEAR_R, &CSend2ReceiveDlg::OnBnClickedClearR)
	ON_CBN_DROPDOWN(IDC_PORT, &CSend2ReceiveDlg::OnCbnDropdownPort)
	ON_CBN_DROPDOWN(IDC_BAUD, &CSend2ReceiveDlg::OnCbnDropdownBaud)
	ON_CBN_DROPDOWN(IDC_DATA, &CSend2ReceiveDlg::OnCbnDropdownData)
	ON_CBN_DROPDOWN(IDC_STOP, &CSend2ReceiveDlg::OnCbnDropdownStop)
	ON_CBN_DROPDOWN(IDC_PARITY, &CSend2ReceiveDlg::OnCbnDropdownParity)
	ON_BN_CLICKED(IDC_CLOSE, &CSend2ReceiveDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CSend2ReceiveDlg 消息处理程序

BOOL CSend2ReceiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_hCom=INVALID_HANDLE_VALUE;
	//设置各个下拉框默认值
	m_Port.SetCurSel(2);
	m_Baud.SetCurSel(1);
	m_Data.SetCurSel(1);
	m_Stop.SetCurSel(0);
	m_Parity.SetCurSel(0);

	m_Send=_T("");
	m_Receive=_T("");

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CSend2ReceiveDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_SEND2RECEIVE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_SEND2RECEIVE_DIALOG));
	}
}
#endif

//***********************************************
//打开串口
BOOL CSend2ReceiveDlg::OpenPort(LPCTSTR port,UINT baud,UINT databits,UINT stopbits,UINT parity)
{
    DCB dcb;
    if(m_hCom!=INVALID_HANDLE_VALUE)
	{
	    MessageBox(_T("无法打开串口"));
		return FALSE;
	}
    
	//ASSERT(pPortOwner!=NULL);

    m_hCom=CreateFile(port,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);

	if(m_hCom==INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("打开串口失败"));
		return FALSE;
	}
  
    if(!GetCommState(m_hCom,&dcb))//读取串口的DCB
    {return FALSE;}

	dcb.BaudRate=baud;
    dcb.fBinary=TRUE;//二进制模式
	dcb.ByteSize=databits;
	dcb.Parity=parity;
	dcb.StopBits=stopbits;
	dcb.fParity=parity;
	//?
	dcb.fOutxCtsFlow=FALSE;
	dcb.fOutxDsrFlow=FALSE;
	dcb.fDtrControl=0;
	dcb.fRtsControl=0;
	dcb.fOutX=0;
	dcb.fInX=0;
	dcb.fTXContinueOnXoff=0;

        if(!SetCommState(m_hCom,&dcb))
	{
		MessageBox(_T("无法按当前参数配置端口，请检查参数"));
		return FALSE;
	}

    //设置串口读写时间
    COMMTIMEOUTS CommTimeOuts;
    GetCommTimeouts(m_hCom,&CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout=10;//读字符最大时间间隔
    CommTimeOuts.ReadTotalTimeoutMultiplier=0;//读一字节的时间上限
	CommTimeOuts.ReadTotalTimeoutConstant=100;//读数据总超时常量
	CommTimeOuts.WriteTotalTimeoutMultiplier=0;//写一字节的时间上限
	CommTimeOuts.WriteTotalTimeoutConstant=0;//写数据总超时常量

	if(!GetCommTimeouts(m_hCom,&CommTimeOuts))
	{
		TRACE(_T("设置超时参数错误"));
		//ClosePort();
		return FALSE;
	}


        //设置串口检测事件集
        SetCommMask(m_hCom,EV_RXCHAR);//接收一个字符事件
        
        //分配设备缓冲区
        SetupComm(m_hCom,1024,1024);

        //初始化发送与接收缓冲区
	    PurgeComm(m_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);

        //创建读退出事件
        m_hReadCloseEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
        
        //创建读串口线程
        m_hReadThread=CreateThread(NULL,0,ReadThreadFunc,this,0,&m_ReadThreadID);

		if(m_hReadThread==NULL||m_hReadThread==NULL)
		{
			MessageBox(_T("创建接收线程失败"));
			return FALSE;
		};
			
       
        return TRUE;
}

void CSend2ReceiveDlg::OnBnClickedOpen()
{
	
	const CString PortTable[5]={_T("COM1:"),_T("COM2:"),_T("COM3:"),_T("COM4:"),_T("COM5:")};
	const DWORD BaudTable[5]={4800,9600,19200,38400,115200};
	const DWORD DataTable[3]={7,8,9};
	const BYTE  StopTable[3]={ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS};
	const BYTE  ParityTable[3]={NOPARITY,ODDPARITY,EVENPARITY};

	
	CString port=PortTable[m_Port.GetCurSel()];
	DWORD  baudrate= BaudTable[m_Baud.GetCurSel()];
    DWORD  data=DataTable[m_Data.GetCurSel()];
    BYTE   stop=StopTable[m_Stop.GetCurSel()];
	BYTE  parity=ParityTable[m_Parity.GetCurSel()];

	/*调试用/
	DWORD  baudrate= 115200;
    BYTE  parity=ODDPARITY;
    DWORD  data=7;
    BYTE   stop=TWOSTOPBITS;

	const CString PortTable[6]={_T("COM0:"),_T("COM1:"),_T("COM2:"),_T("COM3:"),_T("COM4:"),_T("COM5:")};
	CString port=PortTable[3];
	/调试结束*/
	
	BOOL ret=OpenPort(port,baudrate,data,stop,parity);

	if(ret==FALSE)
	{
		return;
	}

	else
	{
		MessageBox(_T("打开")+port+_T("成功"));
	}
}

void CSend2ReceiveDlg::OnBnClickedSend()
{
	//UpdateData(TRUE);

    DWORD dwactlen;
	
	if(m_hCom==INVALID_HANDLE_VALUE)
	{
	    MessageBox(_T("串口未打开"));
		return ;
	}
//****************len无法得到值；
	UpdateData(TRUE);

    int len;
	len=m_Send.GetLength();

	char *psendbuf=new char[len];


	for(int i=0;i<len;i++)
		psendbuf[i]=(char)m_Send.GetAt(i);
	
	//while(1)
	//{
		//psendbuf[0]=0x55;
		//len=2;


	WriteFile(m_hCom,psendbuf,len,&dwactlen,NULL);
	delete[] psendbuf;
	//}
	
}

void CSend2ReceiveDlg::OnBnClickedClearS()
{
	m_Send=_T("");
	UpdateData(FALSE);
}

void CSend2ReceiveDlg::OnBnClickedClearR()
{
	m_Receive=_T("");
	UpdateData(FALSE);
}


DWORD CSend2ReceiveDlg::ReadThreadFunc(LPVOID lparam)
{
	DWORD len;
	char* receiveBuf=new char[1024];
    CSend2ReceiveDlg *pDlg=(CSend2ReceiveDlg*)lparam;

	
	while(1)
	{
		if(WaitForSingleObject(pDlg->m_hReadCloseEvent,0)==WAIT_OBJECT_0)
			break;//不断查询串口接收线程退出事件

		if(pDlg->m_hCom!=INVALID_HANDLE_VALUE)
		{
			BOOL readState=ReadFile(pDlg->m_hCom,receiveBuf,1024,&len,NULL);

			if(!readState)
			{
				//TRACE(_T("无法从串口读取数据"));
			}
			else
			{
				if(len!=0)
					OnRead(pDlg,receiveBuf,len);
			}
		}
	}
	delete[] receiveBuf;
	return 0;
}

//***************************************串口成功接收 回调函数
void CALLBACK CSend2ReceiveDlg::OnRead(CWnd* pWnd,char *buf,int buflen)
{
	CString tmp;
	CSend2ReceiveDlg* pDlg=(CSend2ReceiveDlg*)pWnd;
	CEdit *pRecvStrEdit=(CEdit*)pDlg->GetDlgItem(IDC_RECEIVE_EDIT);//编辑框id
	
    for(int i=0;i<buflen;i++,buf++)
	{
		tmp.Format(_T("%c"),*buf);//将字符转换为字符串
		pDlg->m_Receive += tmp;//编辑框类型是value
	}
   
	pRecvStrEdit->SetWindowTextW(pDlg->m_Receive);
}


void CSend2ReceiveDlg::OnCbnDropdownPort()
{
	m_Port.ResetContent();//显示
	m_Port.AddString(_T("COM1"));
	m_Port.AddString(_T("COM2"));
	m_Port.AddString(_T("COM3"));
	m_Port.AddString(_T("COM4"));
	m_Port.AddString(_T("COM5"));
}

void CSend2ReceiveDlg::OnCbnDropdownBaud()
{
	m_Baud.ResetContent();
	m_Baud.AddString(_T("4800"));
	m_Baud.AddString(_T("9600"));
	m_Baud.AddString(_T("19200"));
	m_Baud.AddString(_T("38400"));
	m_Baud.AddString(_T("115200"));
}

void CSend2ReceiveDlg::OnCbnDropdownData()
{
	m_Data.ResetContent();
	m_Data.AddString(_T("7"));
	m_Data.AddString(_T("8"));
	m_Data.AddString(_T("9"));
}

void CSend2ReceiveDlg::OnCbnDropdownStop()
{
	m_Stop.ResetContent();
	m_Stop.AddString(_T("1"));
	m_Stop.AddString(_T("1.5"));
    m_Stop.AddString(_T("2"));
}

void CSend2ReceiveDlg::OnCbnDropdownParity()
{
	m_Parity.ResetContent();
	m_Parity.AddString(_T("NONE"));
	m_Parity.AddString(_T("ODD"));
	m_Parity.AddString(_T("EVEN"));
}
//**********************************************
//关闭读线程
void  CSend2ReceiveDlg::CloseReadThread()
{
    SetEvent(m_hReadCloseEvent);
    
    SetCommMask(m_hCom,0);//设置所有事件无效
    
    PurgeComm(m_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);//清空所有接收、发送缓冲

    if(WaitForSingleObject(m_hReadThread,1000)==WAIT_TIMEOUT)
    {
	TerminateThread(m_hReadThread,0);
    }

    m_hReadThread=NULL;
}


//关闭串口
void CSend2ReceiveDlg::ClosePort()
{
	if(m_hCom==INVALID_HANDLE_VALUE)
	{
	    return;
	}
	
	SetCommMask(m_hCom,0);

	CloseReadThread();
    
	if(!CloseHandle(m_hCom))
        {
            m_hCom=INVALID_HANDLE_VALUE;
			return ;
        }
}

void CSend2ReceiveDlg::OnBnClickedClose()
{
	ClosePort();
}
