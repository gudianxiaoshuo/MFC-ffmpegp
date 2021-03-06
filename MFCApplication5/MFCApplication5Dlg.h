
// MFCApplication5Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "TabRecordDlg.h"
#include "TabVedioDlg.h"
#include "TabSeniorDlg.h"


// CMFCApplication5Dlg 对话框
class CMFCApplication5Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication5Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION5_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnClose();
	

	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

public :
	CTabCtrl m_tab;
	TabRecordDlg tab_RecordDlg;
	TabVedioDlg tab_VedioDlg;
	TabSeniorDlg tab_SeniorDlg;
	afx_msg void OnBnClickedButton1();
};
