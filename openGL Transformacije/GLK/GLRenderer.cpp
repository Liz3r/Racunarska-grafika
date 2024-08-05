#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	ugao = 45;
	m_hrc = NULL;
	this->viewR = 10;

	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;

	this->CalculatePosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	this->viewAngleXY += dXY;
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	this->viewAngleXZ += dXZ;
	this->CalculatePosition();
}

void CGLRenderer::CalculatePosition()
{
	double dWXY = this->viewAngleXY * 3.14 / 180,
		dWXZ = this->viewAngleXZ * 3.14 / 180;

	this->eyex = this->viewR * cos(dWXY) * cos(dWXZ);
	this->eyey = this->viewR * sin(dWXY);
	this->eyez = this->viewR * cos(dWXY) * sin(dWXZ);

	this->upy = signbit(cos(dWXY)) ? -1 : 1;
}


void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//gluLookAt(6.0, 5.2, -0.3, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	

	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	DrawGrid(pDC);
	DrawXYZ(pDC);

	glColor3f(0.50,0.75,1.0);
	glTranslatef(0.0, 0.4, 0.0);
	DrawCube(pDC, 0.4, 0.8);
	glTranslatef(0.0, 0.4, 0.0);
	glColor3f(0.00, 0.80, 0.00);
	DrawSphere(pDC, 0.16, 18, 18);
	glPushMatrix();
	{
		glColor3f(0.50, 0.75, 1.0);
		glRotatef(-45, 1.0, 0.0, 0.0);
		glTranslatef(0.0, 0.4+0.16, 0.0);
		DrawCube(pDC, 0.4, 0.8);
		glTranslatef(0.0, 0.4, 0.0);
		glColor3f(0.00, 0.80, 0.00);
		DrawSphere(pDC, 0.16, 18, 18);

		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		glColor3f(0.50, 0.75, 1.0);
		DrawCone(pDC, 0.2, 0.8, 16);

		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		glColor3f(0.00, 0.80, 0.00);
		DrawSphere(pDC, 0.16, 18, 18);

	}
	glPopMatrix();
	glPushMatrix();
	{
		glColor3f(1.0, 1.0,	0.0);
		glRotatef(ugao, 1.0, 0.0, 0.0);
		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		DrawCube(pDC, 0.4, 0.8);
		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		glColor3f(0.00, 0.80, 0.00);
		DrawSphere(pDC, 0.16, 18, 18);
		glPushMatrix();
		{
			glRotatef(60, 1.0, 0.0, 0.0);
			glColor3f(0.50, 0.75, 1.0);
			glTranslatef(0.0, 0.4 + 0.16, 0.0);
			DrawCube(pDC, 0.4, 0.8);

			glTranslatef(0.0, 0.4, 0.0);
			glColor3f(0.00, 0.80, 0.00);
			DrawSphere(pDC, 0.16, 18, 18);

			glTranslatef(0.0, 0.4 + 0.16, 0.0);
			glColor3f(0.50, 0.75, 1.0);
			DrawCone(pDC, 0.2, 0.8, 16);

			glTranslatef(0.0, 0.4 + 0.16, 0.0);
			glColor3f(0.00, 0.80, 0.00);
			DrawSphere(pDC, 0.16, 18, 18);

		}
		glPopMatrix();
		glPushMatrix();
		{
			glRotatef(-40, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.4 + 0.16, 0.0);
			glColor3f(0.50, 0.75, 1.0);
			DrawCone(pDC, 0.2, 0.8, 16);
			glTranslatef(0.0, 0.4 + 0.16, 0.0);
			glColor3f(0.00, 0.80, 0.00);
			DrawSphere(pDC, 0.16, 18, 18);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glColor3f(0.50, 0.75, 1.0);
	glTranslatef(0.0, 0.4 + 0.16, 0.0);
	DrawCube(pDC, 0.4, 0.8);

	glTranslatef(0.0, 0.4, 0.0);
	glColor3f(0.00, 0.80, 0.00);
	DrawSphere(pDC, 0.16, 18, 18);

	glPushMatrix();
	{
		glRotatef(-40, 1.0, 0.0, 0.0);
		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		glColor3f(0.50, 0.75, 1.0);
		DrawCone(pDC, 0.2, 0.8, 16);
		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		glColor3f(0.00, 0.80, 0.00);
		DrawSphere(pDC, 0.16, 18, 18);
		glColor3f(0.50, 0.75, 1.0);
		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		DrawCube(pDC, 0.4, 0.8);
		glTranslatef(0.0, 0.4, 0.0);
		glColor3f(0.00, 0.80, 0.00);
		DrawSphere(pDC, 0.16, 18, 18);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(30, 1.0, 0.0, 0.0);
		glColor3f(0.50, 0.75, 1.0);
		glTranslatef(0.0, 0.4 + 0.16, 0.0);
		DrawCube(pDC, 0.4, 0.8);
		glTranslatef(0.0, 0.4, 0.0);
		glColor3f(0.00, 0.80, 0.00);
		DrawSphere(pDC, 0.16, 18, 18);
	}
	glPopMatrix();
	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawCone(CDC* pDC, float r, float h, int nSeg) {

	float segSize = 2 * 3.141592 / nSeg;

	float ang = 0;


	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i <= nSeg; i++) {
			glVertex3f(r*sin(ang), -h / 2, r*cos(ang));
			glVertex3f(0.0 , h / 2, 0.0);
			ang += segSize;
			glVertex3f(r*sin(ang), -h / 2, r*cos(ang));
		}
	}
	glEnd();
}

