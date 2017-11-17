#pragma once
#include "afxwin.h"


// EncapsulationDlg 对话框

class EncapsulationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EncapsulationDlg)

public:
	EncapsulationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EncapsulationDlg();

// 对话框数据
	enum { IDD = IDD_ENCAPSULATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CWinThread *pThreadEncapsulate;

	DECLARE_MESSAGE_MAP()
public:

	void SetInputVideoPath(CString inputpath);
	void SetInputAudioPath(CString inputpath);
	void SetOutputMediaPath(CString outputpath);

	bool CheckEncapsulationMessage();

	CEdit m_vedioinfile;
	CEdit m_audioinfile;
	CEdit m_outputfile;
	CEdit m_encapsulationmessage;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedVideoinfilebutton();
	afx_msg void OnBnClickedAudioinfilebutton();
	afx_msg void OnBnClickedOutputfilebutton();
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedClose();
};
