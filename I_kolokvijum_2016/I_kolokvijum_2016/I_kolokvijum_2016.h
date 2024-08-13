
// I_kolokvijum_2016.h : main header file for the I_kolokvijum_2016 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIkolokvijum2016App:
// See I_kolokvijum_2016.cpp for the implementation of this class
//

class CIkolokvijum2016App : public CWinApp
{
public:
	CIkolokvijum2016App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIkolokvijum2016App theApp;
