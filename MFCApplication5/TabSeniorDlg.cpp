// TabSeniorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "TabSeniorDlg.h"
#include "afxdialogex.h"


// TabSeniorDlg �Ի���

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


// TabSeniorDlg ��Ϣ�������




void TabSeniorDlg::OnBnClickedInformation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MediaInformationDlg mediaInformationDlg;
	
	mediaInformationDlg.DoModal();
}


void TabSeniorDlg::OnBnClickedEncapsulation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EncapsulationDlg encapsulationDlg;

	encapsulationDlg.DoModal();
}



void TabSeniorDlg::OnBnClickedWatermark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WaterMarkDlg waterMarkDlg;

	waterMarkDlg.DoModal();
}
