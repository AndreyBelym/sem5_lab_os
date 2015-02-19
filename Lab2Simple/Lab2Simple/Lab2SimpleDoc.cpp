
// Lab2SimpleDoc.cpp : реализация класса CLab2SimpleDoc
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Lab2Simple.h"
#endif

#include "Lab2SimpleDoc.h"
#include "CntrItem.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLab2SimpleDoc

IMPLEMENT_DYNCREATE(CLab2SimpleDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CLab2SimpleDoc, CRichEditDoc)
	// Включите реализацию контейнера OLE
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_POPUP, &CLab2SimpleDoc::OnUpdateObjectVerbPopup)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// создание/уничтожение CLab2SimpleDoc

CLab2SimpleDoc::CLab2SimpleDoc()
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CLab2SimpleDoc::~CLab2SimpleDoc()
{
}

BOOL CLab2SimpleDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}

CRichEditCntrItem* CLab2SimpleDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CLab2SimpleCntrItem(preo, const_cast<CLab2SimpleDoc*>(this));
}




// сериализация CLab2SimpleDoc

void CLab2SimpleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}

	// Вызов базового класса CRichEditDoc включает сериализацию
	//  объектов COleClientItem документа контейнера.
	// TODO: задайте CRichEditDoc::m_bRTF = FALSE в случае сериализации как текста
	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CLab2SimpleDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CLab2SimpleDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CLab2SimpleDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// диагностика CLab2SimpleDoc

#ifdef _DEBUG
void CLab2SimpleDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CLab2SimpleDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// команды CLab2SimpleDoc
