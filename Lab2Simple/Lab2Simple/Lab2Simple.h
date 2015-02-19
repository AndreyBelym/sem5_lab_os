
// Lab2Simple.h : главный файл заголовка для приложения Lab2Simple
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CLab2SimpleApp:
// О реализации данного класса см. Lab2Simple.cpp
//

class CLab2SimpleApp : public CWinApp
{
public:
	CLab2SimpleApp();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab2SimpleApp theApp;
