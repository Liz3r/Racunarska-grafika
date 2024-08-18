
// Auto.h : main header file for the Auto application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAutoApp:
// See Auto.cpp for the implementation of this class
//

class CAutoApp : public CWinApp
{
public:
	CAutoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAutoApp theApp;
