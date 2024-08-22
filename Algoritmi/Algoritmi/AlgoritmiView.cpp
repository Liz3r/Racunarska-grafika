
// AlgoritmiView.cpp : implementation of the CAlgoritmiView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Algoritmi.h"
#endif

#include "AlgoritmiDoc.h"
#include "AlgoritmiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAlgoritmiView

IMPLEMENT_DYNCREATE(CAlgoritmiView, CView)

BEGIN_MESSAGE_MAP(CAlgoritmiView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAlgoritmiView construction/destruction

CAlgoritmiView::CAlgoritmiView() noexcept
{
	// TODO: add construction code here

}

CAlgoritmiView::~CAlgoritmiView()
{
}

BOOL CAlgoritmiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAlgoritmiView drawing

void CAlgoritmiView::OnDraw(CDC* pDC)
{
	CAlgoritmiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect c;
	GetClientRect(&c);
	pDC->SetGraphicsMode(GM_ADVANCED);
	Translate(pDC,500,500, false);
	pDC->SetPixel(0,0,RGB(0,0,0));
	NagibniAlg(pDC, 0, 0, 50, -200, RGB(0, 0, 0));
}

void CAlgoritmiView::NagibniAlg(CDC *pDC, int x0, int y0, int x1, int y1, COLORREF col) {

	float dx, dy, b, y, m;
	dx = x1 - x0;
	dy = y1 - y0;
	m = dy / dx;
	b = y1 - m * x1;

	for (int x = 0; x < x1; x++) {
		y = m * x + b;
		pDC->SetPixel(x, int(y + 0.5), col);
	}
}
void CAlgoritmiView::Translate(CDC* pDC, int dX, int dY, bool rightMul) {

	XFORM t = {
		1,0,
		0,1,
		dX,dY
	};
	pDC->ModifyWorldTransform(&t, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

// CAlgoritmiView printing

BOOL CAlgoritmiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAlgoritmiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAlgoritmiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CAlgoritmiView diagnostics

#ifdef _DEBUG
void CAlgoritmiView::AssertValid() const
{
	CView::AssertValid();
}

void CAlgoritmiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAlgoritmiDoc* CAlgoritmiView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAlgoritmiDoc)));
	return (CAlgoritmiDoc*)m_pDocument;
}
#endif //_DEBUG


// CAlgoritmiView message handlers
