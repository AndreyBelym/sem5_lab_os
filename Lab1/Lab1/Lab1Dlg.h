
// Lab1Dlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
#include "Threads.h"

// диалоговое окно CLab1Dlg
class CLab1Dlg : public CDialogEx
{
// Создание
public:
	CLab1Dlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_LAB1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	virtual void CLab1Dlg::OnOK();

// Реализация
protected:
	HICON m_hIcon;
	THREADLIST mythreads;
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnLbnSelchangeList1();
	CListBox m_lstThreads;
	afx_msg void OnBnClickedButton3();
	CString m_strLog;
	CEdit m_ctlLog;
	DWORD m_SleepTime;
	afx_msg void OnBnClickedButton1();
};
