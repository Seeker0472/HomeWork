// image6Dlg.h : header file
//

#if !defined(AFX_IMAGE6DLG_H__66D6DBF0_14C1_4B6E_8256_C4460D4C0CE0__INCLUDED_)
#define AFX_IMAGE6DLG_H__66D6DBF0_14C1_4B6E_8256_C4460D4C0CE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImage6Dlg dialog

class CImage6Dlg : public CDialog
{
// Construction
public:
	CImage6Dlg(CWnd* pParent = NULL);	// standard constructor
int m_s[7][7];
// Dialog Data
	//{{AFX_DATA(CImage6Dlg)
	enum { IDD = IDD_IMAGE6_DIALOG };
	int		m_n;
	int		m_b;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImage6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE6DLG_H__66D6DBF0_14C1_4B6E_8256_C4460D4C0CE0__INCLUDED_)
