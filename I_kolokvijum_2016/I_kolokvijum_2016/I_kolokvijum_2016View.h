
// I_kolokvijum_2016View.h : interface of the CIkolokvijum2016View class
//

#pragma once
#include "DImage.h"


class CIkolokvijum2016View : public CView
{
protected: // create from serialization only
	CIkolokvijum2016View() noexcept;
	DECLARE_DYNCREATE(CIkolokvijum2016View)

// Attributes
public:
	CIkolokvijum2016Doc* GetDocument() const;
	DImage drvoImg, zelenoImg;
	int stapAng;
	int range;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);
	void DrawBall(CDC* pDC);
	// overridden to draw this view
	void DrawStick(CDC* pDC, int w);
	void Translate(CDC* pDC, int dx, int dy, bool leftMul);
	void Rotate(CDC* pDC, float ang, bool leftMul);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIkolokvijum2016View();
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

#ifndef _DEBUG  // debug version in I_kolokvijum_2016View.cpp
inline CIkolokvijum2016Doc* CIkolokvijum2016View::GetDocument() const
   { return reinterpret_cast<CIkolokvijum2016Doc*>(m_pDocument); }
#endif

