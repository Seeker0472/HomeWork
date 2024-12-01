// image5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "image5.h"
#include "image5Dlg.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BYTE im[3][500][500]; //图像中每个像素的RGB
BYTE put[3][500][500];
long h,w;    //图像的高度和宽度
BYTE f[500][500];
int hor,ver,sym,ho,ve,sy,dx,dy;
int n=5;
/////////////////////////////////////////////////////////////////////////////
// CImage5Dlg dialog

CImage5Dlg::CImage5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImage5Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImage5Dlg)
	m_c1 = FALSE;
	m_c2 = FALSE;
	m_c3 = FALSE;
	d = 30;
	s = 2.0;
	m_r = -1;
	amplitude = 10.0;
	frequency = 1.0;
	m_c4 = FALSE;
	m_c5 = FALSE;
	m_c6 = FALSE;
	phase = 1.5;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImage5Dlg)
	DDX_Check(pDX, IDC_CHECK1, m_c1);
	DDX_Check(pDX, IDC_CHECK2, m_c2);
	DDX_Check(pDX, IDC_CHECK3, m_c3);
	DDX_Text(pDX, IDC_EDIT1, d);
	DDX_Text(pDX, IDC_EDIT2, s);
	DDX_Radio(pDX, IDC_RADIO1, m_r);
	DDX_Text(pDX, IDC_EDIT3, amplitude);
	DDX_Text(pDX, IDC_EDIT4, frequency);
	DDX_Check(pDX, IDC_CHECK4, m_c4);
	DDX_Check(pDX, IDC_CHECK5, m_c5);
	DDX_Check(pDX, IDC_CHECK6, m_c6);
	DDX_Text(pDX, IDC_EDIT5, phase);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImage5Dlg, CDialog)
	//{{AFX_MSG_MAP(CImage5Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage5Dlg message handlers

BOOL CImage5Dlg::OnInitDialog()
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

void CImage5Dlg::OnPaint() 
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
HCURSOR CImage5Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
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

void CImage5Dlg::OnButton1()
{
    CFileDialog dlg(TRUE); // 定义打开文件对话框对象
    if (dlg.DoModal() == IDOK) // 显示文件对话框，判断用户是否选择了图像
    {   if (ReadBmpImage(dlg.GetFileName(), im, h, w)) // 读取图像
        {
            CDC *p= GetDlgItem(IDC_IMAGE)->GetDC();
            DisColorImage(p, im, h, w); // 显示原始图像
			ReleaseDC(p);
	}
        else
        {
            MessageBox(_T("不是24位 BMP 图像"));
        }
    }
}

void MirrorImage(CDC*p, BYTE im[3][500][500],long h,long w,int hor,int ver,int sym,int dx,int dy)
{ if(sym)  //相对中心点的对称变换
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
		{int x1=w-x-1;int y1=h-y-1;
			p->SetPixel(x1+dx,y1+dy,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
		}
	}
else if(hor)  //水平镜像
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			{int x1=w-x-1; int y1=y;
				p->SetPixel(x1+dx,y1+dy,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
			}
	}
else if(ver)  //垂直镜像
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			{int x1=x;int y1=h-y-1;
				p->SetPixel(x1+dx,y1+dy,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
			}
	}
else //平移
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			p->SetPixel(x+dx,y+dy,RGB(im[0][y][x],im[1][y][x],im[2][y][x])) ;
	}
}