void CGLRenderer::DrawSphere(CDC* pDC, float r, int nSegAlpha, int nSegBeta) {

	double ang1, ang2;
	double dAng1, dAng2;

	dAng1 = 3.141592 / (double)nSegAlpha;
	dAng2 = 2 * 3.141592 / (double)nSegBeta;
	ang1 = -3.141592 / 2;

	for (int i = 0; i < nSegAlpha; i++) {

		ang2 = 0;
		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j < nSegBeta + 1; j++) {
			double x1 = r * cos(ang1) * cos(ang2);
			double y1 = r * sin(ang1);
			double z1 = r * cos(ang1) * sin(ang2);

			double x2 = r * cos(ang1 + dAng1) * cos(ang2);
			double y2 = r * sin(ang1 + dAng1);
			double z2 = r * cos(ang1 + dAng1) * sin(ang2);

			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);

			ang2 += dAng2;
		}
		glEnd();
		ang1 += dAng1;
	}
}

void CGLRenderer::DrawCube(CDC* pDC, float a, float h) {

	glBegin(GL_QUADS);


	glVertex3f(-a / 2, -h / 2, a / 2);
	glVertex3f(a / 2, -h / 2, a / 2);
	glVertex3f(a / 2, h / 2, a / 2);
	glVertex3f(-a / 2, h / 2, a / 2);

	glVertex3f(-a / 2, -h / 2, a / 2);
	glVertex3f(a / 2, -h / 2, -a / 2);
	glVertex3f(a / 2, h / 2, -a / 2);
	glVertex3f(a / 2, h / 2, a / 2);

	glVertex3f(-a / 2, h / 2, -a / 2);
	glVertex3f(a / 2, h / 2, -a / 2);
	glVertex3f(a / 2, -h / 2, -a / 2);
	glVertex3f(-a / 2, -h / 2, -a / 2);

	glVertex3f(-a / 2, -h / 2, a / 2);
	glVertex3f(a / 2, -h / 2, a / 2);
	glVertex3f(-a / 2, h / 2, -a / 2);
	glVertex3f(-a / 2, -h / 2, -a / 2);

	glVertex3f(-a / 2, h / 2, a / 2);
	glVertex3f(a / 2, h / 2, a / 2);
	glVertex3f(a / 2, h / 2, -a / 2);
	glVertex3f(-a / 2, h / 2, -a / 2);

	glVertex3f(-a / 2, -h / 2, a / 2);
	glVertex3f(a / 2, -h / 2, a / 2);
	glVertex3f(a / 2, -h / 2, -a / 2);
	glVertex3f(-a / 2, h / 2, -a / 2);
	glEnd();

}

void CGLRenderer::DrawGrid(CDC* pDC) {

	int i;
	float size = 0.5;
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	for (i = -5; i <= 5; i++) {
		glVertex3f(i * size, 0.0, size*5);
		glVertex3f(i * size, 0.0, -size*5);

		glVertex3f(size*5, 0.0, i * size);
		glVertex3f(-size*5, 0.0, i * size);
	}

	glEnd();
}

void CGLRenderer::DrawXYZ(CDC* pDC) {

	glBegin(GL_LINES);
	//X
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	//Y
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	//Z
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double) h, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
