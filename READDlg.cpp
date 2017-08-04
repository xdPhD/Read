
// READDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "READ.h"
#include "READDlg.h"
#include "afxdialogex.h"
#include<shlwapi.h>
#include "string.h"
#include <stdlib.h>  
#include <stdio.h>  
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
//	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_MENUSELECT()
//	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CREADDlg �Ի���



CREADDlg::CREADDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_READ_DIALOG, pParent)
	, m_Mass(0)
	, m_Stiff(0)
	, m_Damp(0)
	, m_dt(0)
	, m_pga(0)
	, m_acc(0)
	, m_velo(0)
	, m_disp(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CREADDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_Mass);
	DDX_Text(pDX, IDC_EDIT4, m_Stiff);
	DDX_Text(pDX, IDC_EDIT5, m_Damp);
	DDX_Control(pDX, IDC_ListCOMBO, m_filenames);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT2, m_dt);
	DDX_Text(pDX, IDC_EDIT6, m_pga);
	DDX_Text(pDX, IDC_EDIT8, m_acc);
	DDX_Text(pDX, IDC_EDIT9, m_velo);
	DDX_Text(pDX, IDC_EDIT10, m_disp);
}

BEGIN_MESSAGE_MAP(CREADDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DrawBUTTON, &CREADDlg::OnBnClickedDrawbutton)
	ON_BN_CLICKED(IDC_ContentBUTTON, &CREADDlg::OnBnClickedContentbutton)
	ON_BN_CLICKED(IDC_DisplayBUTTON, &CREADDlg::OnBnClickedDisplaybutton)
	ON_CBN_SELCHANGE(IDC_ListCOMBO, &CREADDlg::OnCbnSelchangeListcombo)
	ON_STN_CLICKED(IDC_STATIC_NOW, &CREADDlg::OnStnClickedStaticNow)
	
	ON_BN_CLICKED(IDC_CalculateBUTTON, &CREADDlg::OnBnClickedCalculatebutton)
	ON_EN_CHANGE(IDC_EDIT5, &CREADDlg::OnEnChangeEdit5)
	ON_STN_CLICKED(IDC_PictureSTATIC, &CREADDlg::OnStnClickedPicturestatic)
END_MESSAGE_MAP()


// CREADDlg ��Ϣ�������

BOOL CREADDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	m_Values.resize(3000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CREADDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CREADDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CREADDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CREADDlg::OnBnClickedDrawbutton()
{
	UpdateData(TRUE);
	CWnd *picture = this->GetDlgItem(IDC_PictureSTATIC); //�õ��ÿؼ��Ĵ�kouָ��
	CRect prect;			//
	picture->GetWindowRect(&prect); //�õ����ľ������򣨰�������ʹ�С��
	ScreenToClient(&prect);	//ӳ�䵽�ͻ�������

	CDC *pdc= picture->GetDC();	//�õ��ÿռ��DC	
	CBrush brush(RGB(255,0,0));// ��һ����ˢ
	pdc->SelectObject(&brush);//�󶨸û�ˢ
	pdc->MoveTo(CPoint(0, 0));//�ƶ���ԭ��  ��ʼ��ͼ
	pdc->Rectangle(0, 0, prect.Width(), prect.Height());

	for (int i = 0; i <= prect.Width(); i+=10)
	{
		int y = prect.Height()*0.5* sin(10*i)+0.5* prect.Height();
		
		pdc->LineTo(CPoint(i, y));
	}
	
	
	ReleaseDC(pdc);
}


void CREADDlg::OnBnClickedContentbutton()
{
	// TODO: Add your control notification handler code here   
	// ���ù�����   
	TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString strFilePath;
	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strFilePath = fileDlg.GetPathName();
		int pos= strFilePath.ReverseFind('\\');//���������һ��\��λ�ã�
		strFilePath=strFilePath.Left(pos);//��\��ߵ�·����ֵ��strFilePath
		SetDlgItemText(IDC_ContentEDIT, strFilePath);//��·����ʾ����
	}  // TODO: �ڴ���ӿؼ�֪ͨ����������
}

