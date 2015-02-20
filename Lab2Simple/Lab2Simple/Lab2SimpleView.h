
// Lab2SimpleView.h : ��������� ������ CLab2SimpleView
//

#pragma once

class CLab2SimpleCntrItem;

class CLab2SimpleView : public CRichEditView
{
protected: // ������� ������ �� ������������
	CLab2SimpleView();
	DECLARE_DYNCREATE(CLab2SimpleView)

// ��������
public:
	CLab2SimpleDoc* GetDocument() const;

// ��������
public:

// ���������������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���������� � ������ ��� ����� ������������

// ����������
public:
	virtual ~CLab2SimpleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ���������� ������ � Lab2SimpleView.cpp
inline CLab2SimpleDoc* CLab2SimpleView::GetDocument() const
   { return reinterpret_cast<CLab2SimpleDoc*>(m_pDocument); }
#endif

