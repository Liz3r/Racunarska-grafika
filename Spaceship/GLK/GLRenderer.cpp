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
	eyex = 0;
	eyey = 0;
	eyez = 0;
	viewR = 10;
	viewAlpha = 0;
	viewBetha = 0;
	upy = 1.0;
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
	this->back = LoadTexture("back.jpg");
	this->bot = LoadTexture("bot.jpg");
	this->left = LoadTexture("left.jpg");
	this->right = LoadTexture("right.jpg");
	this->top = LoadTexture("top.jpg");
	this->front = LoadTexture("front.jpg");
	this->ShipT1 = LoadTexture("ShipT1.png");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	CalculteCameraPos();
	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, upy, 0.0);

	DrawCubeBG(50);
	DrawAxes();
	DrawShip();
	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawShip()
{
	glBindTexture(GL_TEXTURE_2D, ShipT1);
	double a1 = atan2(2.15, 5.8);
	float rep = 3.0;
	// Gornji levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * 3.141592*180, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// Gornji desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * 3.141592 * 180, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// Donji deo
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	// Levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * 3.141592 * 180, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// Desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * 3.141592 * 180, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	glPopMatrix();
}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	double a1 = atan2(d2, d1);
	double d3 = sqrt(d1 * d1 + d2 * d2);
	double y = d1 * cos(a1) / d3;
	double x = d1 * sin(a1) / d3;
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0, 0, 1.0);
	glTexCoord2f(0.5 * rep, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f((0.5 + x) * rep, y * rep);
	glVertex3f(d1, 0.0, 0.0);
	glTexCoord2f(0.5 * rep, 1.0 * rep);
	glVertex3f(d1, d2, 0.0);
	glEnd();
}

void CGLRenderer::DrawCubeBG(int a)
{
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex3f( - a,  - a,  + a);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( - a,  - a,  - a);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( - a,  + a,  - a);
		glTexCoord2f(0.0, 1.0);
		glVertex3f( - a,  + a,  + a);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex3f( + a,  - a,  + a);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( - a,  - a,  + a);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( - a,  + a,  + a);
		glTexCoord2f(0.0, 1.0);
		glVertex3f( + a,  + a,  + a);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex3f( + a,  - a,  - a);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( + a,  - a,  + a);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( + a,  + a,  + a);
		glTexCoord2f(0.0, 1.0);
		glVertex3f( + a,  + a,  - a);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex3f( - a,  - a,  - a);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( + a,  - a,  - a);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( + a,  + a,  - a);
		glTexCoord2f(0.0, 1.0);
		glVertex3f( - a,  + a,  - a);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bot);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex3f( - a,  - a,  + a);
		glTexCoord2f(1.0,0.0);
		glVertex3f( + a,  - a,  + a);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( + a,  - a,  - a);
		glTexCoord2f(0.0, 1.0);
		glVertex3f( - a,  - a,  - a);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex3f( - a,  + a,  - a);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( + a,  + a,  - a);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( + a,  + a,  + a);
		glTexCoord2f(0.0, 1.0);
		glVertex3f( - a,  + a,  + a);
	}
	glEnd();
}

UINT CGLRenderer::LoadTexture(char* path)
{
	UINT texID;
	DImage img;
	img.Load(CString(path));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::CalculteCameraPos()
{
	this->eyex = cos(viewAlpha) * cos(viewBetha) * viewR;
	this->eyey = sin(viewAlpha) * viewR;
	this->eyez = cos(viewAlpha) * sin(viewBetha) * viewR;

}

void CGLRenderer::DrawAxes()
{

	glBegin(GL_LINES);
	{
		//x
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);
		//y
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);
		//z
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	}
	glEnd();
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 1.0, 100);
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
