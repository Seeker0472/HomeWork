// image2.h : main header file for the IMAGE2 application
//

#if !defined(AFX_IMAGE2_H__1ABC8EF1_83FA_4EB8_A996_AA17C29A7524__INCLUDED_)
#define AFX_IMAGE2_H__1ABC8EF1_83FA_4EB8_A996_AA17C29A7524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImage2App:
// See image2.cpp for the implementation of this class
//

class CImage2App : public CWinApp
{
public:
	CImage2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImage2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE2_H__1ABC8EF1_83FA_4EB8_A996_AA17C29A7524__INCLUDED_)
