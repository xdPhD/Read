
// READDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CREADDlg 对话框



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


// CREADDlg 消息处理程序

BOOL CREADDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CREADDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CREADDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CREADDlg::OnBnClickedDrawbutton()
{
	UpdateData(TRUE);
	CWnd *picture = this->GetDlgItem(IDC_PictureSTATIC); //拿到该控件的窗kou指针
	CRect prect;			//
	picture->GetWindowRect(&prect); //拿到它的矩形区域（包括坐标和大小）
	ScreenToClient(&prect);	//映射到客户区坐标

	CDC *pdc= picture->GetDC();	//拿到该空间的DC	
	CBrush brush(RGB(255,0,0));// 建一个画刷
	pdc->SelectObject(&brush);//绑定该画刷
	pdc->MoveTo(CPoint(0, 0));//移动到原点  开始绘图
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
	// 设置过滤器   
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString strFilePath;
	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		int pos= strFilePath.ReverseFind('\\');//（查找最后一个\的位置）
		strFilePath=strFilePath.Left(pos);//将\左边的路径赋值到strFilePath
		SetDlgItemText(IDC_ContentEDIT, strFilePath);//将路径显示出来
	}  // TODO: 在此添加控件通知处理程序代码
}

#include "Math.h"
void CREADDlg::OnBnClickedDisplaybutton()
{
	CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4 | 2,
		_TEXT("文本文件(*.txt,*.ini,*.log)|*.txt;*.ini;*.log|全部文件(*.*)|*.*||"));
	// 调用 DoModal() 方法来打开对话框  
	if (fDlg.DoModal() == IDOK)
	{
		// 设置编辑框1的内容为选择文件的路径  
		// GetPathName() --> 返回选定文件的完整路径  
		SetDlgItemText(IDC_ContentEDIT, fDlg.GetPathName());
		CFile fFile(fDlg.GetPathName(), CFile::modeRead);
		// 因为在Windows下常用的文本保存编码为ANSI  
		// 而本工程在当初选择的时候选择了Unicode环境  
		// 所以需要使用CStringA类型的变量。  
		CStringA strFile;
		// 读取打开文件内容  
		// 第一个参数：读取内容的缓冲区域  
		// 第二个参数：读取内容的长度  
		int size_str = fFile.GetLength();//得到文件的长度
		char* pChars = new char[size_str];//建立一个该长度的字符数组
		char* fomerPchars = pChars;//pChars 会在后面更改，formerPchars做一个备份，以利于最后的删除该字符数组
		fFile.Read(pChars, size_str);  //将文件读到pChars数组里面
		int lines_Start = 0;
		m_Values.clear();				//清空用于 保存结果的double数组空间
		for (int i = 0; i < size_str; i++)		//开始一个一个字节的向文件末尾读
		{
		
			if (*pChars == '\r' && *(pChars+1)=='\n')  //如果读到的字符是转行和新行符（即完整的换行符），就表示读了一行
			{
				char temp[20] = {'0'};		//新建临时字符数组用来存储该行内容
				memcpy(temp, fomerPchars+lines_Start,i-lines_Start);	//存储该行内容，lines_Start表示该行起点处在文件中的地址，加上fomerPchars就是该行的内存地址（lines_Start从 0开始，fomerPchars才是地址的起始量 ）
				temp[19] = '\0';	//规定临时字符数组的最后一个字节是0结尾 可要可不要
				
				double dd = atof(temp);// double dd=atof(temp); 将字符数组中的字符串转换成double值
				m_Values.push_back(dd);	//得到的值加入到结果保存数组中
				lines_Start = i + 2;//当进入这个if的时候，i处在每一行结尾处'\r'处，下一行起始地址就是 i+2 赋给lines_Start
			}
			pChars ++;//循环啊
		}	
		// 关闭文件句柄  
		fFile.Close();
		// 释放缓冲区域  
		strFile.ReleaseBuffer();
		// 因环境为Unicdoe的原因，CStringA不可以直接显示到界面控件上  
		// 所以使用CString转换为Unicdoe字符  
		CString temp;
		bool is_continue_zero = false;
		int non_zero_index = 0;
		for (int i = 0; i < m_Values.size(); i++)		//看看结果数组末尾是否有零 ，结尾一直是零的话，non_zero_index就会不增加 从而得到真正的数组大小
		{
			
			is_continue_zero = m_Values[i]==0 ? true : false;
			if (!is_continue_zero)non_zero_index = i+1;
		}
		m_Values.resize(non_zero_index);

		for (int i = 0; i <m_Values.size(); i++) 
		{
			temp.Format(_T("%.7f"), m_Values[i]);
			m_list.AddString(temp);	//加入到listControl里面显示
		}

		temp.Format(_T("个数：%d"), m_Values.size());
		SetDlgItemText(IDC_STATIC_NOW, temp); 
		delete fomerPchars;
	}  	// TODO: 在此添加控件通知处理程序代码
}


void CREADDlg::OnCbnSelchangeListcombo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CREADDlg::OnStnClickedStaticNow()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CREADDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_dt = m_Mass+m_Stiff;
	UpdateData(FALSE);// TODO: 在此添加控件通知处理程序代码
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
		}//求解最大值
	}
	m_acc = a;
	UpdateData(FALSE);// TODO: 在此添加控件通知处理程序代码
}


void CREADDlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CREADDlg::OnStnClickedPicturestatic()
{
	// TODO: 在此添加控件通知处理程序代码（画图）
}
