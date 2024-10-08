#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawShip();
	void DrawTriangle(float d1, float d2, float rep);
	void DrawCubeBG(int a);
	UINT LoadTexture(char* path);
	void CalculteCameraPos();
	void DrawAxes();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,


	UINT back, bot, front, left, right, top, ShipT1;
	double eyex, eyez, eyey;
	double viewAlpha, viewBetha;
	int viewR;
	float upy;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
