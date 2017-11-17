// WaterMarkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "WaterMarkDlg.h"
#include "afxdialogex.h"


// WaterMarkDlg 对话框

IMPLEMENT_DYNAMIC(WaterMarkDlg, CDialog)

WaterMarkDlg::WaterMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WaterMarkDlg::IDD, pParent)
{

}

WaterMarkDlg::~WaterMarkDlg()
{
}

void WaterMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VIDEOINFILE_WATERMARK, m_vedioinfile_watermark);
	DDX_Control(pDX, IDC_AUDIOINFILE_WATERMARK, m_audioinfile_watermark);
	DDX_Control(pDX, IDC_OUTPUTFILE_WATERMARK, m_outputfile_watermark);
	DDX_Control(pDX, IDC_ENCAPSULATION_MESSAGE_WATERMARK, m_encapsulationmessage_watermark);
}


BEGIN_MESSAGE_MAP(WaterMarkDlg, CDialog)
	ON_BN_CLICKED(ID_Begin, &WaterMarkDlg::OnBnClickedBegin)
	ON_BN_CLICKED(ID_CLOSE, &WaterMarkDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_VIDEOINFILEBUTTON_WATERMARK, &WaterMarkDlg::OnBnClickedVideoinfilebuttonWatermark)
	ON_BN_CLICKED(IDC_AUDIOINFILEBUTTON_WATERMARK, &WaterMarkDlg::OnBnClickedAudioinfilebuttonWatermark)
	ON_BN_CLICKED(IDC_OUTPUTFILEBUTTON_WATERMARK, &WaterMarkDlg::OnBnClickedOutputfilebuttonWatermark)
END_MESSAGE_MAP()


// WaterMarkDlg 消息处理程序


void WaterMarkDlg::OnBnClickedBegin()
{
	// TODO: 在此添加控件通知处理程序代码
}


void WaterMarkDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
}


void WaterMarkDlg::OnBnClickedVideoinfilebuttonWatermark()
{
	// TODO: 在此添加控件通知处理程序代码
}


void WaterMarkDlg::OnBnClickedAudioinfilebuttonWatermark()
{
	// TODO: 在此添加控件通知处理程序代码
}


void WaterMarkDlg::OnBnClickedOutputfilebuttonWatermark()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL WaterMarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString encapsulationMessage(_T("        将视频加上水印将输出YUV文件，如果需要编码，请将输出的YUV文件进行编码输出。"
									"\r\n        音频输入为音频流文件，不能是视频流或者封装格式文件。"));
	m_encapsulationmessage_watermark.SetWindowTextA(encapsulationMessage);

	return TRUE;  // return TRUE unless you set the focus to a control

}
