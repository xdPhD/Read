
// READDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>

// CREADDlg �Ի���
class CREADDlg : public CDialogEx
{
// ����
public:
	CREADDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_READ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	std::vector<double> m_Values;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double m_Mass;
	double m_Stiff;
	double m_Damp;
	afx_msg void OnBnClickedDrawbutton();
	afx_msg void OnBnClickedContentbutton();
	CComboBox m_filenames;
	afx_msg void OnBnClickedDisplaybutton();
	CListBox m_list;
	afx_msg void OnCbnSelchangeListcombo();
	double m_dt;
	double m_pga;
	afx_msg void OnStnClickedStaticNow();
	double m_acc;
	double m_velo;
	double m_disp;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCalculatebutton();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnStnClickedPicturestatic();
};
