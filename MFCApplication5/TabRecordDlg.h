#pragma once
#include "afxwin.h"


// TabRecordDlg 对话框

class TabRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(TabRecordDlg)

public:
	TabRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TabRecordDlg();

// 对话框数据
	enum { IDD = Tab_Record };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CWinThread *pThreadPlay;
	CWinThread *pThreadSave;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEndrecord();
	afx_msg void OnBnClickedEncode();
	CEdit m_yuvoutfile;
	CButton m_savefile;
	CButton m_stoprecord;
	CButton m_beginrecord;

};
