// image2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "image2.h"
#include "image2Dlg.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BYTE im[3][500][500]; //图像中每个像素的RGB
long h,w;    //图像的高度和宽度
BYTE f[500][500];
float a[3][3],b[3][3];
BYTE g[500][500];
BYTE i[500][500];
BYTE j[500][500];
BYTE l[500][500];
int n=3;

/////////////////////////////////////////////////////////////////////////////
// CImage2Dlg dialog

CImage2Dlg::CImage2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImage2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImage2Dlg)
	m_m00 = 0.0f;
	m_n00 = 0.0f;
	m_n02 = 0.0f;
	m_n01 = 0.0f;
	m_n10 = 0.0f;
	m_n11 = 0.0f;
	m_n12 = 0.0f;
	m_n20 = 0.0f;
	m_n21 = 0.0f;
	m_n22 = 0.0f;
	m_m01 = 0.0f;
	m_m02 = 0.0f;
	m_m10 = 0.0f;
	m_m11 = 0.0f;
	m_m12 = 0.0f;
	m_m20 = 0.0f;
	m_m21 = 0.0f;
	m_m22 = 0.0f;
	m_r = -1;
	m_r2 = -1;
	m_r4 = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImage2Dlg)
	DDX_Text(pDX, IDC_EDIT1, m_m00);
	DDX_Text(pDX, IDC_EDIT10, m_n00);
	DDX_Text(pDX, IDC_EDIT12, m_n02);
	DDX_Text(pDX, IDC_EDIT11, m_n01);
	DDX_Text(pDX, IDC_EDIT13, m_n10);
	DDX_Text(pDX, IDC_EDIT14, m_n11);
	DDX_Text(pDX, IDC_EDIT15, m_n12);
	DDX_Text(pDX, IDC_EDIT16, m_n20);
	DDX_Text(pDX, IDC_EDIT17, m_n21);
	DDX_Text(pDX, IDC_EDIT18, m_n22);
	DDX_Text(pDX, IDC_EDIT2, m_m01);
	DDX_Text(pDX, IDC_EDIT3, m_m02);
	DDX_Text(pDX, IDC_EDIT4, m_m10);
	DDX_Text(pDX, IDC_EDIT5, m_m11);
	DDX_Text(pDX, IDC_EDIT6, m_m12);
	DDX_Text(pDX, IDC_EDIT7, m_m20);
	DDX_Text(pDX, IDC_EDIT8, m_m21);
	DDX_Text(pDX, IDC_EDIT9, m_m22);
	DDX_Radio(pDX, IDC_RADIO1, m_r);
	DDX_Radio(pDX, IDC_RADIO3, m_r2);
	DDX_Radio(pDX, IDC_RADIO5, m_r4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImage2Dlg, CDialog)
	//{{AFX_MSG_MAP(CImage2Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage2Dlg message handlers

BOOL CImage2Dlg::OnInitDialog()
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

void CImage2Dlg::OnPaint() 
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
HCURSOR CImage2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//反转变换函数
void Reversion(BYTE f[500][500],long h,long w,BYTE g[500][500])
{for(int y=0;y<h;y++)
     for(int x=0;x<w;x++)
		 g[y][x]=255-f[y][x];
}

void Power(BYTE f[500][500],long h,long w,float b,BYTE g[500][500])
{for (int y=0;y<h;y ++)
     for (int x =0;x<w;x++)
         g[y][x]=255*pow(f[y][x]/255.0,b);
}

