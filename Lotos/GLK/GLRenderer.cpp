#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define RAD(a) (a*3.141592/180) 

CGLRenderer::CGLRenderer(void)
{

	this->eyex = 0.0;
	this->eyey = 0.0;
	this->eyez = 0.0;
	this->viewAlpha = 0;
	this->viewBetha = 0;
	this->viewR = 10;
	this->upy = 1.0;
	this->factor = 0.0;
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
	this->teksture = LoadTexture("Env.jpg");
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
	gluLookAt(this->eyex, this->eyey, this->eyez, 0.0, 0.0, 0.0, 0.0, this->upy, 0.0);

	DrawAxes();

	DrawSphere(10.0, 20, 20);
	
	glColor3f(0.6, 0.2, 0.6);
	DrawSphFlower(2.0, 10, 10, factor);

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawSphFlower(float R, int n, int m, float factor)
{
	float pi = 3.141592;
	float alfa, beta;
	float dAlfa, dBeta;

	alfa = -pi / 2;
	dBeta = 2 * pi / m;
	dAlfa = 2 * pi / n;

	float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	double x;
	double y;
	double z;
	for (int i = 0; i < n; i++) {
		

		beta = 0.0;
		for (int j = 0; j < m ; j++) {

			x4 = cos(alfa) * sin(beta) * R;
			y4 = sin(alfa) * R;
			z4 = cos(alfa) * cos(beta) * R;

			x1 = cos(alfa + dAlfa) * sin(beta) * R;
			y1 = sin(alfa + dAlfa) * R;
			z1 = cos(alfa + dAlfa) * cos(beta) * R;

			x2 = cos(alfa + dAlfa) * sin(beta + dBeta) * R;
			y2 = sin(alfa + dAlfa) * R;
			z2 = cos(alfa + dAlfa) * cos(beta + dBeta) * R;

			x3 = cos(alfa) * sin(beta + dBeta) * R;
			y3 = sin(alfa) * R;
			z3 = cos(alfa) * cos(beta + dBeta) * R;

			CalcRotAxis(x3, y3, z3, x4, y4, z4, x, y, z);

			glPushMatrix();
				glTranslatef(x4, y4, z4);
				glRotated(-factor, x, y, z);
				
				glBegin(GL_QUADS);
				glVertex3f(x4, y4, z4);
				glVertex3f(x1, y1, z1);
				glVertex3f(x2, y2, z2);
				glVertex3f(x3, y3, z3);
				glEnd();
			glPopMatrix();

			beta += dBeta;
		}

		
		alfa += dAlfa;
	}
}

void CGLRenderer::CalcRotAxis(double x1, double y1, double z1, double x2,
	double y2, double z2, double& x3, double& y3, double& z3)
{
	double mag = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
	x3 = (x2 - x1) / mag;
	y3 = (y2 - y1) / mag;
	z3 = (z2 - z1) / mag;
}

void CGLRenderer::PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z)
{
	x = cos(phi) * sin(theta) * R;
	y = sin(phi) * R;
	z = cos(phi) * cos(theta) * R;
}

void CGLRenderer::DrawSphere(float R, int n, int m)
{
	float pi = 3.141592;
	float alfa, beta;
	float dAlfa, dBeta;

	alfa = -pi / 2;
	dBeta = 2 * pi / m;
	dAlfa = 2 * pi / n;

	float x, y, z, x1,y1,z1, teX,teY, teX1, teY1;
	
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, this->teksture);
	for (int i = 0; i < n+1; i++) {
		glBegin(GL_QUAD_STRIP);

		beta = 0.0;
		for (int j = 0; j < m+1; j++) {
			
			x = cos(alfa) * sin(beta) * R;
			y = sin(alfa) * R;
			z = cos(alfa) * cos(beta) * R;
			
			x1 = cos(alfa + dAlfa) * sin(beta) * R;
			y1 = sin(alfa + dAlfa) * R;
			z1 = cos(alfa + dAlfa) * cos(beta) * R;
			
			teX = 0.5 + 0.5*x/R;
			teY = 0.5 + 0.5 * y/R;
			teX1 = 0.5 + 0.5 * x1/R;
			teY1 = 0.5 + 0.5 * y1/R;

			glTexCoord2f(teX, teY);
			glVertex3f(x, y, z);
			glTexCoord2f(teX1, teY1);
			glVertex3f(x1, y1, z1);
			beta += dBeta;
		}
		
		glEnd();
		alfa += dAlfa;
	}
}

UINT CGLRenderer::LoadTexture(char* path)
{
	UINT texID;
	DImage img;
	img.Load(CString(path));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::CalculateCameraPos()
{
	this->eyex = cos(RAD(this->viewAlpha)) * sin(RAD(this->viewBetha)) * this->viewR;
	this->eyey = sin(RAD(this->viewAlpha)) * this->viewR;
	this->eyez = cos(RAD(this->viewAlpha)) * cos(RAD(this->viewBetha)) * this->viewR;
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
	glViewport(0, 0, (GLint)w, (GLint)h);
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
