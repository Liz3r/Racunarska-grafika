
// I_kolokvijum_2016View.cpp : implementation of the CIkolokvijum2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I_kolokvijum_2016.h"
#endif

#include "I_kolokvijum_2016Doc.h"
#include "I_kolokvijum_2016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIkolokvijum2016View

IMPLEMENT_DYNCREATE(CIkolokvijum2016View, CView)

BEGIN_MESSAGE_MAP(CIkolokvijum2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIkolokvijum2016View construction/destruction

CIkolokvijum2016View::CIkolokvijum2016View() noexcept
{
	

	drvoImg.Load(CString("wood.jpg"));
	zelenoImg.Load(CString("felt2.jpg"));

	stapAng = 0;

	//koristim za translaciju lopte i stapa u kombinaciji sa uglom stapAng
	range = 0;
}

CIkolokvijum2016View::~CIkolokvijum2016View()
{
}

BOOL CIkolokvijum2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIkolokvijum2016View drawing

void CIkolokvijum2016View::OnDraw(CDC* pDC)
{
	CIkolokvijum2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	XFORM fOld;
	pDC->GetWorldTransform(&fOld);

	CRect c;
	GetClientRect(&c);

	CBitmap* drvoCBmp = drvoImg.getBitmap();
	CBitmap* zelenoCBmp = zelenoImg.getBitmap();
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBM;
	memBM.CreateCompatibleBitmap(pDC, c.Width(), c.Height());

	CRgn clipRgnArea, clipRgnExclude, finalRgn;
	clipRgnArea.CreateRectRgn(0, 0, c.Width(), c.Height());
	clipRgnExclude.CreateRectRgn(50, 50, c.Width() -50, c.Height() - 50);
	clipRgnArea.CombineRgn(&clipRgnArea, &clipRgnExclude, RGN_DIFF);

	memDC.SelectObject(zelenoCBmp);
	for (int i = 0; i < c.Width() / zelenoImg.Width()+1; i++) {
		for (int j = 0; j < c.Height() / zelenoImg.Height()+1; j++) {
		pDC->BitBlt(i*zelenoImg.Width(), j * zelenoImg.Height(), c.Width(), c.Height(), &memDC, 0.0, 0.0, SRCCOPY);
		}
	}

	pDC->SelectClipRgn(&clipRgnArea);
	memDC.SelectObject(drvoCBmp);
	pDC->StretchBlt(0, 0, c.Width(), c.Height(), &memDC, 0,0,drvoImg.Width(), drvoImg.Height(), SRCCOPY);
	pDC->SelectClipRgn(NULL);

	CPen *bPen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush *bBrush = new CBrush(RGB(0, 0, 0));

	CPen* oldPen = pDC->SelectObject(bPen);
	CBrush* oldBrush = pDC->SelectObject(bBrush);

	int r = 30;
	
	pDC->Ellipse(50 - r, 50 - r, 50 + r, 50 + r);
	pDC->Ellipse(c.Width()- 50 - r, c.Height() - 50 - r, c.Width()-50 + r, c.Height()-50 + r);
	pDC->Ellipse(c.Width() - 50 - r, 50 - r, c.Width() - 50 + r, 50 + r);
	pDC->Ellipse(50 - r, c.Height() - 50 - r, 50 + r, c.Height() - 50 + r);
	pDC->Ellipse(c.Width()/2 - r, 50 - r, c.Width() / 2 + r, 50 + r);
	pDC->Ellipse(c.Width() / 2 - r, c.Height() - 50 - r, c.Width() / 2 + r, c.Height() - 50 + r);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	pDC->SetGraphicsMode(GM_ADVANCED);

	int stapW = 500;

	Translate(pDC, 500, 500, true);
	Rotate(pDC, stapAng, true);
	if (range >= 0) {
		DrawBall(pDC);
		Translate(pDC, -stapW / 2 - stapW / 100 - 20 - range, 0, true);
		DrawStick(pDC, stapW);
	}
	else {
		Translate(pDC, -stapW / 2 - stapW / 100 - 20, 0, true);
		DrawStick(pDC, stapW);
		Translate(pDC, stapW / 2 + stapW / 100 + 20 + abs(range),0, true);
		DrawBall(pDC);
	}
	
}

void CIkolokvijum2016View::DrawBall(CDC* pDC)
{
	CPen* bPen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* bBrush = new CBrush(RGB(96, 0, 0));
	CPen* wPen = new CPen(PS_SOLID, 1, RGB(250, 250, 250));
	CBrush* wBrush = new CBrush(RGB(250, 250,250));

	CPen* oldPen = pDC->SelectObject(bPen);
	CBrush* oldBrush = pDC->SelectObject(bBrush);

	pDC->Ellipse(-20, -20, 20, 20);
	pDC->SelectObject(wPen);
	pDC->SelectObject(wBrush);
	pDC->Ellipse(0, -10, 10, 0);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	delete bPen;
	delete bBrush;
}

void CIkolokvijum2016View::DrawStick(CDC* pDC, int w)
{
	CPen* bPen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* bBrush = new CBrush(RGB(250, 192, 128));
	CBrush* darkBBrush = new CBrush(RGB(64, 42, 0));

	CPen* oldPen = pDC->SelectObject(bPen);
	CBrush* oldBrush = pDC->SelectObject(bBrush);

	int r = w / 100;

	pDC->MoveTo(-w / 2, -w / 50);
	pDC->BeginPath();
	{
		pDC->LineTo(w / 2, -w / 100);
		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(w/2-r, -w/100, w/2+r, w/100, w/2, -w/100, w/2, w/100);
		pDC->LineTo(-w / 2, w / 50);
		pDC->LineTo(-w / 2, -w / 50);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(darkBBrush);
	pDC->MoveTo(-w / 2, -w / 50);
	pDC->BeginPath();
	{
		pDC->LineTo(-w / 2 + w/2*2/3 , -w / 50 + w/100*1/3);
		pDC->LineTo(-w / 2 + w / 2 * 2 / 3, w / 50 - w / 100 * 1 / 3);
		pDC->LineTo(-w / 2, w / 50);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	delete bPen;
	delete bBrush;
	delete darkBBrush;
}

void CIkolokvijum2016View::Translate(CDC* pDC, int dx, int dy, bool leftMul)
{
	XFORM f;
	f.eM11 = 1;
	f.eM12 = 0;
	f.eM21 = 0;
	f.eM22 = 1;
	f.eDx = dx;
	f.eDy = dy;

	pDC->ModifyWorldTransform(&f, leftMul ? MWT_LEFTMULTIPLY : MWT_RIGHTMULTIPLY);

}
void CIkolokvijum2016View::Rotate(CDC* pDC, float ang, bool leftMul)
{
	float a = ang*3.141592 / 180;
	XFORM f{
		cos(a),sin(a),
		-sin(a),cos(a)
	};

	pDC->ModifyWorldTransform(&f, leftMul ? MWT_LEFTMULTIPLY : MWT_RIGHTMULTIPLY);

}

// CIkolokvijum2016View printing

BOOL CIkolokvijum2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIkolokvijum2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIkolokvijum2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIkolokvijum2016View diagnostics

#ifdef _DEBUG
void CIkolokvijum2016View::AssertValid() const
{
	CView::AssertValid();
}

void CIkolokvijum2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIkolokvijum2016Doc* CIkolokvijum2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIkolokvijum2016Doc)));
	return (CIkolokvijum2016Doc*)m_pDocument;
}
#endif //_DEBUG


// CIkolokvijum2016View message handlers


void CIkolokvijum2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) {
		stapAng += 5;
	}
	if (nChar == VK_RIGHT) {
		stapAng -= 5;
	}
	if (nChar == VK_UP) {
		range += 15;
	}
	if (nChar == VK_DOWN) {
		range -= 15;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
