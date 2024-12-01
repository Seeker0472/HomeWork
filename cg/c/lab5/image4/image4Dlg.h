// image4Dlg.h : header file
//

#if !defined(AFX_IMAGE4DLG_H__8A35330B_E64C_4C74_A0CD_C6B6A838499E__INCLUDED_)
#define AFX_IMAGE4DLG_H__8A35330B_E64C_4C74_A0CD_C6B6A838499E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImage4Dlg dialog

class CImage4Dlg : public CDialog
{
// Construction
public:
	CImage4Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImage4Dlg)
	enum { IDD = IDD_IMAGE4_DIALOG };
	int		n;
	BOOL	down;
	int		m_m;
	int		m;
	int		m_r;
	BOOL	m_r1;
	BOOL	m_r2;
	BOOL	m_r3;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImage4Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton7();
	afx_msg void OnButton10();
	afx_msg void OnButton6();
	afx_msg void OnButton8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE4DLG_H__8A35330B_E64C_4C74_A0CD_C6B6A838499E__INCLUDED_)
