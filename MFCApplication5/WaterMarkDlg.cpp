// WaterMarkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "WaterMarkDlg.h"
#include "afxdialogex.h"


// WaterMarkDlg �Ի���

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


// WaterMarkDlg ��Ϣ�������


void WaterMarkDlg::OnBnClickedBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void WaterMarkDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void WaterMarkDlg::OnBnClickedVideoinfilebuttonWatermark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void WaterMarkDlg::OnBnClickedAudioinfilebuttonWatermark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void WaterMarkDlg::OnBnClickedOutputfilebuttonWatermark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL WaterMarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString encapsulationMessage(_T("        ����Ƶ����ˮӡ�����YUV�ļ��������Ҫ���룬�뽫�����YUV�ļ����б��������"
									"\r\n        ��Ƶ����Ϊ��Ƶ���ļ�����������Ƶ�����߷�װ��ʽ�ļ���"));
	m_encapsulationmessage_watermark.SetWindowTextA(encapsulationMessage);

	return TRUE;  // return TRUE unless you set the focus to a control

}