void Rotation(CDC *p,BYTE im[3][500][500],int h,int w,int d,int dx,int dy)
{float cd=d*3.14/180; int x1,y1;
 for(int y=0;y<h;y++)
	for(int x=0;x<w;x++)
		{x1=x*cos(cd)-y*sin(cd)+dx;
		 y1=x*sin(cd)+y*cos(cd)+dy;
		 p->SetPixel(x1,y1,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
		 p->SetPixel(x1+1,y1,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
         p->SetPixel(x1,y1+1,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
		}
}

void Scale(CDC *p,BYTE im[3][500][500],int h,int w,float s)
{ float x1,y1;
  for(int y=0;y<h-s/2;y++)
	for(int x=0;x<w-s/2;x++)
		{x1=s*x;y1=s*y;
		if(s<=1)
			p->SetPixel(x1,y1,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
		else
			for(int j=0;j<s;j++)
				for(int i=0;i<s;i++)
					p->SetPixel(x1+i,y1+j,RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
		}
}

void Enlarge(CDC *pDC,BYTE im[3][500][500],int h,int w,float s)
{float x1,y1,c1,c2; BYTE C[3];
 for(int y=0;y<h-1; y++)
	for(int x=0;x<w-1;x++)
		{x1=s*x, y1=s*y;
			for(int j=0;j<s;j++)
				for(int i=0;i<s;i++)
					{for(int k=0;k<3;k++)
					  c1=i/(s-1)*(1.0*im[k][y][x+1]-im[k][y][x])+im[k][y][x],
					  c2=i/(s-1)*(1.0*im[k][y+1][x+1]-im[k][y+1][x])+im[k][y+1][x],
					  C[k]=j/(s-1)*(c2-c1)+c1;
					 pDC->SetPixel(x1+i,y1+j,RGB(C[0],C[1],C[2]));
					}
		}
}

void Smooth(BYTE im[3][500][500],int h,int w,int n,BYTE put[3][500][500])
{for(int y=0;y<h;y++)
	for(int x=0;x<w; x++)
		for(int k=0;k<3;k++)
			{float s=0;
			 for(int i=-n;i<=n;i++)
			   for(int j=-n;j<=n;j++)
				 s+=im[k][y+j][x+i];
			 put[k][y][x]=s/((2*n+1)*(2*n+1));
			}
}

void SineTransform(CDC*p, BYTE im[3][500][500],long h,long w,float amplitude, float frequency,float phase,int ho,int ve,int sy,int dx,int dy)
{if(ho)
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			{int x1=x+amplitude*sin(2*3.14*frequency*y/(h-1)+phase);int y1=y;
				p->SetPixel(x1+dx, y+dy, RGB(im[0][y][x],im[1][y][x],im[2][y][x]));
            }
	}
else if(ve) 
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			{int x1=x;int y1=y+amplitude*sin(2*3.149*frequency*x/(w-1)+phase);
				p->SetPixel(x1+dx,y1+dy, RGB(im[0][y][x], im[1][y][x], im[2][y][x]));
            }
	}
else if(sy)
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			{int x1=x+amplitude*sin(2*3.14*frequency*y/(h-1)+phase);
			int y1=y+amplitude*sin(2*3.149*frequency*x/(w-1)+phase);
				p->SetPixel(x1+dx,y1+dy, RGB(im[0][y][x], im[1][y][x], im[2][y][x]));
            }
	}
else //平移
	{for(int y=0;y<h;y++)
		for(int x=0;x<w;x++)
			p->SetPixel(x+dx,y+dy,RGB(im[0][y][x],im[1][y][x],im[2][y][x])) ;
	}
}

void CImage5Dlg::OnButton2() 
{
	UpdateData(1);
	int hor=m_c1;
	int ver=m_c2;
	int sym=m_c3;
    CDC *p= GetDlgItem(IDC_IMAGE1)->GetDC();
	if(sym){
		MirrorImage(p,im,h,w,hor,ver,sym,w,h);}
	else if(hor){
		MirrorImage(p,im,h,w,hor,ver,sym,w,0);}
	else if(ver){
		MirrorImage(p,im,h,w,hor,ver,sym,0,h);}
}

void CImage5Dlg::OnButton3() 
{
	UpdateData(1);
    CDC *p=GetDlgItem(IDC_IMAGE2)->GetDC();
    Rotation(p,im,h,w,d,0,0);
}

void CImage5Dlg::OnButton4() 
{
	UpdateData(1);
    CDC *p=GetDlgItem(IDC_IMAGE3)->GetDC();
	if(m_r==0){
	Scale(p,im,h,w,s);}
	else{
	Enlarge(p,im,h,w,s);}
}

void CImage5Dlg::OnButton5() 
{
RECT r;	
r.left=0;
r.right=5000;
r.top=0;
r.bottom=5000;
InvalidateRect(&r,1);		
}

void CImage5Dlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	
}

void CImage5Dlg::OnButton7() 
{
	UpdateData(1);
    CDC *p=GetDlgItem(IDC_IMAGE5)->GetDC();
	int hor=m_c1;
	int ver=m_c2;
	int sym=m_c3;
	Smooth(im,h,w,n,put);
	MirrorImage(p,put,h,w,hor,ver,sym,0,h);
}


void CImage5Dlg::OnButton8() 
{
	UpdateData(1);
	int ho=m_c4;
	int ve=m_c5;
	int sy=m_c6;
    CDC *p= GetDlgItem(IDC_IMAGE4)->GetDC();
	if(ho){
		SineTransform(p,im,h,w,amplitude,frequency,phase,ho,ve,sy,w,0);}
	else if(ve){
		SineTransform(p,im,h,w,amplitude,frequency,phase,ho,ve,sy,0,h);}
	else if(sy){
		SineTransform(p,im,h,w,amplitude,frequency,phase,ho,ve,sy,w,h);}
}
