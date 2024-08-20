
// AutoView.cpp : implementation of the CAutoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Auto.h"
#endif

#include "AutoDoc.h"
#include "AutoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RAD(a) (a*3.141592/180)
// CAutoView

IMPLEMENT_DYNCREATE(CAutoView, CView)

BEGIN_MESSAGE_MAP(CAutoView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CAutoView construction/destruction

CAutoView::CAutoView() noexcept
{
	
	wheel.Load(CString("Wheel.png"));
	clio = GetEnhMetaFile(L"clio.emf");
	wheelAng = 0;
	autoX = 1;
	poluprecnikTocka = 80;
	groundAngle = 0;
}

CAutoView::~CAutoView()
{
}

BOOL CAutoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAutoView drawing

void CAutoView::OnDraw(CDC* pDC)
{
	CAutoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//pDC->SetGraphicsMode(GM_ADVANCED);
	
	GetClientRect(&c);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, c.Width(), c.Height());

	memDC.SelectObject(&bm);
	int oldMode = memDC.SetGraphicsMode(GM_ADVANCED);
	
	XFORM oldF;
	memDC.GetWorldTransform(&oldF);
	{
		CBrush bb(RGB(255, 255, 255));
		memDC.FillRect(CRect(0, 0, c.Width(), c.Height()), &bb);

		DrawGround(&memDC, groundAngle);

		Translate(&memDC, autoX * cos(-RAD(groundAngle)), c.Height() - 147 + autoX * sin(-RAD(groundAngle)), false);
		Rotate(&memDC, -groundAngle, false);
		DrawCar(&memDC, 0, 0, 350, 125);
		DrawWheel(&memDC, 13, 80, poluprecnikTocka, wheelAng);
		DrawWheel(&memDC, 240, 80, poluprecnikTocka, wheelAng);
	}
	memDC.SetWorldTransform(&oldF);
	memDC.SetGraphicsMode(oldMode);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);
}

void CAutoView::DrawWheel(CDC* pDC, int x, int y, int r, float angle) {

	XFORM old;
	pDC->GetWorldTransform(&old);

	float sW = (float)r / (float)wheel.Width();
	float sH = (float)r / (float)wheel.Height();
	
	Translate(pDC, x+240*sW, y+204*sW, false);
	Rotate(pDC, angle, false);
	Translate(pDC, -240*sW, -204*sW, false);
	Scale(pDC, sW, sH, false);
	DrawImgTransparent(pDC, &this->wheel);
	pDC->SetWorldTransform(&old);
}

void CAutoView::DrawImgTransparent(CDC* pDC, DImage *pImg) {

	CDC srcDC, dstDC;
	srcDC.CreateCompatibleDC(pDC);
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* img = pImg->getCBitmap();
	CBitmap mask;
	mask.CreateBitmap(pImg->Width(), pImg->Height(), 1, 1, NULL);

	CBitmap* oldSrc = srcDC.SelectObject(img);
	CBitmap* oldDst = dstDC.SelectObject(&mask);

	srcDC.SetBkColor(srcDC.GetPixel(0, 0));
	dstDC.BitBlt(0, 0, pImg->Width(), pImg->Height(), &srcDC, 0, 0, SRCCOPY);

	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.BitBlt(0, 0, pImg->Width(), pImg->Height(), &dstDC, 0, 0, SRCAND);

	srcDC.SelectObject(oldSrc);
	dstDC.SelectObject(oldDst);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&mask);
	pDC->BitBlt(0, 0, pImg->Width(), pImg->Height(), &memDC, 0, 0, SRCAND);
	memDC.SelectObject(img);
	pDC->BitBlt(0, 0, pImg->Width(), pImg->Height(), &memDC, 0, 0, SRCPAINT);

}

void CAutoView::DrawCar(CDC* pDC, int x, int y, int w, int h) {

	ENHMETAHEADER hd;
	GetEnhMetaFileHeader(this->clio, sizeof(ENHMETAHEADER), &hd);

	XFORM old;
	pDC->GetWorldTransform(&old);
	Translate(pDC, x + w, y, false);
	Scale(pDC, -1, 1, false);
	pDC->PlayMetaFile(this->clio, CRect(hd.rclBounds.left, hd.rclBounds.top, w, h));
	pDC->SetWorldTransform(&old);
}

void CAutoView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply) {
	XFORM f = {
		sX,0,
		0,sY
	};
	pDC->ModifyWorldTransform(&f, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CAutoView::Rotate(CDC* pDC, float ang, bool rightMultiply) {

	XFORM f = {
		cos(RAD(ang)),sin(RAD(ang)),
		-sin(RAD(ang)),cos(RAD(ang))
	};
	pDC->ModifyWorldTransform(&f, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CAutoView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply) {

	XFORM f = {
		1,0,
		0,1,
		dX,dY
	};
	pDC->ModifyWorldTransform(&f, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CAutoView::DrawGround(CDC* pDC, float angle) {

	CPen* p = new CPen(PS_SOLID, 5, RGB(115, 43, 55));
	CBrush* b = new CBrush(HS_DIAGCROSS, RGB(244,250,188));

	CPen* oldPen = pDC->SelectObject(p);
	CBrush* oldBrush = pDC->SelectObject(b);

	double x = c.Width() / cos(RAD(angle));
	
	CPoint pts[3] = { CPoint(0,c.Height()), CPoint(c.Width(), c.Height()-(int)(sin(RAD(angle))*x)), CPoint(c.Width(), c.Height()) };
	pDC->Polygon(pts, 3);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	delete p;
	delete b;
}


// CAutoView printing

BOOL CAutoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAutoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAutoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CAutoView diagnostics

#ifdef _DEBUG
void CAutoView::AssertValid() const
{
	CView::AssertValid();
}

void CAutoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAutoDoc* CAutoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAutoDoc)));
	return (CAutoDoc*)m_pDocument;
}
#endif //_DEBUG


// CAutoView message handlers


void CAutoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (nChar == VK_RIGHT) {
		this->autoX += 10;
		this->wheelAng += 10;// / this->poluprecnikTocka;
	}
	if (nChar == VK_LEFT) {
		this->autoX -= 10;
		this->wheelAng -= 10;// / this->poluprecnikTocka;
	}
	if (nChar == VK_UP) {
		this->autoX = 0;
		this->wheelAng = 0;
		this->groundAngle += 5;
	}
	if (nChar == VK_DOWN) {
		this->autoX = 0;
		this->wheelAng = 0;
		this->groundAngle -= 5;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CAutoView::OnEraseBkgnd(CDC* pDC)
{
	return true;

	return CView::OnEraseBkgnd(pDC);
}