#include "Math.h"
void CREADDlg::OnBnClickedDisplaybutton()
{
	CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4 | 2,
		_TEXT("�ı��ļ�(*.txt,*.ini,*.log)|*.txt;*.ini;*.log|ȫ���ļ�(*.*)|*.*||"));
	// ���� DoModal() �������򿪶Ի���  
	if (fDlg.DoModal() == IDOK)
	{
		// ���ñ༭��1������Ϊѡ���ļ���·��  
		// GetPathName() --> ����ѡ���ļ�������·��  
		SetDlgItemText(IDC_ContentEDIT, fDlg.GetPathName());
		CFile fFile(fDlg.GetPathName(), CFile::modeRead);
		// ��Ϊ��Windows�³��õ��ı��������ΪANSI  
		// ���������ڵ���ѡ���ʱ��ѡ����Unicode����  
		// ������Ҫʹ��CStringA���͵ı�����  
		CStringA strFile;
		// ��ȡ���ļ�����  
		// ��һ����������ȡ���ݵĻ�������  
		// �ڶ�����������ȡ���ݵĳ���  
		int size_str = fFile.GetLength();//�õ��ļ��ĳ���
		char* pChars = new char[size_str];//����һ���ó��ȵ��ַ�����
		char* fomerPchars = pChars;//pChars ���ں�����ģ�formerPchars��һ�����ݣ�����������ɾ�����ַ�����
		fFile.Read(pChars, size_str);  //���ļ�����pChars��������
		int lines_Start = 0;
		m_Values.clear();				//������� ��������double����ռ�
		for (int i = 0; i < size_str; i++)		//��ʼһ��һ���ֽڵ����ļ�ĩβ��
		{
		
			if (*pChars == '\r' && *(pChars+1)=='\n')  //����������ַ���ת�к����з����������Ļ��з������ͱ�ʾ����һ��
			{
				char temp[20] = {'0'};		//�½���ʱ�ַ����������洢��������
				memcpy(temp, fomerPchars+lines_Start,i-lines_Start);	//�洢�������ݣ�lines_Start��ʾ������㴦���ļ��еĵ�ַ������fomerPchars���Ǹ��е��ڴ��ַ��lines_Start�� 0��ʼ��fomerPchars���ǵ�ַ����ʼ�� ��
				temp[19] = '\0';	//�涨��ʱ�ַ���������һ���ֽ���0��β ��Ҫ�ɲ�Ҫ
				
				double dd = atof(temp);// double dd=atof(temp); ���ַ������е��ַ���ת����doubleֵ
				m_Values.push_back(dd);	//�õ���ֵ���뵽�������������
				lines_Start = i + 2;//���������if��ʱ��i����ÿһ�н�β��'\r'������һ����ʼ��ַ���� i+2 ����lines_Start
			}
			pChars ++;//ѭ����
		}	
		// �ر��ļ����  
		fFile.Close();
		// �ͷŻ�������  
		strFile.ReleaseBuffer();
		// �򻷾�ΪUnicdoe��ԭ��CStringA������ֱ����ʾ������ؼ���  
		// ����ʹ��CStringת��ΪUnicdoe�ַ�  
		CString temp;
		bool is_continue_zero = false;
		int non_zero_index = 0;
		for (int i = 0; i < m_Values.size(); i++)		//�����������ĩβ�Ƿ����� ����βһֱ����Ļ���non_zero_index�ͻ᲻���� �Ӷ��õ������������С
		{
			
			is_continue_zero = m_Values[i]==0 ? true : false;
			if (!is_continue_zero)non_zero_index = i+1;
		}
		m_Values.resize(non_zero_index);

		for (int i = 0; i <m_Values.size(); i++) 
		{
			temp.Format(_T("%.7f"), m_Values[i]);
			m_list.AddString(temp);	//���뵽listControl������ʾ
		}

		temp.Format(_T("������%d"), m_Values.size());
		SetDlgItemText(IDC_STATIC_NOW, temp); 
		delete fomerPchars;
	}  	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CREADDlg::OnCbnSelchangeListcombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CREADDlg::OnStnClickedStaticNow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CREADDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_dt = m_Mass+m_Stiff;
	UpdateData(FALSE);// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CREADDlg::OnBnClickedCalculatebutton()
{
	double i,a;
	a = m_Values[0];
	for (i = 0; i <= m_Values.size() - 1; i++)
	{
		if (m_Values[i] > a)
		{
			a = m_Values[i];
		}//������ֵ
	}
	m_acc = a;
	UpdateData(FALSE);// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CREADDlg::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CREADDlg::OnStnClickedPicturestatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ���������루��ͼ��
}
