// Send2Receive.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CSend2ReceiveApp:
// �йش����ʵ�֣������ Send2Receive.cpp
//

class CSend2ReceiveApp : public CWinApp
{
public:
	CSend2ReceiveApp();
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSend2ReceiveApp theApp;
