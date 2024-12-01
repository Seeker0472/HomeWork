// image6Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "image6.h"
#include "image6Dlg.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BYTE im[3][500][500]; //图像中每个像素的RGB
BYTE g[500][500];
long h,w;    //图像的高度和宽度
int e=1;
BYTE f[500][500];
BYTE l[500][500];
BYTE j[500][500];
BYTE m[500][500];
BYTE o[500][500];
BYTE s[31][31];
int n;
BYTE mb;
/////////////////////////////////////////////////////////////////////////////
// CImage6Dlg dialog

CImage6Dlg::CImage6Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImage6Dlg::IDD, pParent)
{for(int i=0;i<7;i++)
	for(int j=0;j<7;j++)
		m_s[i][j]=1;
	//{{AFX_DATA_INIT(CImage6Dlg)
	m_n = 5;
	m_b = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage6Dlg::DoDataExchange(CDataExchange* pDX)
{CEdit *pE=(CEdit *)GetDlgItem(IDC_EDIT1);
 int nID=pE->GetDlgCtrlID();
 for(int i=0;i<7;i++)
	 for(int j=0;j<7;j++)
		 DDX_Text(pDX,nID++,m_s[i][j]);
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImage6Dlg)
	DDX_Text(pDX, IDC_EDIT50, m_n);
	DDX_Radio(pDX, IDC_RADIO1, m_b);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImage6Dlg, CDialog)
	//{{AFX_MSG_MAP(CImage6Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage6Dlg message handlers

BOOL CImage6Dlg::OnInitDialog()
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

void CImage6Dlg::OnPaint() 
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
HCURSOR CImage6Dlg::OnQueryDragIcon()
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

void CImage6Dlg::OnButton1()
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

int BasicGlobal(BYTE f[][500], int h, int w, int e)
{float s =0,t0,t,m1,m2;
 int n1,n2,x,y;
 for(y=0;y<h;y++)//计算图像像素的灰度平均值
    for(x=0;x<w;x++)
       s=s+f[y][x];
       t0=s/(h*w); // 阈值初值为平均灰度
    for(int k=0;k<100;k++)    //迭代计算阈值
    {   n1=0; //初始化计数器
        n2=0;
        m1=0;
        m2=0;
        for(y=0;y<h;y++)//根据当前阈值分两类，计算每类的平均灰度
        {for(x=0;x<w;x++)
            {if(f[y][x]>t0)
                {m1+=f[y][x];
                    n1++;
                }
                else
                {
                    m2+= f[y][x];
                    n2++;
                }
            }
        }
        if(n1==0||n2==0) break;//防止除零错误
        t=(m1/n1+m2/n2)/2;//重新计算阈值
        if(fabs(t-t0)<e)break;// 检查阈值是否收敛
        t0=t;
    }
    return (int)(t+0.5); //返回最终阈值四舍五入
}

void GrayToTwo(BYTE f[][500],int h,int w,int t,BYTE g[][500])
{for(int y=0;y<h;y++)
    {for(int x=0;x<w;x++)
        {if(f[y][x]<t)
                g[y][x]=0; //阈值以下设为黑色
            else
                g[y][x]=255; //阈值以上设为白色
        }
    }
}

void Erosion(BYTE f[][500],int h,int w,BYTE s[][31],int n,BYTE mb,BYTE g[][500])
{BYTE a[500][500];float sum;
 int num=0;
 for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
		if(s[i][j]==1) num++;
 for(int y=0;y<h;y++)
    for(int x=0;x<w;x++)
		if(f[y][x]==mb) a[y][x]=1; 
		else a[y][x]=0;
 for(y=0;y<h;y++)
    for(int x=0;x<w;x++)
	{if(y<n/2||x<n/2||y>=h-n/2||x>=w-n/2) 
		g[y][x]=255-mb;//边界设为背景
	 else 
	 {sum=0;
	  for(int j=0;j<n;j++)
		for(int i=0;i<n;i++)
		 sum=sum+a[y+j-n/2][x+i-n/2]*s[j][i];
	  if(sum==num)g[y][x]=mb;
	  else g[y][x]=255-mb;
	 }
	}
}

void Dilation(BYTE f[][500],int h,int w,BYTE s[][31],int n,BYTE mb,BYTE g[][500])
{BYTE a[500][500];float sum;
 for(int y=0;y<h;y++)
    for(int x=0;x<w;x++)
		if(f[y][x]==mb) a[y][x]=1; 
		else a[y][x]=0;
 for(y=0;y<h;y++)
    for(int x=0;x<w;x++)
	{if(y<n/2||x<n/2||y>=h-n/2||x>=w-n/2) 
		g[y][x]=255-mb;//边界设为背景
	 else 
	 {sum=0;
	  for(int j=0;j<n;j++)
		for(int i=0;i<n;i++)
		 sum=sum+a[y+j-n/2][x+i-n/2]*s[j][i];
	  if(sum>=1)g[y][x]=mb;
	  else g[y][x]=255-mb;
	 }
	}
}

void Edge(BYTE f[][500],int h,int w,BYTE mb,BYTE g[][500])
{for(int y=1;y<h-1;y++)
	for(int x=1;x<w-1;x++)
		if(f[y][x]==mb&&f[y+1][x]==mb&&f[y-1][x]==mb&&f[y][x+1]==mb&&f[y][x-1]==mb)
			g[y][x]=255-mb;
		else
			g[y][x]=f[y][x];
}

