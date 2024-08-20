
// Zupcanici.h : main header file for the Zupcanici application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CZupcaniciApp:
// See Zupcanici.cpp for the implementation of this class
//

class CZupcaniciApp : public CWinApp
{
public:
	CZupcaniciApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CZupcaniciApp theApp;
