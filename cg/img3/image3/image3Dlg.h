// image3Dlg.h : header file
//

#if !defined(AFX_IMAGE3DLG_H__67396331_BFFE_4587_A293_DCBC6DA6B414__INCLUDED_)
#define AFX_IMAGE3DLG_H__67396331_BFFE_4587_A293_DCBC6DA6B414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImage3Dlg dialog

class CImage3Dlg : public CDialog
{
// Construction
public:
	CImage3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImage3Dlg)
	enum { IDD = IDD_IMAGE3_DIALOG };
	float	K;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImage3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImage3Dlg)
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
	afx_msg void OnButton8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE3DLG_H__67396331_BFFE_4587_A293_DCBC6DA6B414__INCLUDED_)
