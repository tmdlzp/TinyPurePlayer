
// TinyPurePlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTinyPurePlayerApp:
// �йش����ʵ�֣������ TinyPurePlayer.cpp
//

class CTinyPurePlayerApp : public CWinApp
{
public:
	CTinyPurePlayerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTinyPurePlayerApp theApp;