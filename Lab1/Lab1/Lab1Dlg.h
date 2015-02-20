
// Lab1Dlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"
#include "Threads.h"

// ���������� ���� CLab1Dlg
class CLab1Dlg : public CDialogEx
{
// ��������
public:
	CLab1Dlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_LAB1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV
	virtual void CLab1Dlg::OnOK();

// ����������
protected:
	HICON m_hIcon;
	THREADLIST mythreads;
	// ��������� ������� ����� ���������
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
