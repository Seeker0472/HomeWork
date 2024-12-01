// image5Dlg.h : header file
//

#if !defined(AFX_IMAGE5DLG_H__27375F4C_777E_4218_8DAF_69F1F19F576F__INCLUDED_)
#define AFX_IMAGE5DLG_H__27375F4C_777E_4218_8DAF_69F1F19F576F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImage5Dlg dialog

class CImage5Dlg : public CDialog
{
// Construction
public:
	CImage5Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImage5Dlg)
	enum { IDD = IDD_IMAGE5_DIALOG };
	BOOL	m_c1;
	BOOL	m_c2;
	BOOL	m_c3;
	int		d;
	float	s;
	int		m_r;
	float	amplitude;
	float	frequency;
	BOOL	m_c4;
	BOOL	m_c5;
	BOOL	m_c6;
	float	phase;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImage5Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButton8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE5DLG_H__27375F4C_777E_4218_8DAF_69F1F19F576F__INCLUDED_)
