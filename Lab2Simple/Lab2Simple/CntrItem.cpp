
// CntrItem.cpp : ���������� ������ CLab2SimpleCntrItem
//

#include "stdafx.h"
#include "Lab2Simple.h"

#include "Lab2SimpleDoc.h"
#include "Lab2SimpleView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� CLab2SimpleCntrItem

IMPLEMENT_SERIAL(CLab2SimpleCntrItem, CRichEditCntrItem, 0)

CLab2SimpleCntrItem::CLab2SimpleCntrItem(REOBJECT* preo, CLab2SimpleDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: �������� ��� ��� ������������ ������ ������������
}

CLab2SimpleCntrItem::~CLab2SimpleCntrItem()
{
	// TODO: �������� ��� �������
}


// ����������� CLab2SimpleCntrItem

#ifdef _DEBUG
void CLab2SimpleCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CLab2SimpleCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

