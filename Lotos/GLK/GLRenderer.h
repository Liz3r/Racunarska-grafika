#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);
	void DrawSphFlower(float R, int n, int m, float factor);
	void CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3);
	void PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z);
	// iscrtava scenu
	void DrawSphere(float R, int n, int m);
	UINT LoadTexture(char* path);
	void CalculateCameraPos();
	void DrawAxes();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	float eyex, eyey, eyez, upy;
	int viewR;
	int viewAlpha, viewBetha;
	UINT teksture;
	int factor;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
