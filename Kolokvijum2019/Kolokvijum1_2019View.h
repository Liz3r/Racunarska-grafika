
// Kolokvijum1_2019View.h : interface of the CKolokvijum12019View class
//
#include "DImage.h"
#pragma once


class CKolokvijum12019View : public CView
{
protected: // create from serialization only
	CKolokvijum12019View() noexcept;
	DECLARE_DYNCREATE(CKolokvijum12019View)

// Attributes
public:
	CKolokvijum12019Doc* GetDocument() const;
	DImage arm1, arm2, body1, leg1, leg2, pozadina;
	float arm1Ang, arm2Ang, body1Ang, leg1Ang, leg2Ang;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBackground(CDC* pDC, CRect rc);
	void Translate(CDC* pDC, float dX, float dY, bool rightMul);
	void Rotate(CDC* pDC, float angle, bool rightMul);
	void Scale(CDC* pDC, float sX, float sY, bool rightMul);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawTransparent(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKolokvijum12019View();
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

#ifndef _DEBUG  // debug version in Kolokvijum1_2019View.cpp
inline CKolokvijum12019Doc* CKolokvijum12019View::GetDocument() const
   { return reinterpret_cast<CKolokvijum12019Doc*>(m_pDocument); }
#endif

