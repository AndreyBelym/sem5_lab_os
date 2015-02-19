
// Lab2SimpleView.cpp : реализация класса CLab2SimpleView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Lab2Simple.h"
#endif

#include "Lab2SimpleDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "Lab2SimpleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab2SimpleView

IMPLEMENT_DYNCREATE(CLab2SimpleView, CRichEditView)

BEGIN_MESSAGE_MAP(CLab2SimpleView, CRichEditView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// создание/уничтожение CLab2SimpleView

CLab2SimpleView::CLab2SimpleView()
{
	// TODO: добавьте код создания

}

CLab2SimpleView::~CLab2SimpleView()
{
}

BOOL CLab2SimpleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CLab2SimpleView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Задайте границы печати (720 твип = 1/2 дюйма)
	SetMargins(CRect(720, 720, 720, 720));
}

void CLab2SimpleView::OnDestroy()
{
	// Деактивируйте элемент при удалении; это важно
	// в случае использования представления разделителя
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}



// диагностика CLab2SimpleView

#ifdef _DEBUG
void CLab2SimpleView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CLab2SimpleView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CLab2SimpleDoc* CLab2SimpleView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab2SimpleDoc)));
	return (CLab2SimpleDoc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений CLab2SimpleView
