
// ZupcaniciView.cpp : implementation of the CZupcaniciView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Zupcanici.h"
#endif

#include "ZupcaniciDoc.h"
#include "ZupcaniciView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define RAD(a) (a*3.141592/180) 
// CZupcaniciView

IMPLEMENT_DYNCREATE(CZupcaniciView, CView)

BEGIN_MESSAGE_MAP(CZupcaniciView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CZupcaniciView construction/destruction

CZupcaniciView::CZupcaniciView() noexcept
{
	// TODO: add construction code here
	this->angle = 0;
}

CZupcaniciView::~CZupcaniciView()
{
}

BOOL CZupcaniciView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CZupcaniciView drawing

void CZupcaniciView::OnDraw(CDC* pDC)
{
	CZupcaniciDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GetClientRect(&this->c);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, c.Width(), c.Height());
	memDC.SelectObject(&bm);

	int oldMode = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM oldF;
	memDC.GetWorldTransform(&oldF);
	{
		CBrush bgb(RGB(255, 255, 255));
		memDC.FillRect(CRect(0, 0, c.Width(), c.Height()), &bgb);

		Translate(&memDC, 300, 300);
		Rotate(&memDC, this->angle);
		DrawGear(&memDC, 20, 20);
		Rotate(&memDC, -this->angle);
		Translate(&memDC, 190, 0);
		Rotate(&memDC, -(this->angle*2));
		DrawGear(&memDC, 10, 20);
		Rotate(&memDC, this->angle*2);
		Translate(&memDC, -310, -150);
		Rotate(&memDC, -(8+this->angle * 2));
		DrawGear(&memDC, 10, 20);
		Rotate(&memDC, 8+this->angle * 2);
	}
	memDC.SetWorldTransform(&oldF);
	memDC.SetGraphicsMode(oldMode);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);

}

void CZupcaniciView::DrawGear(CDC* pDC, int spokeCount, int spokeWidth) {
	CPen p(PS_DASH, 1, RGB(0, 0, 0));
	CPen plaviP(PS_SOLID, 2, RGB(0, 0, 0));
	CBrush plaviB(RGB(128, 192, 255));
	CBrush* oldB;
	CPen* oldP = pDC->SelectObject(&p);
	oldB = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	double dvapi = (2 * 3.141592);
	double r = (spokeCount * spokeWidth) / 3.141592;

	double plaviR = r - spokeWidth / 2;

	pDC->Ellipse(-r, -r, r, r);
	

	double angleStep = dvapi / spokeCount;
	double angCurr = 0;

	for (int i = 0; i < spokeCount; i++) {

		Rotate(pDC, angleStep);
		Translate(pDC, 0, r-3);
		Trapez(pDC, spokeWidth * 0.6, spokeWidth * 1.4, spokeWidth);
		Translate(pDC, 0, -(r-3));
		angCurr += angleStep;
	}

	pDC->SelectObject(&plaviP);
	pDC->SelectObject(&plaviB);
	pDC->Ellipse(-plaviR, -plaviR, plaviR, plaviR);

	pDC->Rectangle(-spokeWidth / 2, -spokeWidth / 2, spokeWidth / 2, spokeWidth / 2);

	CFont f;
	f.CreateFontW(18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
	pDC->SetBkMode(TBS_TRANSPARENTBKGND);
	pDC->SelectObject(&f);
	pDC->TextOutW(-spokeWidth / 2, spokeWidth / 2, CString("zubaca "));
	
	pDC->SelectObject(oldP);
	pDC->SelectObject(oldB);
}

void CZupcaniciView::Trapez(CDC* pDC, int a, int b, int h) {

	CPen p(PS_SOLID, 2, RGB(0, 0, 0));
	CBrush br(RGB(128,192,255));

	CPen *oldP = pDC->SelectObject(&p);
	CBrush *oldB = pDC->SelectObject(&br);

	CPoint pts[4] = {CPoint(-b/2,-h/2), CPoint(b / 2,-h / 2), CPoint(a / 2,h / 2),CPoint(-a / 2,h / 2) };

	pDC->Polygon(pts, 4);

	pDC->SelectObject(oldP);
	pDC->SelectObject(oldB);
}

void CZupcaniciView::Rotate(CDC* pDC, float angle) {

	XFORM t = {
		cos(angle),sin(angle),
		-sin(angle),cos(angle),
	};
	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CZupcaniciView::Translate(CDC* pDC, float x, float y) {

	XFORM t = {
		1,0,
		0,1,
		x,y
	};
	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

// CZupcaniciView printing

BOOL CZupcaniciView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CZupcaniciView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CZupcaniciView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CZupcaniciView diagnostics

#ifdef _DEBUG
void CZupcaniciView::AssertValid() const
{
	CView::AssertValid();
}

void CZupcaniciView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZupcaniciDoc* CZupcaniciView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZupcaniciDoc)));
	return (CZupcaniciDoc*)m_pDocument;
}
#endif //_DEBUG


// CZupcaniciView message handlers


void CZupcaniciView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	

	if (nChar = VK_UP) {
		this->angle += 5;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CZupcaniciView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
}
