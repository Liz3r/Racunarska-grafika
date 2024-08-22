
// AlgoritmiView.h : interface of the CAlgoritmiView class
//

#pragma once


class CAlgoritmiView : public CView
{
protected: // create from serialization only
	CAlgoritmiView() noexcept;
	DECLARE_DYNCREATE(CAlgoritmiView)

// Attributes
public:
	CAlgoritmiDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void NagibniAlg(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF col);
	void Translate(CDC* pDC, int dX, int dY, bool rightMul);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CAlgoritmiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AlgoritmiView.cpp
inline CAlgoritmiDoc* CAlgoritmiView::GetDocument() const
   { return reinterpret_cast<CAlgoritmiDoc*>(m_pDocument); }
#endif

