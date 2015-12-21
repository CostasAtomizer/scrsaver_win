// mc.h : main header file for the mc application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CmcApp:
// See mc.cpp for the implementation of this class
//

class CmcApp : public CWinApp
{
public:
	CmcApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CmcApp theApp;