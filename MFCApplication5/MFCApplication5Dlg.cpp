
// MFCApplication5Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "MFCApplication5Dlg.h"
#include "afxdialogex.h"



// CMFCApplication5Dlg �Ի���



CMFCApplication5Dlg::CMFCApplication5Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication5Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMFCApplication5Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication5Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication5Dlg::OnBnClickedCancel)
//	ON_COMMAND(IDCLOSE, &CMFCApplication5Dlg::OnIdclose)
ON_WM_CLOSE()
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMFCApplication5Dlg::OnSelchangeTab1)
ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication5Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication5Dlg ��Ϣ�������

BOOL CMFCApplication5Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_tab.InsertItem(0,"¼��");  //��Ӳ���һѡ�
	m_tab.InsertItem(1,"��Ƶ����");  //��Ӳ�����ѡ�
	m_tab.InsertItem(2,"�߼�����");    //��ӽ��ѡ�

	tab_RecordDlg.Create(Tab_Record,GetDlgItem(IDC_TAB1));
	tab_VedioDlg.Create(Tab_Vedio,GetDlgItem(IDC_TAB1));
	tab_SeniorDlg.Create(Tab_Senior,GetDlgItem(IDC_TAB1));

	//���IDC_TABTEST�ͻ�����С
	CRect rc;
	m_tab.GetClientRect(&rc);
    
	
	rc.top += 30;
	rc.bottom -= 8;
	rc.left += 8;
	rc.right -= 8;
	
	
	tab_RecordDlg.MoveWindow(&rc);
	tab_VedioDlg.MoveWindow(&rc);
	tab_SeniorDlg.MoveWindow(&rc);
	




	m_tab.SetCurSel(0);

	tab_RecordDlg.ShowWindow(true);
	tab_VedioDlg.ShowWindow(false);
	tab_SeniorDlg.ShowWindow(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication5Dlg::OnPaint()
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
HCURSOR CMFCApplication5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication5Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	

	//CDialogEx::OnOK();
}



void CMFCApplication5Dlg::OnBnClickedCancel()
{
	//thread_exit=1;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}




void CMFCApplication5Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//thread_exit=1;
	CDialogEx::OnClose();
}


void CMFCApplication5Dlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();

    switch(CurSel)
    {
    case 0:
        tab_RecordDlg.ShowWindow(true);
        tab_VedioDlg.ShowWindow(false);
		tab_SeniorDlg.ShowWindow(false);
        break;
    case 1:
        tab_RecordDlg.ShowWindow(false);
        tab_VedioDlg.ShowWindow(true);
		tab_SeniorDlg.ShowWindow(false);
        break;
	case 2:
		tab_RecordDlg.ShowWindow(false);
        tab_VedioDlg.ShowWindow(false);
		tab_SeniorDlg.ShowWindow(true);
        break;

    }
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMFCApplication5Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
