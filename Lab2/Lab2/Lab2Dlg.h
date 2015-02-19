
// Lab2Dlg.h : файл заголовка
//

#pragma once
#include "afxcmn.h"


// диалоговое окно CLab2Dlg
class CLab2Dlg : public CDialogEx
{
// Создание
public:
	CLab2Dlg(CWnd* pParent = NULL);	// стандартный конструктор
	~CLab2Dlg();
// Данные диалогового окна
	enum { IDD = IDD_LAB2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_SalonList;
};
