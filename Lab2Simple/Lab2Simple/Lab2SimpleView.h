
// Lab2SimpleView.h : интерфейс класса CLab2SimpleView
//

#pragma once

class CLab2SimpleCntrItem;

class CLab2SimpleView : public CRichEditView
{
protected: // создать только из сериализации
	CLab2SimpleView();
	DECLARE_DYNCREATE(CLab2SimpleView)

// Атрибуты
public:
	CLab2SimpleDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // вызывается в первый раз после конструктора

// Реализация
public:
	virtual ~CLab2SimpleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // отладочная версия в Lab2SimpleView.cpp
inline CLab2SimpleDoc* CLab2SimpleView::GetDocument() const
   { return reinterpret_cast<CLab2SimpleDoc*>(m_pDocument); }
#endif

