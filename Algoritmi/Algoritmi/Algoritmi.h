
// Algoritmi.h : main header file for the Algoritmi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAlgoritmiApp:
// See Algoritmi.cpp for the implementation of this class
//

class CAlgoritmiApp : public CWinApp
{
public:
	CAlgoritmiApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAlgoritmiApp theApp;
