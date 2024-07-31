
// Kolokvijum1_2019View.cpp : implementation of the CKolokvijum12019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolokvijum1_2019.h"
#endif

#include "Kolokvijum1_2019Doc.h"
#include "Kolokvijum1_2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "DImage.h"

#define ARM1 "arm1.png"
#define RAD(a) (a*3.1415/180)

// CKolokvijum12019View

IMPLEMENT_DYNCREATE(CKolokvijum12019View, CView)

BEGIN_MESSAGE_MAP(CKolokvijum12019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKolokvijum12019View construction/destruction

CKolokvijum12019View::CKolokvijum12019View() noexcept
{

	arm1.Load(CString(".\\res\\arm1.png"));
	arm2.Load(CString(".\\res\\arm2.png"));
	body1.Load(CString(".\\res\\body1.png"));
	leg1.Load(CString(".\\res\\leg1.png"));
	leg2.Load(CString(".\\res\\leg2.png"));
	pozadina.Load(CString(".\\res\\background.jpg"));

	arm1Ang = 0;
	arm2Ang = 0;
	body1Ang = 0;
	leg1Ang = 0;
	leg2Ang = 0;

}

CKolokvijum12019View::~CKolokvijum12019View()
{
}

BOOL CKolokvijum12019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolokvijum12019View drawing

void CKolokvijum12019View::OnDraw(CDC* pDC)
{
	CKolokvijum12019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(clientRect);

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DrawBackground(pDC, clientRect);


	Translate(pDC, 350, 350, false);
	DrawBody1(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	pDC->SetWorldTransform(&oldForm);
	Translate(pDC, 138, 356, false);
	DrawLeg1(pDC);
	DrawLeg2(pDC);
}

void CKolokvijum12019View::DrawArm1(CDC* pDC) {

	
	Translate(pDC, 34, 31, false);
	Rotate(pDC, arm1Ang, false);
	Translate(pDC, -34, -31, false);

	DrawImgTransparent(pDC, &arm1);
	Translate(pDC, 210-23, 102-61, false);
}
void CKolokvijum12019View::DrawArm2(CDC* pDC) {

	Translate(pDC, 23, 61, false);
	Rotate(pDC, arm2Ang, false);
	Translate(pDC, -23, -61, false);
	DrawImgTransparent(pDC, &arm2);
}

void CKolokvijum12019View::DrawBody1(CDC* pDC) {

	Translate(pDC, 26, 133, false);
	Rotate(pDC, body1Ang, false);
	Translate(pDC, -26, -133, false);

	DrawImgTransparent(pDC, &body1);
	Translate(pDC, 212 - 34, 86 - 31, false);
}

void CKolokvijum12019View::DrawLeg1(CDC* pDC) {


	Translate(pDC, 237, 125, false);
	Rotate(pDC, leg1Ang, false);
	Translate(pDC, -237, -125, false);

	DrawImgTransparent(pDC, &leg1);
	Translate(pDC, 30 - 35, 125 - 60, false);
}

void CKolokvijum12019View::DrawLeg2(CDC* pDC) {
	Translate(pDC, 35, 60, false);
	Rotate(pDC, leg2Ang, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, &leg2);
}

void CKolokvijum12019View::DrawBackground(CDC* pDC, CRect rc) {

	pozadina.Draw(pDC, CRect(0, 0, pozadina.Width(), pozadina.Height()), rc);
}

void CKolokvijum12019View::Translate(CDC *pDC, float dX, float dY, bool rightMul) {

	XFORM f{
		1,0,
		0,1,
		dX,dY
	};
	
	pDC->ModifyWorldTransform(&f, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolokvijum12019View::Rotate(CDC* pDC, float angle, bool rightMul) {

	XFORM f{
		cos(RAD(angle)),sin(RAD(angle)),
		-sin(RAD(angle)),cos(RAD(angle))
	};

	pDC->ModifyWorldTransform(&f, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolokvijum12019View::Scale(CDC* pDC, float sX, float sY, bool rightMul) {

	XFORM f{
		sX,0,
		0,sY
	};

	pDC->ModifyWorldTransform(&f, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolokvijum12019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect c;
	GetClientRect(&c);

	int w = pImage->Width();
	int h = pImage->Height();

	CDC srcDC, dstDC;
	srcDC.CreateCompatibleDC(pDC);
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* bmpImage = pImage->GetBitmap();
	CBitmap bmpMask;

	bmpMask.CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC.SelectObject(bmpImage);
	CBitmap* oldDest = dstDC.SelectObject(&bmpMask);

	srcDC.SetBkColor(srcDC.GetPixel(0, 0));
	dstDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &dstDC, 0, 0, SRCAND);

	srcDC.SelectObject(oldSrc);
	dstDC.SelectObject(oldDest);

	srcDC.DeleteDC();
	dstDC.DeleteDC();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	memDC.SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCAND);

	memDC.SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCPAINT);
}


void CKolokvijum12019View::DrawTransparent(CDC* pDC) {
	
	
}


// CKolokvijum12019View printing

BOOL CKolokvijum12019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolokvijum12019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolokvijum12019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolokvijum12019View diagnostics

#ifdef _DEBUG
void CKolokvijum12019View::AssertValid() const
{
	CView::AssertValid();
}

void CKolokvijum12019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolokvijum12019Doc* CKolokvijum12019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolokvijum12019Doc)));
	return (CKolokvijum12019Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolokvijum12019View message handlers


void CKolokvijum12019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	switch (nChar) {
		case 'Q':
			arm1Ang -= 10;
			break;
		case 'A':
			arm1Ang += 10;
			break;
		case 'T':
			arm2Ang -= 10;
			break;
		case 'G':
			arm2Ang += 10;
			break;
		case 'W':
			body1Ang -= 10;
			break;
		case 'S':
			body1Ang += 10;
			break;
		case 'E':
			leg1Ang -= 10;
			break;
		case 'D':
			leg1Ang += 10;
			break;
		case 'R':
			leg2Ang -= 10;
			break;
		case 'F':
			leg2Ang += 10;
			break;
		default:
			break;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
