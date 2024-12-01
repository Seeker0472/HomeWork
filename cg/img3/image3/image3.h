// image3.h : main header file for the IMAGE3 application
//

#if !defined(AFX_IMAGE3_H__AAC91C22_C1F8_4898_9A6A_617BFCEE8B72__INCLUDED_)
#define AFX_IMAGE3_H__AAC91C22_C1F8_4898_9A6A_617BFCEE8B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImage3App:
// See image3.cpp for the implementation of this class
//

class CImage3App : public CWinApp
{
public:
	CImage3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImage3App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE3_H__AAC91C22_C1F8_4898_9A6A_617BFCEE8B72__INCLUDED_)
