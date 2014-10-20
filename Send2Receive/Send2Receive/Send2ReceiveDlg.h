// Send2ReceiveDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

// CSend2ReceiveDlg 对话框
class CSend2ReceiveDlg : public CDialog
{
// 构造
public:
	CSend2ReceiveDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEND2RECEIVE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	
	HANDLE m_hCom;
	HANDLE m_hReadThread;
	DWORD m_ReadThreadID;
	HANDLE m_hReadCloseEvent;
	CString m_Send;
	CString m_Receive;
	
	//读线程函数
    static DWORD WINAPI ReadThreadFunc(LPVOID lparam);
	//读线程回调函数
	typedef void (CALLBACK* ONREAD)(CWnd* pWnd,char* buf,int buflen);
	static void CALLBACK OnRead(CWnd* pWnd,char *buf,int buflen);
	//关闭读线程
	void CloseReadThread();

	BOOL OpenPort(LPCTSTR port,UINT baud,UINT databits,UINT stopbits,UINT parity);
	void ClosePort();

	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedClearS();
	afx_msg void OnBnClickedClearR();
	afx_msg void OnBnClickedClose();

	CComboBox m_Port;
	CComboBox m_Baud;
	CComboBox m_Data;
	CComboBox m_Stop;
	CComboBox m_Parity;
	afx_msg void OnCbnDropdownPort();
	afx_msg void OnCbnDropdownBaud();
	afx_msg void OnCbnDropdownData();
	afx_msg void OnCbnDropdownStop();
	afx_msg void OnCbnDropdownParity();
	
	
};




