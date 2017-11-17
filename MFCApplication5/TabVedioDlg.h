#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// TabVedioDlg �Ի���



class TabVedioDlg : public CDialog
{
	DECLARE_DYNAMIC(TabVedioDlg)

public:
	TabVedioDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TabVedioDlg();

// �Ի�������
	enum { IDD = Tab_Vedio };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CWinThread *pThreadVedioPlay;
public:

	CString URLtoFilename(CString url);
	CString bytetoKBMBGB(int filesize);

	afx_msg void OnBnClickedFilebrowser();
	afx_msg void OnBnClickedCancel();
	CEdit m_url;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnStnClickedScreenVedio();
};
