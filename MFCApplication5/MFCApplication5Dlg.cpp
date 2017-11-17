
// MFCApplication5Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "MFCApplication5Dlg.h"
#include "afxdialogex.h"



// CMFCApplication5Dlg 对话框



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


// CMFCApplication5Dlg 消息处理程序

BOOL CMFCApplication5Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	m_tab.InsertItem(0,"录制");  //添加参数一选项卡
	m_tab.InsertItem(1,"视频处理");  //添加参数二选项卡
	m_tab.InsertItem(2,"高级处理");    //添加结果选项卡

	tab_RecordDlg.Create(Tab_Record,GetDlgItem(IDC_TAB1));
	tab_VedioDlg.Create(Tab_Vedio,GetDlgItem(IDC_TAB1));
	tab_SeniorDlg.Create(Tab_Senior,GetDlgItem(IDC_TAB1));

	//获得IDC_TABTEST客户区大小
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication5Dlg::OnPaint()
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
HCURSOR CMFCApplication5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication5Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	

	

	//CDialogEx::OnOK();
}



void CMFCApplication5Dlg::OnBnClickedCancel()
{
	//thread_exit=1;
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}




void CMFCApplication5Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCApplication5Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
