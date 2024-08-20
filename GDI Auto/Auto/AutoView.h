
// AutoView.h : interface of the CAutoView class
//

#pragma once
#include "DImage.h"


class CAutoView : public CView
{
protected: // create from serialization only
	CAutoView() noexcept;
	DECLARE_DYNCREATE(CAutoView)

// Attributes
public:
	CAutoDoc* GetDocument() const;
	CRect c;

	DImage wheel;
	HENHMETAFILE clio;
	float wheelAng;
	int autoX, poluprecnikTocka;
	float groundAngle;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);
	void DrawImgTransparent(CDC* pDC, DImage* pImg);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	// overridden to draw this view
	void Rotate(CDC* pDC, float ang, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void DrawGround(CDC* pDC, float angle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CAutoView();
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
};

#ifndef _DEBUG  // debug version in AutoView.cpp
inline CAutoDoc* CAutoView::GetDocument() const
   { return reinterpret_cast<CAutoDoc*>(m_pDocument); }
#endif

