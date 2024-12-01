// image2Dlg.h : header file
//

#if !defined(AFX_IMAGE2DLG_H__B10637E1_05C0_46DF_84A4_4D6052CA03AE__INCLUDED_)
#define AFX_IMAGE2DLG_H__B10637E1_05C0_46DF_84A4_4D6052CA03AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImage2Dlg dialog

class CImage2Dlg : public CDialog
{
// Construction
public:
	CImage2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImage2Dlg)
	enum { IDD = IDD_IMAGE2_DIALOG };
	float	m_m00;
	float	m_n00;
	float	m_n02;
	float	m_n01;
	float	m_n10;
	float	m_n11;
	float	m_n12;
	float	m_n20;
	float	m_n21;
	float	m_n22;
	float	m_m01;
	float	m_m02;
	float	m_m10;
	float	m_m11;
	float	m_m12;
	float	m_m20;
	float	m_m21;
	float	m_m22;
	int		m_r;
	int		m_r2;
	int		m_r4;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImage2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton8();
	afx_msg void OnRadio4();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE2DLG_H__B10637E1_05C0_46DF_84A4_4D6052CA03AE__INCLUDED_)
