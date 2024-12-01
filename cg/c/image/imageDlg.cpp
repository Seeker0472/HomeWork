// imageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "image.h"
#include "imageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDlg dialog

CImageDlg::CImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImageDlg, CDialog)
	//{{AFX_MSG_MAP(CImageDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageDlg message handlers

BOOL CImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL ReadBmpImage(CString FileName, BYTE im[3][500][500], long &h, long &w) {
  char bm[2];
  short bit;
  CFile file;
  int c;
  file.Open(FileName, CFile::modeRead); 
  file.Read(bm, 2);
  if (bm[0] != 'B' || bm[1] != 'M')
    return (0);
  file.Seek(16, CFile::current);
  file.Read(&w, 4);
  file.Read(&h, 4);
  file.Seek(2, CFile::current);
  file.Read(&bit, 2);
  if (bit != 24)
    return (0);
  file.Seek(24, CFile::current); c = w * 3 % 4;
  for (int y = h - 1; y >= 0; y--) {
    for (int x = 0; x < w; x++)
      for (int k = 2; k >= 0; k--)
        file.Read(&im[k][y][x], 1);
    if (c != 0)
      file.Seek(4 - c, CFile::current);
  }
  file.Close();
  return (1);
}
void DispColorImage(CDC *p, BYTE im[3][500][500], long h, long w, int dx,
                    int dy) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      p->SetPixel(x + dx, y + dy, RGB(im[0][y][x], im[1][y][x], im[2][y][x]));
}

BYTE im[3][500][500];
long h, w;
void CImageDlg::OnButton1() {
  CFileDialog dlg(true);
  if (dlg.DoModal() == IDOK) {
    CDC *p = GetDlgItem(IDC_IMAGE)->GetDC();
    ReadBmpImage(dlg.GetFileName(), im, h, w);
    DispColorImage(p, im, h, w,10,10);
  }
}
