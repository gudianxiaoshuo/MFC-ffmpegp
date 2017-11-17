#pragma once

#include "MediaInformationDlg.h"
#include "EncapsulationDlg.h"
#include "WaterMarkDlg.h"
// TabSeniorDlg 对话框

class TabSeniorDlg : public CDialog
{
	DECLARE_DYNAMIC(TabSeniorDlg)

public:
	TabSeniorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TabSeniorDlg();

// 对话框数据
	enum { IDD = Tab_Senior };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedInformation();
	afx_msg void OnBnClickedEncapsulation();
	afx_msg void OnBnClickedWatermark();
};
