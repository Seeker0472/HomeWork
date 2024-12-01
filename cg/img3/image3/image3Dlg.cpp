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
BYTE im[3][500][500]; //ͼ����ÿ�����ص�RGB
long h,w;    //ͼ��ĸ߶ȺͿ��
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

void CImage3Dlg::OnButton1()
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


void HIntegralProjection(BYTE f[][500], RECT r, int p[], float K)
{    int k = 0;
    for (int y = r.top; y <= r.bottom; y++) // ѭ����ֱ��Χ��ÿ������
    {   float s = 0;
        for (int x = r.left; x <= r.right; x++)
            s=s+f[y][x]; // һ�з�Χ���������ػҶ����
        p[k++] = K * s / (r.right - r.left + 1); // һ�з�Χ������ƽ���Ҷ�
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
    r.left = 0;  // ������߽�
    r.top = 0;   // �����ϱ߽�
    r.right = w; // �����ұ߽�
    r.bottom = h-1; // �����±߽�
    int p[500];    // ����һ���������ڴ洢ÿ�е�ƽ���Ҷ�ֵ
    UpdateData(TRUE); // �ӿؼ��������ݵ�����
    float i = K; // ��ȡ�û������Kֵ
    HIntegralProjection(g, r, p, i);    // ����ÿ�е�ƽ���Ҷ�ֵ
    CDC *pDC = GetDlgItem(IDC_IMAGE2)->GetDC(); // ��ȡ�豸������
    DispHProjectuionCurve(pDC, r, p);    // �� IDC_IMAGE2 �����������
    ReleaseDC(pDC);// �ͷ��豸������
}



void VIntegralProjection(BYTE f[][500], RECT r, int p[], float K)
{    int k = 0;
    for (int x = r.left; x <= r.right; x++) // ѭ����ֱ��Χ��ÿ������
    {   float s = 0;
        for (int y = r.top; y <= r.bottom; y++)
            s=s+f[y][x]; // һ�з�Χ���������ػҶ����
        p[k++] = K * s / (r.bottom- r.top + 1); // һ�з�Χ������ƽ���Ҷ�
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
    r.left = 0;  // ������߽�
    r.top = 0;   // �����ϱ߽�
    r.right = w-1; // �����ұ߽�
    r.bottom = h; // �����±߽�
    int p[500];    // ����һ���������ڴ洢ÿ�е�ƽ���Ҷ�ֵ
    UpdateData(TRUE); // �ӿؼ��������ݵ�����
    float i= K; // ��ȡ�û������Kֵ
    VIntegralProjection(g, r, p, i);    // ����ÿ�е�ƽ���Ҷ�ֵ
    CDC *pDC = GetDlgItem(IDC_IMAGE3)->GetDC();  // ��ȡ�豸������
    DispVProjectuionCurve(pDC, r, p);    // �� IDC_IMAGE3 �����������
    ReleaseDC(pDC);    // �ͷ��豸������
}

void HDifferentProjection(BYTE f[][500], RECT r, int p[], float K)
{   int k = 0;
    for (int y = r.top; y <= r.bottom; y++) // ѭ����ֱ��Χ��ÿ������
    {   float s = 0;
        for (int x = r.left+1; x <= r.right; x++)
            s=s+fabs(f[y][x]-f[y][x-1]); // һ�з�Χ���������ػҶ����ľ���ֵ
        p[k++] = K * s / (r.right- r.left); // һ�з�Χ������ƽ���Ҷ�
    }
}

void CImage3Dlg::OnButton3() 
{  
    RECT r;
    r.left = 0;  // ������߽�
    r.top = 0;   // �����ϱ߽�
    r.right = w; // �����ұ߽�
    r.bottom = h-1; // �����±߽�
    int p[500];    // ����һ���������ڴ洢ÿ�е�ƽ���Ҷ�ֵ
    UpdateData(TRUE); // �ӿؼ��������ݵ�����
    float i= K; // ��ȡ�û������Kֵ
    HDifferentProjection(g, r, p, i);    // ����ÿ�е�ƽ���Ҷ�ֵ
    CDC *pDC = GetDlgItem(IDC_IMAGE4)->GetDC();  // ��ȡ�豸������
    DispHProjectuionCurve(pDC, r, p);    // �� IDC_IMAGE3 �����������
    ReleaseDC(pDC);    // �ͷ��豸������
}

void VDifferentialProjection(BYTE f[][500], RECT r, int p[], float K)
{    int k = 0;
    for (int x = r.left+1; x <= r.right; x++) // ѭ����ֱ��Χ��ÿ������
    {   float s = 0;
        for (int y = r.top+1; y <= r.bottom; y++)
            s=s+fabs(f[y][x]-f[y][x-1]);// һ�з�Χ���������ػҶ����ľ���ֵ
        p[k++] = K * s / (r.bottom- r.top); // һ�з�Χ������ƽ���Ҷ�
    }
}

void CImage3Dlg::OnButton5() 
{  
    RECT r;
    r.left = 0;  // ������߽�
    r.top = 0;   // �����ϱ߽�
    r.right = w-1; // �����ұ߽�
    r.bottom = h; // �����±߽�
    int p[500];    // ����һ���������ڴ洢ÿ�е�ƽ���Ҷ�ֵ
    UpdateData(TRUE); // �ӿؼ��������ݵ�����
    float i= K; // ��ȡ�û������Kֵ
    VDifferentialProjection(g, r, p, i);    // ����ÿ�е�ƽ���Ҷ�ֵ
    CDC *pDC = GetDlgItem(IDC_IMAGE5)->GetDC();  // ��ȡ�豸������
    DispVProjectuionCurve(pDC, r, p);    // �� IDC_IMAGE3 �����������
    ReleaseDC(pDC);    // �ͷ��豸������
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
EndDialog(IDOK);//�رնԻ���	 	
}

void ph(int p[],int n,int s,int q[])
{	for(int i=0;i<s/2;i++)
			q[i]=p[i],q[n-1-i]=p[n-1-i]; //�������˵ĵ㱣��ԭֵ
	for(i=s/2;i<n-s/2;i++)
		{	q[i]=0;
			for(int j=-s/2;j<=s/2;j++)q[i]=q[i]+p[i+j];
			q[i]=q[i]/s;//�����еĵ�ȡ s ����ƽ��
		}
}

int Max(int p[],int n)
{	int pmax=p[0],max=0;
	for(int k=1;k<n; k++)
		if(pmax<p[k])pmax=p[k],max=k;//��λ����ˮƽλ��
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

//���������ԭ���� q []�����ߵ����귶Χy1,y2
//�������������ֵλ�� p []������ֵ���� k 
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
    CDC *pDC = GetDlgItem(IDC_IMAGE6)->GetDC(); // ��ȡ�豸������
    
    // ��ʾ�Ҷ�ͼ��
    DispGrayImage(pDC, g, h, w);

    // ��������Χ
    r.top = 0; 
    r.bottom = h - 1; 
    r.left = 0; 
    r.right = w; 

    // ˮƽ�ҶȻ���ͶӰ
    HIntegralProjection(g, r, p, K);
    ph(p, h, 7, q);
    int Yeye = Max(q, h); // ��ȡ�۾���ˮƽλ��
    // ȷ�����۷�Χ
    r.top = Yeye - h / 10; 
    r.bottom = Yeye + h / 10; 
    r.left = 0; 
    r.right = w - 1;

    // ��ֱ�ҶȻ���ͶӰ
    VIntegralProjection(g, r, p, K); 
    ph(p, w, 7, q); // ƽ������

    // ���ƴ�ֱͶӰ����
    pDC->MoveTo(0, q[0]); // �ƶ�����ֱ���ߵ����
    for (int x = 1; x < w; x++) { // ȷ������ȷ�ķ�Χ�ڻ���
        pDC->LineTo(x, q[x]); // ����ֱͶӰ������
    }

	int m,mx;
	int Xeye1=90,Xeye2=150;
	LimMin(q,3,w-3,p,m);//���㼫Сֵ
	int mid=w/2; int min=fabs(mid-p[0]);mx=p[0];
	for( x=1;x<m;x++)
		if(fabs (mid-p[x])<min)
			{min=fabs(mid-p[x]);
			mx=p[x];//����������ļ�СֵΪ�������Ĵ�ֱλ�� mx 
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
    pDC->MoveTo(Xeye1, 0); // ��ͼ�񶥲���ʼ
    pDC->LineTo(Xeye1, h); // ���Ƶ�ͼ��ײ�


	pDC->MoveTo(Xeye2, 0); // ��ͼ�񶥲���ʼ
    pDC->LineTo(Xeye2, h); // ���Ƶ�ͼ��ײ�

    ReleaseDC(pDC); // �ͷ��豸������
}





