// Send2Receive.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CSend2ReceiveApp:
// 有关此类的实现，请参阅 Send2Receive.cpp
//

class CSend2ReceiveApp : public CWinApp
{
public:
	CSend2ReceiveApp();
	
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSend2ReceiveApp theApp;
