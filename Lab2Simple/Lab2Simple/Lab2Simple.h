
// Lab2Simple.h : ������� ���� ��������� ��� ���������� Lab2Simple
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// CLab2SimpleApp:
// � ���������� ������� ������ ��. Lab2Simple.cpp
//

class CLab2SimpleApp : public CWinApp
{
public:
	CLab2SimpleApp();


// ���������������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����������
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab2SimpleApp theApp;
