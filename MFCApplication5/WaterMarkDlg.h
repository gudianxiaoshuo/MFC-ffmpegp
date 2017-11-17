#pragma once


// WaterMarkDlg 对话框

class WaterMarkDlg : public CDialog
{
	DECLARE_DYNAMIC(WaterMarkDlg)

public:
	WaterMarkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WaterMarkDlg();

// 对话框数据
	enum { IDD = IDD_WATERMARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CEdit m_vedioinfile_watermark;
	CEdit m_audioinfile_watermark;
	CEdit m_outputfile_watermark;
	CEdit m_encapsulationmessage_watermark;

	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedVideoinfilebuttonWatermark();
	afx_msg void OnBnClickedAudioinfilebuttonWatermark();
	afx_msg void OnBnClickedOutputfilebuttonWatermark();
	virtual BOOL OnInitDialog();
};
