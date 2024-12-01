// image4.h : main header file for the IMAGE4 application
//

#if !defined(AFX_IMAGE4_H__8DC5766F_4250_41F3_8C17_5CA611234A50__INCLUDED_)
#define AFX_IMAGE4_H__8DC5766F_4250_41F3_8C17_5CA611234A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImage4App:
// See image4.cpp for the implementation of this class
//

class CImage4App : public CWinApp
{
public:
	CImage4App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage4App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImage4App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE4_H__8DC5766F_4250_41F3_8C17_5CA611234A50__INCLUDED_)
