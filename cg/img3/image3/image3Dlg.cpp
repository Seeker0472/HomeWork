// image3Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "image3.h"
#include "image3Dlg.h"
#include "math.h"
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
int n=3;
float K;
/////////////////////////////////////////////////////////////////////////////
// CImage3Dlg dialog

CImage3Dlg::CImage3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImage3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImage3Dlg)
	K = 1.0F;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImage3Dlg)
	DDX_Text(pDX, IDC_EDIT1, K);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImage3Dlg, CDialog)
	//{{AFX_MSG_MAP(CImage3Dlg)
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
// CImage3Dlg message handlers

BOOL CImage3Dlg::OnInitDialog()
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

void CImage3Dlg::OnPaint() 
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
HCURSOR CImage3Dlg::OnQueryDragIcon()
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

void CImage3Dlg::OnButton1()
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


void HIntegralProjection(BYTE f[][500], RECT r, int p[], float K)
{    int k = 0;
    for (int y = r.top; y <= r.bottom; y++) // 循环垂直范围内每行像素
    {   float s = 0;
        for (int x = r.left; x <= r.right; x++)
            s=s+f[y][x]; // 一行范围内所有像素灰度求和
        p[k++] = K * s / (r.right - r.left + 1); // 一行范围内像素平均灰度
    }
}

void DispHProjectuionCurve(CDC *pDC, RECT r, int p[])
{
    pDC->MoveTo(p[0], r.top);
    for (int y = r.top + 1; y <= r.bottom; y++) 
        pDC->LineTo(p[y - r.top], y);
}

void CImage3Dlg::OnButton2() 
{  
   
	RECT r;
    r.left = 0;  // 设置左边界
    r.top = 0;   // 设置上边界
    r.right = w; // 设置右边界
    r.bottom = h-1; // 设置下边界
    int p[500];    // 创建一个数组用于存储每行的平均灰度值
    UpdateData(TRUE); // 从控件更新数据到变量
    float i = K; // 获取用户输入的K值
    HIntegralProjection(g, r, p, i);    // 计算每行的平均灰度值
    CDC *pDC = GetDlgItem(IDC_IMAGE2)->GetDC(); // 获取设备上下文
    DispHProjectuionCurve(pDC, r, p);    // 在 IDC_IMAGE2 区域绘制曲线
    ReleaseDC(pDC);// 释放设备上下文
}



void VIntegralProjection(BYTE f[][500], RECT r, int p[], float K)
{    int k = 0;
    for (int x = r.left; x <= r.right; x++) // 循环垂直范围内每行像素
    {   float s = 0;
        for (int y = r.top; y <= r.bottom; y++)
            s=s+f[y][x]; // 一行范围内所有像素灰度求和
        p[k++] = K * s / (r.bottom- r.top + 1); // 一行范围内像素平均灰度
    }
}

void DispVProjectuionCurve(CDC *pDC, RECT r, int p[])
{
    pDC->MoveTo(r.left,p[0]);
    for (int x= r.left+1;x<= r.right; x++) 
        pDC->LineTo(x,p[x-r.left]);
}

void CImage3Dlg::OnButton4() 
{  
    RECT r;
    r.left = 0;  // 设置左边界
    r.top = 0;   // 设置上边界
    r.right = w-1; // 设置右边界
    r.bottom = h; // 设置下边界
    int p[500];    // 创建一个数组用于存储每行的平均灰度值
    UpdateData(TRUE); // 从控件更新数据到变量
    float i= K; // 获取用户输入的K值
    VIntegralProjection(g, r, p, i);    // 计算每行的平均灰度值
    CDC *pDC = GetDlgItem(IDC_IMAGE3)->GetDC();  // 获取设备上下文
    DispVProjectuionCurve(pDC, r, p);    // 在 IDC_IMAGE3 区域绘制曲线
    ReleaseDC(pDC);    // 释放设备上下文
}

void HDifferentProjection(BYTE f[][500], RECT r, int p[], float K)
{   int k = 0;
    for (int y = r.top; y <= r.bottom; y++) // 循环垂直范围内每行像素
    {   float s = 0;
        for (int x = r.left+1; x <= r.right; x++)
            s=s+fabs(f[y][x]-f[y][x-1]); // 一行范围内所有像素灰度求差的绝对值
        p[k++] = K * s / (r.right- r.left); // 一行范围内像素平均灰度
    }
}

void CImage3Dlg::OnButton3() 
{  
    RECT r;
    r.left = 0;  // 设置左边界
    r.top = 0;   // 设置上边界
    r.right = w; // 设置右边界
    r.bottom = h-1; // 设置下边界
    int p[500];    // 创建一个数组用于存储每行的平均灰度值
    UpdateData(TRUE); // 从控件更新数据到变量
    float i= K; // 获取用户输入的K值
    HDifferentProjection(g, r, p, i);    // 计算每行的平均灰度值
    CDC *pDC = GetDlgItem(IDC_IMAGE4)->GetDC();  // 获取设备上下文
    DispHProjectuionCurve(pDC, r, p);    // 在 IDC_IMAGE3 区域绘制曲线
    ReleaseDC(pDC);    // 释放设备上下文
}

void VDifferentialProjection(BYTE f[][500], RECT r, int p[], float K)
{    int k = 0;
    for (int x = r.left+1; x <= r.right; x++) // 循环垂直范围内每行像素
    {   float s = 0;
        for (int y = r.top+1; y <= r.bottom; y++)
            s=s+fabs(f[y][x]-f[y][x-1]);// 一行范围内所有像素灰度求差的绝对值
        p[k++] = K * s / (r.bottom- r.top); // 一行范围内像素平均灰度
    }
}

