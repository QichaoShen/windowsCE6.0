// Send2ReceiveDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

// CSend2ReceiveDlg �Ի���
class CSend2ReceiveDlg : public CDialog
{
// ����
public:
	CSend2ReceiveDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEND2RECEIVE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	
	//���̺߳���
    static DWORD WINAPI ReadThreadFunc(LPVOID lparam);
	//���̻߳ص�����
	typedef void (CALLBACK* ONREAD)(CWnd* pWnd,char* buf,int buflen);
	static void CALLBACK OnRead(CWnd* pWnd,char *buf,int buflen);
	//�رն��߳�
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




