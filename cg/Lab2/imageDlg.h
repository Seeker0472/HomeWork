// imageDlg.h : header file
//
# include "resource.h"
#if !defined(AFX_IMAGEDLG_H__89EC3A79_5D54_4765_B9CE_D799BA723245__INCLUDED_)
#define AFX_IMAGEDLG_H__89EC3A79_5D54_4765_B9CE_D799BA723245__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImageDlg dialog

class CImageDlg : public CDialog
{
// Construction
public:
	CImageDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImageDlg)
	enum { IDD = IDD_IMAGE_DIALOG };
	float	m_R;
	float	m_G;
	float	m_B;
	float	m_m;
	float	m_a;
	float	m_b;
	float	m_c;
	float	m_d;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit2();
	afx_msg void OnButton3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnChangeEdit8();
	afx_msg void OnChangeEdit7();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEDLG_H__89EC3A79_5D54_4765_B9CE_D799BA723245__INCLUDED_)
