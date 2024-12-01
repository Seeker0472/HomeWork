// image6.h : main header file for the IMAGE6 application
//

#if !defined(AFX_IMAGE6_H__DE743926_CB3A_407B_AB54_F5E9F5FD7E42__INCLUDED_)
#define AFX_IMAGE6_H__DE743926_CB3A_407B_AB54_F5E9F5FD7E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImage6App:
// See image6.cpp for the implementation of this class
//

class CImage6App : public CWinApp
{
public:
	CImage6App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImage6App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE6_H__DE743926_CB3A_407B_AB54_F5E9F5FD7E42__INCLUDED_)