//读取图像全局函数的代码
BOOL ReadBmpImage(CString FileName, BYTE im[3][500][500],long &h,long &w)
{    char bm[2];short bit; CFile file; int c;           //定义相关变量
     file.Open(FileName,CFile::modeRead);               //以读方式打开文件
     file.Read(bm,2);                                   //读文件头两个字节
     if (bm[0]!='B'|| bm[1]!='M') return(0);            //如不是BMP文件返回0
     file.Seek(16,CFile::current);                      //跳过16个不用的字节
     file.Read(&w,4);file.Read(&h,4);                   //读像素宽度存入w，高度存入h
     file.Seek(2,CFile::current);                       //跳过2个不用的字节
     file.Read(&bit,2);                                 //读像素所占的位数
     if (bit!=24) return(false);                        //如果不是24为真彩图返回0
     file.Seek(24,CFile::current);                      //跳过24个不用的字节
     c=w*3%4;                                           //如果c等于0表示每行字节数是4的倍数
     for (int y=h-1;y>=0;y--)                           //按从最后一行到第一行的顺序
     { for (int x=0;x<w;x++)                            //每行按从左到右的顺序
           for(int k=2;k>=0;k--)                        //每个像素按BGR的顺序
           file.Read(&im[k][y][x],1);                   //读(x,y)处像素RGB存入im中
        if(c!=0)file.Seek(4-c,CFile::current);          //跳过多余字节
      }
      file.Close();                                     //关闭文件
      return(1);                                        //读取成功，返回1
}

//显示彩色图像全局函数的代码
void DisColorImage(CDC *p,BYTE im[3][500][500],long h,long w)
{    for (int y =0;y<h;y++)                             //按从第一行到最后一行的顺序
          for (int x = 0;x<w;x++)                       //每行按从左到右的顺序
        p->SetPixel(x,y,RGB(im[0][y][x],im[1][y][x],im[2][y][x])); //显示每个像素
}

//灰度图像显示函数
void DispGrayImage( CDC* pDC,BYTE g[500][500],long h,long w)
{    for (int y=0;y<h;y++)
       for ( int x =0;x<w;x++)
          pDC->SetPixel(x,y,RGB(g[y][x],g[y][x],g[y][x]));
}

// 平均灰度化函数
void AverageGrayscale(BYTE im[3][500][500], BYTE g[500][500], long h, long w)
{for (int y = 0; y < h; ++y)
    {for (int x = 0; x < w; ++x)
        {   int avg = (im[0][y][x] + im[1][y][x] + im[2][y][x]) / 3;
            g[y][x] = static_cast<BYTE>(avg);
        }
    }
}

void CImage2Dlg::OnButton1()
{
    CFileDialog dlg(TRUE); // 定义打开文件对话框对象
    if (dlg.DoModal() == IDOK) // 显示文件对话框，判断用户是否选择了图像
    {   if (ReadBmpImage(dlg.GetFileName(), im, h, w)) // 读取图像
        {
            CDC *pOriginal = GetDlgItem(IDC_IMAGE)->GetDC();
            DisColorImage(pOriginal, im, h, w); // 显示原始图像
            // 计算平均灰度值
            AverageGrayscale(im, g, h, w);
            // 在另一个图片框中显示灰度图像
            CDC *p = GetDlgItem(IDC_IMAGE1)->GetDC();    // 获取显示控件的设备上下文
            DispGrayImage(p,g,h,w);
			ReleaseDC(p);
	}
        
        else
        {
            MessageBox(_T("不是24位 BMP 图像"));
        }
    }
}

void Filtering(BYTE f[][500],long h,long w,float W[3][3],int n,BYTE g[][500])
{for(int y=0;y<h;y++)
   for(int x=0;x<w;x++) //循环图像中所有像素
   {if(y<n/2||y>h-n/2||x<n/2||x>=w-n/2) g[y][x]=f[y][x];//边界像素取原像素值
   float sum=0;
   for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++) //循环滤波器内的有像素
			sum=sum+W[j+1][i+1]*f[y+j][x+i];
		   if(sum<0) g[y][x]=0; //像素灰度值范围规范
		else if(sum>255) g[y][x]=255;
		else g[y][x]=sum;
   }
}

