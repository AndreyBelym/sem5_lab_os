
// Lab2Dlg.h : ���� ���������
//

#pragma once
#include "afxcmn.h"


// ���������� ���� CLab2Dlg
class CLab2Dlg : public CDialogEx
{
// ��������
public:
	CLab2Dlg(CWnd* pParent = NULL);	// ����������� �����������
	~CLab2Dlg();
// ������ ����������� ����
	enum { IDD = IDD_LAB2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_SalonList;
};
