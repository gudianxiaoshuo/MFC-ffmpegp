#pragma once
#include "afxwin.h"


// TabRecordDlg �Ի���

class TabRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(TabRecordDlg)

public:
	TabRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TabRecordDlg();

// �Ի�������
	enum { IDD = Tab_Record };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
