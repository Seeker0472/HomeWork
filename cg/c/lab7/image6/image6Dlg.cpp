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
BYTE im[3][500][500]; //ͼ����ÿ�����ص�RGB
BYTE g[500][500];
long h,w;    //ͼ��ĸ߶ȺͿ��
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

//��ȡͼ��ȫ�ֺ����Ĵ���
BOOL ReadBmpImage(CString FileName, BYTE im[3][500][500],long &h,long &w)
{    char bm[2];short bit; CFile file; int c;           //������ر���
     file.Open(FileName,CFile::modeRead);               //�Զ���ʽ���ļ�
     file.Read(bm,2);                                   //���ļ�ͷ�����ֽ�
     if (bm[0]!='B'|| bm[1]!='M') return(0);            //�粻��BMP�ļ�����0
     file.Seek(16,CFile::current);                      //����16�����õ��ֽ�
     file.Read(&w,4);file.Read(&h,4);                   //�����ؿ�ȴ���w���߶ȴ���h
     file.Seek(2,CFile::current);                       //����2�����õ��ֽ�
     file.Read(&bit,2);                                 //��������ռ��λ��
     if (bit!=24) return(false);                        //�������24Ϊ���ͼ����0
     file.Seek(24,CFile::current);                      //����24�����õ��ֽ�
     c=w*3%4;                                           //���c����0��ʾÿ���ֽ�����4�ı���
     for (int y=h-1;y>=0;y--)                           //�������һ�е���һ�е�˳��
     { for (int x=0;x<w;x++)                            //ÿ�а������ҵ�˳��
           for(int k=2;k>=0;k--)                        //ÿ�����ذ�BGR��˳��
           file.Read(&im[k][y][x],1);                   //��(x,y)������RGB����im��
        if(c!=0)file.Seek(4-c,CFile::current);          //���������ֽ�
      }
      file.Close();                                     //�ر��ļ�
      return(1);                                        //��ȡ�ɹ�������1
}

//��ʾ��ɫͼ��ȫ�ֺ����Ĵ���
void DisColorImage(CDC *p,BYTE im[3][500][500],long h,long w)
{    for (int y =0;y<h;y++)                             //���ӵ�һ�е����һ�е�˳��
          for (int x = 0;x<w;x++)                       //ÿ�а������ҵ�˳��
        p->SetPixel(x,y,RGB(im[0][y][x],im[1][y][x],im[2][y][x])); //��ʾÿ������
}

//�Ҷ�ͼ����ʾ����
void DispGrayImage( CDC* pDC,BYTE g[500][500],long h,long w)
{    for (int y=0;y<h;y++)
       for ( int x =0;x<w;x++)
          pDC->SetPixel(x,y,RGB(g[y][x],g[y][x],g[y][x]));
}

// ƽ���ҶȻ�����
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
    CFileDialog dlg(TRUE); // ������ļ��Ի������
    if (dlg.DoModal() == IDOK) // ��ʾ�ļ��Ի����ж��û��Ƿ�ѡ����ͼ��
    {   if (ReadBmpImage(dlg.GetFileName(), im, h, w)) // ��ȡͼ��
        {
            CDC *pOriginal = GetDlgItem(IDC_IMAGE)->GetDC();
            DisColorImage(pOriginal, im, h, w); // ��ʾԭʼͼ��
            // ����ƽ���Ҷ�ֵ
            AverageGrayscale(im, g, h, w);
            // ����һ��ͼƬ������ʾ�Ҷ�ͼ��
            CDC *p = GetDlgItem(IDC_IMAGE1)->GetDC();    // ��ȡ��ʾ�ؼ����豸������
            DispGrayImage(p,g,h,w);
			ReleaseDC(p);
	}
        
        else
        {
            MessageBox(_T("����24λ BMP ͼ��"));
        }
    }
}

int BasicGlobal(BYTE f[][500], int h, int w, int e)
{float s =0,t0,t,m1,m2;
 int n1,n2,x,y;
 for(y=0;y<h;y++)//����ͼ�����صĻҶ�ƽ��ֵ
    for(x=0;x<w;x++)
       s=s+f[y][x];
       t0=s/(h*w); // ��ֵ��ֵΪƽ���Ҷ�
    for(int k=0;k<100;k++)    //����������ֵ
    {   n1=0; //��ʼ��������
        n2=0;
        m1=0;
        m2=0;
        for(y=0;y<h;y++)//���ݵ�ǰ��ֵ�����࣬����ÿ���ƽ���Ҷ�
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
        if(n1==0||n2==0) break;//��ֹ�������
        t=(m1/n1+m2/n2)/2;//���¼�����ֵ
        if(fabs(t-t0)<e)break;// �����ֵ�Ƿ�����
        t0=t;
    }
    return (int)(t+0.5); //����������ֵ��������
}

void GrayToTwo(BYTE f[][500],int h,int w,int t,BYTE g[][500])
{for(int y=0;y<h;y++)
    {for(int x=0;x<w;x++)
        {if(f[y][x]<t)
                g[y][x]=0; //��ֵ������Ϊ��ɫ
            else
                g[y][x]=255; //��ֵ������Ϊ��ɫ
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
		g[y][x]=255-mb;//�߽���Ϊ����
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
		g[y][x]=255-mb;//�߽���Ϊ����
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
    UpdateData(1); // ��������
    CDC *p = GetDlgItem(IDC_IMAGE6)->GetDC(); // ��������һ���ؼ�������ʾϸ�����
    Thinning(l,h,w,mb,o);
    DispGrayImage(p, o ,h,w);
    ReleaseDC(p); // �ͷ��豸������
	
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