void CImage3Dlg::OnButton5() 
{  
    RECT r;
    r.left = 0;  // 设置左边界
    r.top = 0;   // 设置上边界
    r.right = w-1; // 设置右边界
    r.bottom = h; // 设置下边界
    int p[500];    // 创建一个数组用于存储每行的平均灰度值
    UpdateData(TRUE); // 从控件更新数据到变量
    float i= K; // 获取用户输入的K值
    VDifferentialProjection(g, r, p, i);    // 计算每行的平均灰度值
    CDC *pDC = GetDlgItem(IDC_IMAGE5)->GetDC();  // 获取设备上下文
    DispVProjectuionCurve(pDC, r, p);    // 在 IDC_IMAGE3 区域绘制曲线
    ReleaseDC(pDC);    // 释放设备上下文
}

void CImage3Dlg::OnButton6() 
{
    RECT r;	
    r.left=0;
    r.right=5000;
    r.top=0;
    r.bottom=5000;
    InvalidateRect(&r,1);	
}

void CImage3Dlg::OnButton7() 
{
EndDialog(IDOK);//关闭对话框	 	
}

void ph(int p[],int n,int s,int q[])
{	for(int i=0;i<s/2;i++)
			q[i]=p[i],q[n-1-i]=p[n-1-i]; //曲线两端的点保留原值
	for(i=s/2;i<n-s/2;i++)
		{	q[i]=0;
			for(int j=-s/2;j<=s/2;j++)q[i]=q[i]+p[i+j];
			q[i]=q[i]/s;//曲线中的点取 s 个点平均
		}
}

int Max(int p[],int n)
{	int pmax=p[0],max=0;
	for(int k=1;k<n; k++)
		if(pmax<p[k])pmax=p[k],max=k;//定位人眼水平位置
	return max ;
}


void LimMin(int q[],int y1,int y2,int p[],int &k)
{int t=0; k=0;
 for(int y=y1+1;y<y2;y ++)
	if(q[y]-q[y-1]<0)t=1;
	else if(q [y]-q[y-1]==0 && t>=1)t++;
	else if(q [y]-q[y-1]>0 && t>=1)
		{p[k]=y-1-t/2,t=0;
		if(k>0 && p[k]-p[k-1]<3)p[k-1]=p[k-1]+(p[k]-p[k-1])/2;
		else k++;
		}
}

//输入参数：原曲线 q []，曲线的坐标范围y1,y2
//输出参数：极大值位置 p []，极大值个数 k 
void LimMax(int q[],int y1,int y2,int p[],int &k)
{int t=0;k=0;
 for(int y=y1+1;y<y2;y++)
    if(q[y]-q[y-1]>0)t=1;
    else if(q[y]-q[y-1]==0 && t>=1)t++;
    else if(q[y]-q[y-1]<0 && t>=1)
		{p[k]=y-1-t/2,t=0;
		if(k>0 && p[k]-p[k -1]<3)p[k-1]=p[k-1]+(p[k]-p[k-1])/2;
		else k++;
		}
}


void CImage3Dlg::OnButton8() 
{
    RECT r; 
    int p[500], q[500]; 
    float K = 1.0f;
    CDC *pDC = GetDlgItem(IDC_IMAGE6)->GetDC(); // 获取设备上下文
    
    // 显示灰度图像
    DispGrayImage(pDC, g, h, w);

    // 设置区域范围
    r.top = 0; 
    r.bottom = h - 1; 
    r.left = 0; 
    r.right = w; 

    // 水平灰度积分投影
    HIntegralProjection(g, r, p, K);
    ph(p, h, 7, q);
    int Yeye = Max(q, h); // 获取眼睛的水平位置
    // 确定人眼范围
    r.top = Yeye - h / 10; 
    r.bottom = Yeye + h / 10; 
    r.left = 0; 
    r.right = w - 1;

    // 垂直灰度积分投影
    VIntegralProjection(g, r, p, K); 
    ph(p, w, 7, q); // 平滑曲线

    // 绘制垂直投影曲线
    pDC->MoveTo(0, q[0]); // 移动到垂直曲线的起点
    for (int x = 1; x < w; x++) { // 确保在正确的范围内绘制
        pDC->LineTo(x, q[x]); // 画垂直投影的曲线
    }

	int m,mx;
	int Xeye1=90,Xeye2=150;
	LimMin(q,3,w-3,p,m);//计算极小值
	int mid=w/2; int min=fabs(mid-p[0]);mx=p[0];
	for( x=1;x<m;x++)
		if(fabs (mid-p[x])<min)
			{min=fabs(mid-p[x]);
			mx=p[x];//离中心最近的极小值为两眼中心垂直位置 mx 
			}

   LimMax(q,3,h-3,p,m);
   for(x=0;x<m;x++)
	   if(mx>p[x]&&mx<p[x+1])
	   {Xeye1=p[x],Xeye2=p[x+1];
	   break;
	   }


/*CString message;
message.Format("Xeye1: %d, Xeye2: %d, m: %d", Xeye1, Xeye2, m);
AfxMessageBox(message);*/
    pDC->MoveTo(Xeye1, 0); // 从图像顶部开始
    pDC->LineTo(Xeye1, h); // 绘制到图像底部


	pDC->MoveTo(Xeye2, 0); // 从图像顶部开始
    pDC->LineTo(Xeye2, h); // 绘制到图像底部

    ReleaseDC(pDC); // 释放设备上下文
}





