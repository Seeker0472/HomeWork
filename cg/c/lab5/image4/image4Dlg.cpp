// image4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "image4.h"
#include "image4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BYTE im[3][500][500]; //ͼ����ÿ�����ص�RGB
long h,w;    //ͼ��ĸ߶ȺͿ��
BYTE f[500][500];
BYTE put[3][500][500];
int n;
/////////////////////////////////////////////////////////////////////////////
// CImage4Dlg dialog

CImage4Dlg::CImage4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImage4Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImage4Dlg)
	n=3;
	m=9;
	m_r1 = FALSE;
	m_r2 = FALSE;
	m_r3 = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImage4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImage4Dlg)
	DDX_Text(pDX, IDC_EDIT1, n);
	DDX_Text(pDX, IDC_EDIT2, m);
	DDX_Check(pDX, IDC_CHECK1, m_r1);
	DDX_Check(pDX, IDC_CHECK2, m_r2);
	DDX_Check(pDX, IDC_CHECK3, m_r3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImage4Dlg, CDialog)
	//{{AFX_MSG_MAP(CImage4Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImage4Dlg message handlers

BOOL CImage4Dlg::OnInitDialog()
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

void CImage4Dlg::OnPaint() 
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
HCURSOR CImage4Dlg::OnQueryDragIcon()
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

void CImage4Dlg::OnButton1()
{
    CFileDialog dlg(TRUE); // ������ļ��Ի������
    if (dlg.DoModal() == IDOK) // ��ʾ�ļ��Ի����ж��û��Ƿ�ѡ����ͼ��
    {   if (ReadBmpImage(dlg.GetFileName(), im, h, w)) // ��ȡͼ��
        {
            CDC *p= GetDlgItem(IDC_IMAGE)->GetDC();
            DisColorImage(p, im, h, w); // ��ʾԭʼͼ��
			ReleaseDC(p);
	}
        else
        {
            MessageBox(_T("����24λ BMP ͼ��"));
        }
    }
}

void Smooth1(BYTE im[3][500][500],int h,int w,int n,BYTE put[3][500][500])
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

void Smooth2(BYTE im[3][500][500],int h,int w,int n,BYTE put[3][500][500])
{for(int y=0;y<h;y++)
	for(int x=0;x<w; x++)
		for(int k=0;k<3;k++)
			{float s=0;
			 for(int i=-n;i<=n;i++)
				 s+=im[k][y][x+i];
			 put[k][y][x]=s/(2*n+1);
			}
}

void Smooth3(BYTE im[3][500][500],int h,int w,int n,BYTE put[3][500][500])
{for(int y=0;y<h;y++)
	for(int x=0;x<w; x++)
		for(int k=0;k<3;k++)
			{float s=0;
			   for(int j=-n;j<=n;j++)
				 s+=im[k][y+j][x];
			 put[k][y][x]=s/(2*n+1);
			}
}

void Inset(BYTE im[3][500][500],int h,int w,int n,BYTE put[3][500][500])
{float g[3];
 for(int y=n;y<h-n;y=y+n)
	for(int x=n;x<w-n; x=x+n)
		{g[0]=0,g[1]=0,g[2]=0;
		  for(int k=0;k<3;k++)
			{for(int i =-n;i<=n;i++)
			   for(int j =-n;j<=n;j++)
				g[k]=g[k]+im[k][y+j][x+i];
				g[k]=g[k]/((2*n+1)*(2*n+1));
			}
			for(k=0;k<3;k++)
				for(int i=-n;i<=n;i++)
				  for(int j=-n;j<=n;j++)
		put[k][y+j][x+i]=g[k];
		}
}

void Diffusion (BYTE im[3][500][500],int h,int w,int n,BYTE put[3][500][500])
{for(int y=n;y<h-n;y=y++)
   for(int x=n;x<w-n;x=x++)  //ѭ���Ǳ߽�����
	{int rx=rand()%(2*n+1)-n;
	 int ry=rand()%(2*n+1)-n; //������Χ�ڲ���x��y�������������
	 for(int k=0;k<3;k++)
		 put[k][y][x]=im[k][y+ry][x+rx];//��ǰ����ȡ�����ָ��λ�õ�����ֵ
	}
}
	