void Filtering2(BYTE f[][500],long h,long w,float W1[3][3],float W2[3][3],int n,BYTE g[][500])
{for(int y=0;y<h;y++)
   for(int x=0;x<w;x++) //循环图像中所有像素
   {if(y<1||y>h-1||x<1||x>=w-1)
   {g[y][x]=f[y][x];continue;}//边界像素取原像素值
   float s1=0,s2=0;
   for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++) 
			s1=s1+W1[j+1][i+1]*f[y+j][x+i]; //第一个滤波器运算
	for(i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++) 
			s2=s2+W2[j+1][i+1]*f[y+j][x+i]; //第二个滤波器运算
		s1=fabs(s1)+fabs(s2);   
		if(s1>255) g[y][x]=255; //像素灰度值范围规范
		else g[y][x]=s1;
   }
}

void CImage2Dlg::OnRadio1() 
{
	a[0][0]=m_m00=0,a[0][1]=m_m01=0,a[0][2]=m_m02=0,
	a[1][0]=m_m10=0,a[1][1]=m_m11=1,a[1][2]=m_m12=-1,
	a[2][0]=m_m20=0,a[2][1]=m_m21=0,a[2][2]=m_m22=0,
	b[0][0]=m_n00=0,b[0][1]=m_n01=0,b[0][2]=m_n02=0,
	b[1][0]=m_n10=0,b[1][1]=m_n11=1,b[1][2]=m_n12=0,
	b[2][0]=m_n20=0,b[2][1]=m_n21=-1,b[2][2]=m_n22=0;
	m_r=0; //使"梯度"单选按钮选中
	UpdateData(0); //使控件值与成员变量值相同
}

void CImage2Dlg::OnRadio2() 
{
    a[0][0]=m_m00=0,a[0][1]=m_m01=0,a[0][2]=m_m02=0,
	a[1][0]=m_m10=0,a[1][1]=m_m11=1,a[1][2]=m_m12=0,
	a[2][0]=m_m20=0,a[2][1]=m_m21=0,a[2][2]=m_m22=-1,
	b[0][0]=m_n00=0,b[0][1]=m_n01=0,b[0][2]=m_n02=0,
	b[1][0]=m_n10=0,b[1][1]=m_n11=0,b[1][2]=m_n12=1,
	b[2][0]=m_n20=0,b[2][1]=m_n21=-1,b[2][2]=m_n22=0;
	m_r=1; //使"交叉梯度"单选按钮选中
	UpdateData(0); 	
}

void CImage2Dlg::OnRadio3() 
{
	a[0][0]=m_m00=-1,a[0][1]=m_m01=-1,a[0][2]=m_m02=-1,
	a[1][0]=m_m10=0,a[1][1]=m_m11=0,a[1][2]=m_m12=0,
	a[2][0]=m_m20=1,a[2][1]=m_m21=1,a[2][2]=m_m22=1,
	b[0][0]=m_n00=-1,b[0][1]=m_n01=0,b[0][2]=m_n02=1,
	b[1][0]=m_n10=-1,b[1][1]=m_n11=0,b[1][2]=m_n12=1,
	b[2][0]=m_n20=-1,b[2][1]=m_n21=0,b[2][2]=m_n22=1;
	m_r2=0; //使"Prewitt算子"单选按钮选中
	UpdateData(0);
}

void CImage2Dlg::OnRadio4() 
{
	a[0][0]=m_m00=-1,a[0][1]=m_m01=0,a[0][2]=m_m02=1,
	a[1][0]=m_m10=-2,a[1][1]=m_m11=0,a[1][2]=m_m12=2,
	a[2][0]=m_m20=-1,a[2][1]=m_m21=0,a[2][2]=m_m22=1,
	b[0][0]=m_n00=-1,b[0][1]=m_n01=-2,b[0][2]=m_n02=-1,
	b[1][0]=m_n10=0,b[1][1]=m_n11=0,b[1][2]=m_n12=0,
	b[2][0]=m_n20=1,b[2][1]=m_n21=2,b[2][2]=m_n22=1;
	m_r2=1; //使"Sobel算子"单选按钮选中
	UpdateData(0);	
}