void HitMis(BYTE f[][500],int h,int w,BYTE s1[11][11],int n1,BYTE s2[11][11],int n2,BYTE mb,BYTE g[ ][500] )
{int x,y,i,j;BYTE a[500][500];
 int num=0;
 for(y=0;y<n1;y++)
  for(x=0;x<n1;x++)
	if(s1[y][x]==1) num++;
 for(y=0;y<h;y++)
  for(x=0;x<w;x++)
    if(f[y][x]==mb)a[y][x]=1;
    else a[y][x]=0;
 for(y=0;y<h;y++)
  for(x=0;x<w;x++)
  {if(y<n1/2||x<n1/2||y>h-n1/2||x>w-n1/2) g[y][x]=255-mb;
  else
  {int sum1=0;
   for(j=0;j<n1;j++)
    for(i=0;i<n1;i++) sum1=sum1+a[y+j-n1/2][x+i-n1/2]*s1[j][i];
   if(sum1 == num)
	{int sum2=0;
	 for(j=0;j<n2;j++)
	  for(i=0;i<n2;i++)sum2=sum2+a[y+j-n2/2][x+i-n2/2]*s2[j][i];
	 if(sum2==0)g[y][x]=mb;
	 else g[y][x]=255-mb;
   }
else g[y][x]=255-mb;
  }
  }
}

void Thinning(BYTE f[500][500],int h,int w,int mb,BYTE g[500][500])
{int x,y,i,j; BYTE g1[500][500];
BYTE s1[8][11][11]={{{0,0,0},{0,1,0},{1,1,1}},{{1,0,0},{1,1,0},{1,0,0}},
					{{1,1,1},{0,1,0},{0,0,0}},{{0,0,1},{0,1,1},{0,0,1}},
					{{0,0,0},{1,1,0},{1,1,0}},{{1,1,0},{1,1,0},{0,0,0}},
					{{0,1,1},{0,1,1},{0,0,0}},{{0,0,0},{0,1,1},{0,1,1}}};
BYTE s2[8][11][11]={{{1,1,1},{0,0,0},{0,0,0}},{{0,0,1},{0,0,1},{0,0,1}},
					{{0,0,0},{0,0,0},{1,1,1}},{{1,0,0},{1,0,0},{1,0,0}},
					{{0,1,1},{0,0,1},{0,0,0}},{{0,0,0},{0,0,1},{0,1,1}},
					{{0,0,0},{1,0,0},{1,1,0}},{{1,1,0},{1,0,0},{0,0,0}}};
 for(y=0;y<h;y++)
  for(x=0;x<w;x++)
   g1[y][x]=f[y][x];
 while(1)
 {int bz=1;
   for(i=0;i<8;i++)
	{HitMis(g1,h,w,s1[i],3,s2[i],3,mb,g);
	for(y=0;y<h;y++)
	 for(x=0;x<w;x++)
		{if(g[y][x]==mb)g[y][x]=255-mb,bz=0;
		else g[y][x]=g1[y][x];
		g1[y][x]=g[y][x];
		}
	if(bz==1)break;
   }
if(bz == 1)break;
 }
}

void CImage6Dlg::OnButton2() 
{CDC *p=GetDlgItem(IDC_IMAGE2)->GetDC();     
 int t=BasicGlobal(g,h,w,e);
 GrayToTwo(g,h,w,t,l);
 DispGrayImage(p,l,h,w);
 ReleaseDC(p);
}

void CImage6Dlg::OnButton3() 
{UpdateData(1);
 int n=m_n;
 CDC *p=GetDlgItem(IDC_IMAGE3)->GetDC();
 if(m_b==0){
	 mb=0;}
 else{
	 mb=255;}
 for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
         s[i][j]=m_s[i][j];
 Erosion(l,h,w,s,n,mb,j);
 DispGrayImage(p,j,h,w);
 for(int y=0;y<h;y++)
    for(int x=0;x<w;x++)
		l[x][y]=j[x][y];
 ReleaseDC(p);
}

void CImage6Dlg::OnButton4() 
{UpdateData(1);
 int n=m_n;
 CDC *p=GetDlgItem(IDC_IMAGE4)->GetDC();
 if(m_b==0){
	 mb=0;}
 else{
	 mb=255;}
 for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
         s[i][j]=m_s[i][j];
 Dilation(l,h,w,s,n,mb,j);
 DispGrayImage(p,j,h,w);
 for(int y=0;y<h;y++)
   for(int x=0;x<w;x++)
	   l[x][y]=j[x][y];
 ReleaseDC(p);
}

void CImage6Dlg::OnButton5() 
{
	EndDialog(IDOK);
}

void CImage6Dlg::OnButton7() 
{ UpdateData(1);
  CDC *p=GetDlgItem(IDC_IMAGE5)->GetDC();
  if(m_b==0){
	 mb=0;}
  else{
	 mb=255;}
  Edge(l,h,w,mb,m);	
  DispGrayImage(p,m,h,w);
  ReleaseDC(p);
}

void CImage6Dlg::OnButton8() 
{
    UpdateData(1); // 更新数据
    CDC *p = GetDlgItem(IDC_IMAGE6)->GetDC(); // 假设你有一个控件用于显示细化结果
    Thinning(l,h,w,mb,o);
    DispGrayImage(p, o ,h,w);
    ReleaseDC(p); // 释放设备上下文
	
}

void CImage6Dlg::OnButton6() 
{
RECT r;	
r.left=0;
r.right=5000;
r.top=0;
r.bottom=5000;
InvalidateRect(&r,1);		
}
