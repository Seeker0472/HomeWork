// image.h : main header file for the IMAGE application
//

#if !defined(AFX_IMAGE_H__D878B8D6_5EAA_4BE2_A1E2_DC3A8D263C42__INCLUDED_)
#define AFX_IMAGE_H__D878B8D6_5EAA_4BE2_A1E2_DC3A8D263C42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageApp:
// See image.cpp for the implementation of this class
//

class CImageApp : public CWinApp
{
public:
	CImageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImageApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE_H__D878B8D6_5EAA_4BE2_A1E2_DC3A8D263C42__INCLUDED_)
