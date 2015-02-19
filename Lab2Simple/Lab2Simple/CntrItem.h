
// CntrItem.h : интерфейс класса CLab2SimpleCntrItem
//

#pragma once

class CLab2SimpleDoc;
class CLab2SimpleView;

class CLab2SimpleCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CLab2SimpleCntrItem)

// Конструкторы
public:
	CLab2SimpleCntrItem(REOBJECT* preo = NULL, CLab2SimpleDoc* pContainer = NULL);
		// Примечание. pContainer может иметь значение NULL для разрешения IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE требует наличия у класса конструктора с
		//  нулевым аргументами. Обычно элементы OLE создаются с
		//  указателями на документ, не равными NULL

// Атрибуты
public:
	CLab2SimpleDoc* GetDocument()
		{ return reinterpret_cast<CLab2SimpleDoc*>(CRichEditCntrItem::GetDocument()); }
	CLab2SimpleView* GetActiveView()
		{ return reinterpret_cast<CLab2SimpleView*>(CRichEditCntrItem::GetActiveView()); }

// Реализация
public:
	~CLab2SimpleCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

