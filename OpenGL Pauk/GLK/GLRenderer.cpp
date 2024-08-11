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
	angAlfa = 50;
	angBeta = 0;
}

CGLRenderer::~CGLRenderer(void)
{
}

void CGLRenderer::CalculateCameraPos() 
{
	alfa = angAlfa * 3.141592 / 180;
	beta = angBeta * 3.141592 / 180;

	eyex = 25 * cos(alfa) * sin(beta);
	eyey = 25 * sin(alfa);
	eyez = 25 * cos(alfa) * cos(beta);
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
	 backTex = LoadTexture("res/back.jpg");
	 botTex = LoadTexture("res/bot.jpg");
	 frontTex = LoadTexture("res/front.jpg");
	 leftTex = LoadTexture("res/left.jpg");
	 rightTex = LoadTexture("res/right.jpg");
	 topTex = LoadTexture("res/top.jpg");
	 spiderTex = LoadTexture("res/spider.png");

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
	CalculateCameraPos();
	gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(0, 20, 0);
	DrawBackgroundCube(100.0);
	glTranslatef(0, -20, 0);
	DrawAxes();

	//telo
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(4.4,0.0,0.0);
	DrawSphere(3.0, 25, 0.25, 0.25, 0.24);
	glTranslatef(-4.4, 0.0, 0.0);
	DrawSphere(1.5, 25, 0.25, 0.25, 0.24);
	glTranslatef(-2.4, 0.0, 0.0);
	DrawSphere(1.0, 25, 0.75, 0.25, 0.24);
	glTranslatef(2.4, 0.0, 0.0);
	//noge
	DrawLeg(-90);
	DrawLeg(-115);
	DrawLeg(-140);
	DrawLeg(-165);

	DrawLeg(5);
	DrawLeg(30);
	DrawLeg(55);
	DrawLeg(80);
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DrawLeg(int alfa)
{
	glPushMatrix();
	{
		glRotatef(alfa, 0.0, 1.0, 0.0);
		glRotatef(-25, 1.0, 0.0, 0.0);
		glRotatef(-25, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 3.0, 0.0);
		DrawCone(0.3, 3.0, 10, 0.75, 0.75, 0.24);
		glTranslatef(0.0, 1.5, 0.0);
		glRotatef(-105, 1.0, 0.0, 1.0);
		glTranslatef(0.0, 4.15, 0.0);
		DrawCone(0.6, 8.0, 10, 0.75, 0.75, 0.24);
	}
	glPopMatrix();
}


void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double stepAng = 2 * 3.141592 / nSeg;
	double ang = 0;

	glBindTexture(GL_TEXTURE_2D, spiderTex);
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int i = 0; i < nSeg; i++) {
			glTexCoord2f(texU + texR*sin(ang), texV + texR * cos(ang));
			glVertex3f(r * sin(ang), -h / 2, r * cos(ang));
			glTexCoord2f(texU + texR * sin(ang + stepAng), texV + texR * cos(ang + stepAng));
			glVertex3f(r * sin(ang + stepAng), -h / 2, r * cos(ang + stepAng));
			glTexCoord2f(texU, texV);
			glVertex3f(0.0, h / 2, 0.0);

			ang += stepAng;
		}
	}
	glEnd();
	ang = 0;
	glBegin(GL_POLYGON);
	{
		for (int i = 0; i < nSeg; i++) {
			glTexCoord2f(texU + texR * sin(ang), texV + texR * cos(ang));
			glVertex3f(r * sin(ang), -h / 2, r * cos(ang));
			glTexCoord2f(texU + texR * sin(ang + stepAng), texV + texR * cos(ang + stepAng));
			glVertex3f(r * sin(ang + stepAng), -h / 2, r * cos(ang + stepAng));
			glTexCoord2f(texU, texV);
			glVertex3f(0.0, h / 2, 0.0);

			ang += stepAng;
		}
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	int i, j;
	double ang1, ang2;
	double dAng1, dAng2;
	dAng1 = 3.141592 / (double)nSeg;
	dAng2 = 2 * 3.141592 / (double)nSeg;
	ang1 = -3.141592 / 2;
	glBindTexture(GL_TEXTURE_2D, spiderTex);
	for (i = 0; i < nSeg; i++) {
		ang2 = 0;
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j < nSeg + 1; j++) {
			double x1 = r * cos(ang1) * cos(ang2);
			double y1 = r * sin(ang1);
			double z1 = r * cos(ang1) * sin(ang2);
			double x2 = r * cos(ang1 + dAng1) * cos(ang2);
			double y2 = r * sin(ang1 + dAng1);
			double z2 = r * cos(ang1 + dAng1) * sin(ang2);
			double tx1 = texR * x1 / r + texU;
			double ty1 = texR * z1 / r + texV;
			double tx2 = texR * x2 / r + texU;
			double ty2 = texR * z2 / r + texV;
			glTexCoord2f(tx1, ty1);
			glVertex3d(x1, y1, z1);
			glTexCoord2f(tx2, ty2);
			glVertex3d(x2, y2, z2);
			ang2 += dAng2;
		}
		glEnd();
		ang1 += dAng1;
	}
}

void CGLRenderer::DrawBackgroundCube(float a)
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, this->backTex);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);
	}
	glEnd();

	// front
	glBindTexture(GL_TEXTURE_2D, this->frontTex);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();


	// top
	glBindTexture(GL_TEXTURE_2D, this->topTex);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, this->botTex);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, this->leftTex);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, this->rightTex);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());


	return texID;
}

void CGLRenderer::DrawAxes()
{
	glBegin(GL_LINES);
	{
		//x
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(50.0, 0.0, 0.0);
		//y
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 50.0, 0.0);
		//z
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 50.0);
	}
	glEnd();
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0.0, 0.0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(1, &this->backTex);
	glDeleteTextures(1, &this->frontTex);
	glDeleteTextures(1, &this->topTex);
	glDeleteTextures(1, &this->botTex);
	glDeleteTextures(1, &this->leftTex);
	glDeleteTextures(1, &this->rightTex);
	glDeleteTextures(1, &this->spiderTex);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
