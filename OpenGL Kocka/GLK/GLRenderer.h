#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void RotateView(double dXY, double dXZ);
	void CalculatePosition();
	UINT LoadTexture(CString path);
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawCone(CDC* pDC, float r, float h, int nSeg);
	void DrawSphere(CDC* pDC, float r, int nSegAlpha, int nSegBeta);
	void DrawCube(CDC* pDC, float a, float h);
	void DrawGrid(CDC* pDC);
	void DrawXYZ(CDC* pDC);
	void DestroyScene(CDC* pDC);
	// dealocira resurse alocirane u drugim funkcijama ove klase,
	UINT textura;

	int ugao;

	int ugaoX, ugaoZ, ugaoBezK;

	double viewAngleXY;
	double viewAngleXZ;
	double viewR;

	double eyex, eyey, eyez;
	double centerx, centery, centerz;
	double upx, upy, upz;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 



};
