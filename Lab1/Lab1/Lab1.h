
// Lab1.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CLab1App:
// � ���������� ������� ������ ��. Lab1.cpp
//

class CLab1App : public CWinApp
{
public:
	CLab1App();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CLab1App theApp;