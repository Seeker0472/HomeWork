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
BYTE im[3][500][500]; //ͼ����ÿ�����ص�RGB
long h,w;    //ͼ��ĸ߶ȺͿ��
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

//��ת�任����
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

void CImage2Dlg::OnButton1()
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

void Filtering(BYTE f[][500],long h,long w,float W[3][3],int n,BYTE g[][500])
{for(int y=0;y<h;y++)
   for(int x=0;x<w;x++) //ѭ��ͼ������������
   {if(y<n/2||y>h-n/2||x<n/2||x>=w-n/2) g[y][x]=f[y][x];//�߽�����ȡԭ����ֵ
   float sum=0;
   for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++) //ѭ���˲����ڵ�������
			sum=sum+W[j+1][i+1]*f[y+j][x+i];
		   if(sum<0) g[y][x]=0; //���ػҶ�ֵ��Χ�淶
		else if(sum>255) g[y][x]=255;
		else g[y][x]=sum;
   }
}

void Filtering2(BYTE f[][500],long h,long w,float W1[3][3],float W2[3][3],int n,BYTE g[][500])
{for(int y=0;y<h;y++)
   for(int x=0;x<w;x++) //ѭ��ͼ������������
   {if(y<1||y>h-1||x<1||x>=w-1)
   {g[y][x]=f[y][x];continue;}//�߽�����ȡԭ����ֵ
   float s1=0,s2=0;
   for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++) 
			s1=s1+W1[j+1][i+1]*f[y+j][x+i]; //��һ���˲�������
	for(i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++) 
			s2=s2+W2[j+1][i+1]*f[y+j][x+i]; //�ڶ����˲�������
		s1=fabs(s1)+fabs(s2);   
		if(s1>255) g[y][x]=255; //���ػҶ�ֵ��Χ�淶
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
	m_r=0; //ʹ"�ݶ�"��ѡ��ťѡ��
	UpdateData(0); //ʹ�ؼ�ֵ���Ա����ֵ��ͬ
}

void CImage2Dlg::OnRadio2() 
{
    a[0][0]=m_m00=0,a[0][1]=m_m01=0,a[0][2]=m_m02=0,
	a[1][0]=m_m10=0,a[1][1]=m_m11=1,a[1][2]=m_m12=0,
	a[2][0]=m_m20=0,a[2][1]=m_m21=0,a[2][2]=m_m22=-1,
	b[0][0]=m_n00=0,b[0][1]=m_n01=0,b[0][2]=m_n02=0,
	b[1][0]=m_n10=0,b[1][1]=m_n11=0,b[1][2]=m_n12=1,
	b[2][0]=m_n20=0,b[2][1]=m_n21=-1,b[2][2]=m_n22=0;
	m_r=1; //ʹ"�����ݶ�"��ѡ��ťѡ��
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
	m_r2=0; //ʹ"Prewitt����"��ѡ��ťѡ��
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
	m_r2=1; //ʹ"Sobel����"��ѡ��ťѡ��
	UpdateData(0);	
}

void CImage2Dlg::OnRadio5() 
{	
	a[0][0]=m_m00=0,a[0][1]=m_m01=1,a[0][2]=m_m02=0,
	a[1][0]=m_m10=1,a[1][1]=m_m11=-4,a[1][2]=m_m12=1,
	a[2][0]=m_m20=0,a[2][1]=m_m21=1,a[2][2]=m_m22=0,
	m_r4=0; //ʹ"Laplacian����"��ѡ��ťѡ��
	UpdateData(0);
}

void CImage2Dlg::OnRadio6() 
{
	a[0][0]=m_m00=0,a[0][1]=m_m01=-1,a[0][2]=m_m02=0,
	a[1][0]=m_m10=-1,a[1][1]=m_m11=5,a[1][2]=m_m12=-1,
	a[2][0]=m_m20=0,a[2][1]=m_m21=-1,a[2][2]=m_m22=0,
	m_r4=1; //ʹ"Laplacian��"��ѡ��ťѡ��
	UpdateData(0);
}

void CImage2Dlg::OnButton5() 
{   CDC *pDC1 = GetDlgItem(IDC_IMAGE2)->GetDC();
    CDC *pDC2 = GetDlgItem(IDC_IMAGE3)->GetDC();
	CDC *pDC3 = GetDlgItem(IDC_IMAGE4)->GetDC();
    CDC *pDC4 = GetDlgItem(IDC_IMAGE5)->GetDC();
   if (m_r == 0) // �ݶ�
    {
        Filtering2(g, h, w, a,b, n, i);
        DispGrayImage(pDC1, i, h, w);
		m_r = -1;
    }
    else if (m_r == 1) // �����ݶ�
    {
        Filtering2(g, h, w, a,b ,n, i);
        DispGrayImage(pDC2, i, h, w);
		m_r = -1;
    }
   if (m_r2 == 0) // Prewitt����
    {
        Filtering2(g, h, w, a,b, n, i);
        DispGrayImage(pDC3, i, h, w);
		m_r2 = -1;
    }
    else if (m_r2 == 1) // Sobel����
    {
        Filtering2(g, h, w, a,b ,n, i);
        DispGrayImage(pDC4, i, h, w);
		m_r2 = -1;
    }
    ReleaseDC(pDC1); // �ͷſؼ����豸������
    ReleaseDC(pDC2); // �ͷſؼ����豸������
	ReleaseDC(pDC3); // �ͷſؼ����豸������
    ReleaseDC(pDC4); // �ͷſؼ����豸������
}



void CImage2Dlg::OnButton6() 
{   CDC *pDC1 = GetDlgItem(IDC_IMAGE6)->GetDC();
    CDC *pDC2 = GetDlgItem(IDC_IMAGE7)->GetDC();
   if (m_r4 == 0) // Laplacian����
    {
        Filtering(g, h, w, a, n, i);
        DispGrayImage(pDC1, i, h, w);
		m_r4 = -1;
    }
    else if (m_r4 == 1) // Laplacian��
    {
        Filtering(g, h, w, a, n, i);
        DispGrayImage(pDC2, i, h, w);
		m_r4 = -1;
    }
    ReleaseDC(pDC1); // �ͷſؼ����豸������
    ReleaseDC(pDC2); // �ͷſؼ����豸������
}

void CImage2Dlg::OnButton8() 
{EndDialog(IDOK);//�رնԻ���	
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
