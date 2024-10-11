// imageDlg.cpp : implementation file
//
#include "stdafx.h"
#include "image.h"
#include "imageDlg.h"
#include "math.h"
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CImageDlg dialog

CImageDlg::CImageDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CImageDlg::IDD, pParent) {
  //{{AFX_DATA_INIT(CImageDlg)
  m_R = 0.33f;
  m_G = 0.34f;
  m_B = 0.33f;
	m_m = 2.0f;
	m_a = 10.0f;
	m_b = 10.0f;
	m_c = 100.0f;
	m_d = 100.0f;
	//}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageDlg::DoDataExchange(CDataExchange *pDX) {
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CImageDlg)
  DDX_Text(pDX, IDC_EDIT1, m_R);
  DDX_Text(pDX, IDC_EDIT2, m_G);
  DDX_Text(pDX, IDC_EDIT3, m_B);
	DDX_Text(pDX, IDC_EDIT4, m_m);
	DDX_Text(pDX, IDC_EDIT5, m_a);
	DDX_Text(pDX, IDC_EDIT6, m_b);
	DDX_Text(pDX, IDC_EDIT7, m_c);
	DDX_Text(pDX, IDC_EDIT8, m_d);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImageDlg, CDialog)
//{{AFX_MSG_MAP(CImageDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT8, OnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeEdit7)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageDlg message handlers

BOOL CImageDlg::OnInitDialog() {
  CDialog::OnInitDialog();

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);  // Set big icon
  SetIcon(m_hIcon, FALSE); // Set small icon

  // TODO: Add extra initialization here

  return TRUE; // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageDlg::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageDlg::OnQueryDragIcon() { return (HCURSOR)m_hIcon; }

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
  file.Seek(24, CFile::current);
  c = w * 3 % 4;
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
    DispColorImage(p, im, h, w, 10, 10);
  }
}
BYTE gray[500][500];
float a[3];

void Gray(BYTE im[3][500][500], long h, long w, float a[3], BYTE f[][500]) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      f[y][x] = (a[0] * im[0][y][x] + a[1] * im[1][y][x] + a[2] * im[2][y][x]);
}

void DispGrayImage(CDC *pDC, BYTE f[][500], long h, long w) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      pDC->SetPixel(x, y, RGB(f[y][x], f[y][x], f[y][x]));
}
void CImageDlg::OnButton2() {
  CDC *p = GetDlgItem(IDC_IMAGE1)->GetDC();
  float test[3]={m_R,m_G,m_B};
  Gray(im,h,w,test,gray);
  DispGrayImage(p, gray, h, w);
}
BYTE gray1[500][500];
void Power(BYTE f[500][500], long h, long w, float b, BYTE g[500][500]) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      g[y][x] = 255 * pow(f[y][x] / 255.0, b);
}

void CImageDlg::OnButton3() 
{
  CDC *p = GetDlgItem(IDC_IMAGE2)->GetDC();
  Power(gray, h, w, m_m, gray1);
  DispGrayImage(p, gray1, h, w);
}
BYTE graylin[500][500];
// 分段线性变换的函数
void SLinear(BYTE f[][500], long h, long w, float a, float b, float c, float d,
             BYTE g[][500]) {
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      if (f[y][x] >= 0 && f[y][x] < a)
        g[y][x] = c * f[y][x] / a;
      else if (f[y][x] >= a && f[y][x] < b)
        g[y][x] = (d - c) / (b - a) * (f[y][x] - a) + c;
      else if (f[y][x] >= b && f[y][x] <= 255)
        g[y][x] = (255 - d) / (255 - b) * (f[y][x] - b) + d;
}
#include <algorithm> // For std::max



void CImageDlg::OnButton4() 
{
	CDC *p = GetDlgItem(IDC_IMAGE3)->GetDC();
  SLinear(gray, h, w, m_a,m_b,m_c,m_d, graylin);
  DispGrayImage(p, graylin, h, w);
}
BYTE graybalance[500][500];
void Histogram(BYTE f[][500], long h, long w, int hd[256]) {
  for (int i = 0; i < 256; i++)
    hd[i] = 0;
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      hd[f[y][x]]++;
}
void ImageBalance(BYTE f[500][500], int h, int w, int grad, BYTE g[500][500]) {
  float p, q[256];
  int hd[256];
  Histogram(f, h, w, hd);
  q[0] = (float)hd[0] / (h * w);
  for (int i = 1; i < 256; i++) {
    p = (float)hd[i] / (h * w);
    q[i] = q[i - 1] + p;
  }
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      g[y][x] = (int)(q[f[y][x]] * 100) / 100.0 * (grad - 1);
  // 计算均衡后的相应灰度值
}

void DrawHistogram(BYTE f[500][500], int h, int w, BYTE pic[500][500]) {
    int hd[256];
    Histogram(f, h, w, hd);
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 255; x++) {
            pic[y][x] = 255;
        }
    }
    int maxVal = 0;
    for (int i = 1; i < 256; i++) {
        if (hd[i] > maxVal) {
            maxVal = hd[i];
        }
    }
    for (int x = 1; x < 256; x++) {
        // Calculate the height of the bar
        int barHeight = (int)((hd[x] / (double)maxVal) * 250); // Scale to fit within 500px height

        // Draw the bar
        for (int y = 249; y >= 249 - barHeight; y--) {
            pic[y][x ] = 0;    
            // pic[y][x * 2 + 1] = 0;
        }
    }
}

BYTE pic[500][500];
void CImageDlg::OnButton5() 
{
	CDC *p = GetDlgItem(IDC_IMAGE4)->GetDC();
	ImageBalance(gray,h,w,255,graybalance);
  DispGrayImage(p, graybalance, h, w);

	CDC *pp = GetDlgItem(IDC_IMAGE5)->GetDC();
	DrawHistogram(graybalance,250,250,pic);
  DispGrayImage(pp, pic, 250, 250);

  	CDC *p6 = GetDlgItem(IDC_IMAGE6)->GetDC();
	DrawHistogram(gray,250,250,pic);
  DispGrayImage(p6, pic, 250, 250);

}
BYTE rev[500][500];
void Reverse(BYTE f[500][500], int h, int w, BYTE pic[500][500]) {
   for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      pic[y][x]=255-f[y][x];
}
void CImageDlg::OnButton6() 
{
	CDC *p = GetDlgItem(IDC_IMG)->GetDC();
	Reverse(gray,h,w,rev);
  DispGrayImage(p, rev, h, w);
}


void CImageDlg::OnChangeEdit1() {

}

void CImageDlg::OnChangeEdit3() {

}

void CImageDlg::OnChangeEdit2() {

}


void CImageDlg::OnChangeEdit4() 
{
}

void CImageDlg::OnChangeEdit5() 
{
	
}

void CImageDlg::OnChangeEdit6() 
{
}
void CImageDlg::OnChangeEdit8() 
{
	
}

void CImageDlg::OnChangeEdit7() 
{
	
}





