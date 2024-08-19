
// ZupcaniciView.h : interface of the CZupcaniciView class
//

#pragma once


class CZupcaniciView : public CView
{
protected: // create from serialization only
	CZupcaniciView() noexcept;
	DECLARE_DYNCREATE(CZupcaniciView)

// Attributes
public:
	CZupcaniciDoc* GetDocument() const;
	CRect c;
	float angle;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawGear(CDC* pDC, int spokeCount, int spokeWidth);
	void Trapez(CDC* pDC, int a, int b, int h);
	void Rotate(CDC* pDC, float angle);
	void Translate(CDC* pDC, float x, float y);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CZupcaniciView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in ZupcaniciView.cpp
inline CZupcaniciDoc* CZupcaniciView::GetDocument() const
   { return reinterpret_cast<CZupcaniciDoc*>(m_pDocument); }
#endif

