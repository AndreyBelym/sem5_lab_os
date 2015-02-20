
// Lab2Dlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Lab2.h"
#include "Lab2Dlg.h"
#include "afxdialogex.h"
#include "Threads2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� ���� CLab2Dlg



CLab2Dlg::CLab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLab2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLab2Dlg::~CLab2Dlg(){
	KillThread(productors,0);
}

void CLab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SalonList);
}

BEGIN_MESSAGE_MAP(CLab2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// ����������� ��������� CLab2Dlg

BOOL CLab2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������
	m_SalonList.InsertColumn(0,_T("����������"));
	m_SalonList.InsertColumn(1,_T("Time"));
	
	m_SalonList.InsertItem(0,_T("Hello"));

	THREADDESC params={NULL,20};
	// TODO: �������� �������������� �������������
	SpawnThread(productors,&ProductorFunc,params);
	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CLab2Dlg::OnPaint()
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
HCURSOR CLab2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

