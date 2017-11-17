// TabSeniorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "TabSeniorDlg.h"
#include "afxdialogex.h"


// TabSeniorDlg 对话框

IMPLEMENT_DYNAMIC(TabSeniorDlg, CDialog)

TabSeniorDlg::TabSeniorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TabSeniorDlg::IDD, pParent)
{

}

TabSeniorDlg::~TabSeniorDlg()
{
}

void TabSeniorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TabSeniorDlg, CDialog)
	ON_BN_CLICKED(IDC_INFORMATION, &TabSeniorDlg::OnBnClickedInformation)
	ON_BN_CLICKED(IDC_ENCAPSULATION, &TabSeniorDlg::OnBnClickedEncapsulation)
	ON_BN_CLICKED(IDC_WATERMARK, &TabSeniorDlg::OnBnClickedWatermark)
END_MESSAGE_MAP()


// TabSeniorDlg 消息处理程序




void TabSeniorDlg::OnBnClickedInformation()
{
	// TODO: 在此添加控件通知处理程序代码
	MediaInformationDlg mediaInformationDlg;
	
	mediaInformationDlg.DoModal();
}


void TabSeniorDlg::OnBnClickedEncapsulation()
{
	// TODO: 在此添加控件通知处理程序代码
	EncapsulationDlg encapsulationDlg;

	encapsulationDlg.DoModal();
}



void TabSeniorDlg::OnBnClickedWatermark()
{
	// TODO: 在此添加控件通知处理程序代码
	WaterMarkDlg waterMarkDlg;

	waterMarkDlg.DoModal();
}
