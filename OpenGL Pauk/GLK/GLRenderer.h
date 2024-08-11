#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	void CalculateCameraPos();
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawBackgroundCube(float a);
	UINT LoadTexture(char* path);
	void DrawAxes();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawLeg(int alfa);

	int angAlfa, angBeta;
	int eyex, eyey, eyez;
	double alfa, beta;

	UINT backTex, botTex, frontTex, leftTex, rightTex, topTex, spiderTex;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
