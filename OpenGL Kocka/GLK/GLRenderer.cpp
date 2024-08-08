#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	ugaoX = 0;
	ugaoZ = 0;
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
	//glEnable(GL_DEPTH_TEST);

	this->textura = LoadTexture(CString("./res/dice_texture.png"));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
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

UINT CGLRenderer::LoadTexture(CString path){

	UINT texID;
	DImage img;
	img.Load(path);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}


void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);

	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	DrawGrid(pDC);
	DrawXYZ(pDC);

	ugaoBezK = abs(ugaoX % 90);

	glColor3f(1.0, 1.0, 1.0);
	if(ugaoBezK >= 45)
		glTranslated(-0.011*ugaoX, 0.64 - 0.003*(ugaoBezK %45), 0.0);
	else
		glTranslated(-0.011 * ugaoX,0.64 - 0.003 * (45 - ugaoBezK % 45), 0.0);
	glRotatef(ugaoX, 0.0, 0.0, 1.0);
	printf("ugao: %d", ugaoX);
	/*ugaoBezK = ugaoX % 360;
	if(ugaoBezK >= 0 && ugaoBezK < 90)
		glTranslatef(0.5, 0.5, 0.0);
	if (ugaoBezK >= 90 && ugaoBezK < 180)
		glTranslatef(0.5, -0.5, 0.0);
	if (ugaoBezK >= 180 && ugaoBezK < 270)
		glTranslatef(-0.5, -0.5, 0.0);
	if (ugaoBezK >= 270 && ugaoBezK < 360)
		glTranslatef(-0.5, 0.5, 0.0);*/

	

	DrawCube(pDC, 1.0, 1.0);

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DrawCube(CDC* pDC, float a, float h) {

	glBindTexture(GL_TEXTURE_2D, this->textura);
	glBegin(GL_QUADS);
	{
		//prednja
		glTexCoord2f((float)2 / 4, (float)1 / 3);
		glVertex3f(-a / 2, -h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)1 / 3);
		glVertex3f(a / 2, -h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)2 / 3);
		glVertex3f(a / 2, h / 2, a / 2);
		glTexCoord2f((float)2 / 4, (float)2 / 3);
		glVertex3f(-a / 2, h / 2, a / 2);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//desno
		glTexCoord2f((float)3 / 4, (float)2 / 3);
		glVertex3f(a / 2, h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)1 / 3);
		glVertex3f(a / 2, -h / 2, a / 2);
		glTexCoord2f((float)1, (float)1 / 3);
		glVertex3f(a / 2, -h / 2, -a / 2);
		glTexCoord2f((float)1, (float)2 / 3);
		glVertex3f(a / 2, h / 2, -a / 2);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//desnje
		glTexCoord2f((float)0, (float)2 / 3);
		glVertex3f(a / 2, h / 2, -a / 2);
		glTexCoord2f((float)0, (float)1 / 3);
		glVertex3f(a / 2, -h / 2, -a / 2);
		glTexCoord2f((float)1/4, (float)1 / 3);
		glVertex3f(-a / 2, -h / 2, -a / 2);
		glTexCoord2f((float)1 / 4, (float)2 / 3);
		glVertex3f(-a / 2, h / 2, -a / 2);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//desnje
		glTexCoord2f((float)1 / 4, (float)2 / 3);
		glVertex3f(-a / 2, h / 2, -a / 2);
		glTexCoord2f((float)1 / 4, (float)1 / 3);
		glVertex3f(-a / 2, -h / 2, -a / 2);
		glTexCoord2f((float)2 / 4, (float)1 / 3);
		glVertex3f(-a / 2, -h / 2, a / 2);
		glTexCoord2f((float)2 / 4, (float)2 / 3);
		glVertex3f(-a / 2, h / 2, a / 2);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//gore
		glTexCoord2f((float)2 / 4, (float)2 / 3);
		glVertex3f(-a / 2, h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)2 / 3);
		glVertex3f(a / 2, h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)1);
		glVertex3f(a / 2, h / 2, -a / 2);
		glTexCoord2f((float)2 / 4, (float)1);
		glVertex3f(-a / 2, h / 2, -a / 2);
	}
	glEnd();
	glBegin(GL_QUADS);
	{
		//dole
		glTexCoord2f((float)2 / 4, (float)1 / 3);
		glVertex3f(a / 2, -h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)1 / 3);
		glVertex3f(-a / 2, -h / 2, a / 2);
		glTexCoord2f((float)3 / 4, (float)0);
		glVertex3f(-a / 2, -h / 2, -a / 2);
		glTexCoord2f((float)2 / 4, (float)0);
		glVertex3f(a / 2, -h / 2, -a / 2);
	}
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
	gluPerspective(40, (double)w / (double) h, 1.0, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(1, &this->textura);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
