
// CntrItem.h : ��������� ������ CLab2SimpleCntrItem
//

#pragma once

class CLab2SimpleDoc;
class CLab2SimpleView;

class CLab2SimpleCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CLab2SimpleCntrItem)

// ������������
public:
	CLab2SimpleCntrItem(REOBJECT* preo = NULL, CLab2SimpleDoc* pContainer = NULL);
		// ����������. pContainer ����� ����� �������� NULL ��� ���������� IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE ������� ������� � ������ ������������ �
		//  ������� �����������. ������ �������� OLE ��������� �
		//  ����������� �� ��������, �� ������� NULL

// ��������
public:
	CLab2SimpleDoc* GetDocument()
		{ return reinterpret_cast<CLab2SimpleDoc*>(CRichEditCntrItem::GetDocument()); }
	CLab2SimpleView* GetActiveView()
		{ return reinterpret_cast<CLab2SimpleView*>(CRichEditCntrItem::GetActiveView()); }

// ����������
public:
	~CLab2SimpleCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

