#pragma once

#include "MediaInformationDlg.h"
#include "EncapsulationDlg.h"
#include "WaterMarkDlg.h"
// TabSeniorDlg �Ի���

class TabSeniorDlg : public CDialog
{
	DECLARE_DYNAMIC(TabSeniorDlg)

public:
	TabSeniorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TabSeniorDlg();

// �Ի�������
	enum { IDD = Tab_Senior };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedInformation();
	afx_msg void OnBnClickedEncapsulation();
	afx_msg void OnBnClickedWatermark();
};