void CImage2Dlg::OnRadio5() 
{	
	a[0][0]=m_m00=0,a[0][1]=m_m01=1,a[0][2]=m_m02=0,
	a[1][0]=m_m10=1,a[1][1]=m_m11=-4,a[1][2]=m_m12=1,
	a[2][0]=m_m20=0,a[2][1]=m_m21=1,a[2][2]=m_m22=0,
	m_r4=0; //使"Laplacian算子"单选按钮选中
	UpdateData(0);
}

void CImage2Dlg::OnRadio6() 
{
	a[0][0]=m_m00=0,a[0][1]=m_m01=-1,a[0][2]=m_m02=0,
	a[1][0]=m_m10=-1,a[1][1]=m_m11=5,a[1][2]=m_m12=-1,
	a[2][0]=m_m20=0,a[2][1]=m_m21=-1,a[2][2]=m_m22=0,
	m_r4=1; //使"Laplacian锐化"单选按钮选中
	UpdateData(0);
}

void CImage2Dlg::OnButton5() 
{   CDC *pDC1 = GetDlgItem(IDC_IMAGE2)->GetDC();
    CDC *pDC2 = GetDlgItem(IDC_IMAGE3)->GetDC();
	CDC *pDC3 = GetDlgItem(IDC_IMAGE4)->GetDC();
    CDC *pDC4 = GetDlgItem(IDC_IMAGE5)->GetDC();
   if (m_r == 0) // 梯度
    {
        Filtering2(g, h, w, a,b, n, i);
        DispGrayImage(pDC1, i, h, w);
		m_r = -1;
    }
    else if (m_r == 1) // 交叉梯度
    {
        Filtering2(g, h, w, a,b ,n, i);
        DispGrayImage(pDC2, i, h, w);
		m_r = -1;
    }
   if (m_r2 == 0) // Prewitt算子
    {
        Filtering2(g, h, w, a,b, n, i);
        DispGrayImage(pDC3, i, h, w);
		m_r2 = -1;
    }
    else if (m_r2 == 1) // Sobel算子
    {
        Filtering2(g, h, w, a,b ,n, i);
        DispGrayImage(pDC4, i, h, w);
		m_r2 = -1;
    }
    ReleaseDC(pDC1); // 释放控件的设备上下文
    ReleaseDC(pDC2); // 释放控件的设备上下文
	ReleaseDC(pDC3); // 释放控件的设备上下文
    ReleaseDC(pDC4); // 释放控件的设备上下文
}



void CImage2Dlg::OnButton6() 
{   CDC *pDC1 = GetDlgItem(IDC_IMAGE6)->GetDC();
    CDC *pDC2 = GetDlgItem(IDC_IMAGE7)->GetDC();
   if (m_r4 == 0) // Laplacian算子
    {
        Filtering(g, h, w, a, n, i);
        DispGrayImage(pDC1, i, h, w);
		m_r4 = -1;
    }
    else if (m_r4 == 1) // Laplacian锐化
    {
        Filtering(g, h, w, a, n, i);
        DispGrayImage(pDC2, i, h, w);
		m_r4 = -1;
    }
    ReleaseDC(pDC1); // 释放控件的设备上下文
    ReleaseDC(pDC2); // 释放控件的设备上下文
}

void CImage2Dlg::OnButton8() 
{EndDialog(IDOK);//关闭对话框	
}


void CImage2Dlg::OnButton2() 
{   CDC *pDC = GetDlgItem(IDC_IMAGE8)->GetDC();
    float b=2.0;
	Filtering(g, h, w, a ,n, i);
	m_r2 = -1;
	m_r=-1;
	m_r4=-1;
	Reversion(i,h,w,j);
	Power(j,h,w,b,l);
    DispGrayImage(pDC, l, h, w);
}