void CImage4Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{CDC *p=GetDlgItem(IDC_IMAGE)->GetDC();
	unsigned int g1, g2, g3;
	UpdateData(1);
	if(m_r1==1)
	{ 
		for(int y=point.y-m;y<=point.y+m;y++) //ƽ����ֱ��Χ
		{for(int x=point.x-m;x<=point.x+m;x++)//ƽ��ˮƽ��Χ
			{g1=0,g2=0,g3=0;
			for(int t=-n;t<=n;t++)
				for(int s=-n;s<=n;s++)
					{g1=g1+im[0][y+t][x+s],
					g2=g2+im[1][y+t][x+s],
					g3=g3+im[2][y+t][x+s];//��������������֮��
					}
			g1=g1/((2*n+1)*(2*n+1)),
			g2=g2/((2*n+1)*(2*n+1)),
			g3=g3/((2*n+1)*(2*n+1));//����ƽ��ֵ
			p->SetPixel(x,y,RGB(g1,g2,g3));
			}
		}
	}
	if(m_r2==1)
	{float g[3];
		for(int y=point.y-m;y<=point.y+m;y=y+n) //��Ƕ��ֱ��Χ
		{for(int x=point.x-m;x<=point.x+m;x=x+n)//��Ƕˮƽ��Χ
			{g[0]=0,g[1]=0,g[2]=0;
				for(int k=0;k<3;k++)
					{for(int i =-n;i<=n;i++)
						for(int j =-n;j<=n;j++)
						g[k]=g[k]+im[k][y+j][x+i];
						g[k]=g[k]/((2*n+1)*(2*n+1));
					}
			for(k=0;k<3;k++)
				for(int i=-n;i<=n;i++)
				  for(int j=-n;j<=n;j++)
				  p->SetPixel(x+i,y+j,RGB(g[0],g[1],g[2]));
			}
		}
	}

	if(m_r3==1)
	{
		for(int y=point.y-m;y<=point.y+m;y++)
		{for(int x=point.x-m;x<=point.x+m;x++)
			{g1=0,g2=0,g3=0;
			int rx=rand()%(2*n+1)-n;
	        int ry=rand()%(2*n+1)-n; //������Χ�ڲ���x��y�������������
	        for(int k=0;k<3;k++)
				g1=im[0][y+ry][x+rx],
				g2=im[1][y+ry][x+rx],
				g3=im[2][y+ry][x+rx];//��������������֮��
				p->SetPixel(x,y,RGB(g1,g2,g3));
			}
		}
	}
    ReleaseDC(p);
    CDialog::OnLButtonDown(nFlags, point);
}

void CImage4Dlg::OnButton2() 
{
	UpdateData(1);   //���༭�����û�����������ݸ�����Ӧ�ĳ�Ա����
    int i=n;
    Smooth1(im,h,w,i,put);
	CDC *p=GetDlgItem(IDC_IMAGE1)->GetDC();  //CDC��ָ��ָ��IDC_OUTͼ���
	DisColorImage(p, put, h, w);            //��IDC_OUT����ʾ�Ҷ�ͼ��
	ReleaseDC(p);
}

void CImage4Dlg::OnButton5() 
{
	UpdateData(1);   //���༭�����û�����������ݸ�����Ӧ�ĳ�Ա����
    int i=n;
    Smooth2(im,h,w,i,put);
	CDC *p=GetDlgItem(IDC_IMAGE2)->GetDC();  //CDC��ָ��ָ��IDC_OUTͼ���
	DisColorImage(p, put, h, w);            //��IDC_OUT����ʾ�Ҷ�ͼ��
	ReleaseDC(p);
}

void CImage4Dlg::OnButton7() 
{
	UpdateData(1);   //���༭�����û�����������ݸ�����Ӧ�ĳ�Ա����
    int i=n;
    Smooth3(im,h,w,i,put);
	CDC *p=GetDlgItem(IDC_IMAGE3)->GetDC();  //CDC��ָ��ָ��IDC_OUTͼ���
	DisColorImage(p, put, h, w);            //��IDC_OUT����ʾ�Ҷ�ͼ��
	ReleaseDC(p);
}

void CImage4Dlg::OnButton3() 
{
	UpdateData(1);   //���༭�����û�����������ݸ�����Ӧ�ĳ�Ա����
    int i=n;
    Inset(im,h,w,i,put);
	CDC *p=GetDlgItem(IDC_IMAGE4)->GetDC();  //CDC��ָ��ָ��IDC_OUTͼ���
	DisColorImage(p, put, h, w);            //��IDC_OUT����ʾ�Ҷ�ͼ��
	ReleaseDC(p);
}

void CImage4Dlg::OnButton4() 
{
	UpdateData(1);   //���༭�����û�����������ݸ�����Ӧ�ĳ�Ա����
    int i=n;
    Diffusion(im,h,w,i,put);
	CDC *p=GetDlgItem(IDC_IMAGE5)->GetDC();  //CDC��ָ��ָ��IDC_OUTͼ���
	DisColorImage(p, put, h, w);            //��IDC_OUT����ʾ�Ҷ�ͼ��
	ReleaseDC(p);
}

void CImage4Dlg::OnButton6() 
{
	EndDialog(IDOK);
}

void CImage4Dlg::OnButton8() 
{
	RECT r;	
	r.left=0;
	r.right=5000;
	r.top=0;
	r.bottom=5000;
	InvalidateRect(&r,1);	
}
