
// Lab1Dlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Lab1.h"
#include "Lab1Dlg.h"
#include "afxdialogex.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���������� ���� CLab1Dlg


void CLab1Dlg::OnOK(){
}


CLab1Dlg::CLab1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLab1Dlg::IDD, pParent)
	, m_strLog(_T(""))
	, m_SleepTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstThreads);
	DDX_Text(pDX, IDC_EDIT2, m_strLog);
	DDX_Control(pDX, IDC_EDIT2, m_ctlLog);
	DDX_Text(pDX, IDC_EDIT1, m_SleepTime);
}

BEGIN_MESSAGE_MAP(CLab1Dlg, CDialogEx)
	ON_MESSAGE(WM_UPDATELOG, &CLab1Dlg::OnUpdateLog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLab1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CLab1Dlg::OnBnClickedButton2)
	
//	ON_LBN_SELCHANGE(IDC_LIST1, &CLab1Dlg::OnLbnSelchangeList1)
ON_BN_CLICKED(IDC_BUTTON3, &CLab1Dlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON1, &CLab1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// ����������� ��������� CLab1Dlg

BOOL CLab1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������
	win=GetSafeHwnd();
	sleep_t=1000;
	UpdateData();
	m_SleepTime=sleep_t;
	UpdateData(FALSE);
	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CLab1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CLab1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLab1Dlg::OnBnClickedOk()
{
	// TODO: �������� ���� ��� ����������� �����������
	CDialogEx::OnOK();
}


void CLab1Dlg::OnBnClickedButton2()
{
	// TODO: �������� ���� ��� ����������� �����������
	if(SpawnThread(mythreads)){
		HANDLE new_handle=mythreads.GetTail();
		CString ThreadName;
		ThreadName.Format(_T("����� %p"),new_handle);
		m_lstThreads.AddString(ThreadName);
	}
   // Handle message here.
}



void CLab1Dlg::OnBnClickedButton3()
{
	// TODO: �������� ���� ��� ����������� �����������
	int pos=m_lstThreads.GetCurSel();
	if (pos>=0){
		if (KillThread(mythreads,pos)){
			m_lstThreads.DeleteString(pos);
		} ;
	};
}
afx_msg LRESULT CLab1Dlg::OnUpdateLog(WPARAM wParam, LPARAM lParam){
   UNREFERENCED_PARAMETER(wParam);
   UNREFERENCED_PARAMETER(lParam);
   CString msg; msg.Format(_T("� ����� %p\r\n"),wParam);
   UpdateData();
   m_strLog+=msg;
   UpdateData(FALSE);
   m_ctlLog.LineScroll(m_ctlLog.GetLineCount()-1);
   // Handle message here.

   return 0;
}


void CLab1Dlg::OnBnClickedButton1()
{
	// TODO: �������� ���� ��� ����������� �����������
	UpdateData();
	sleep_t=m_SleepTime;
	UpdateData(FALSE);
}
