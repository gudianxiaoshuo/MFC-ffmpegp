#pragma once
#include "afxwin.h"


// MediaInformationDlg �Ի���

class MediaInformationDlg : public CDialog
{
	DECLARE_DYNAMIC(MediaInformationDlg)

public:
	MediaInformationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MediaInformationDlg();

// �Ի�������
	enum { IDD = IDD_MEDIAINFORMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetInputPath(CString inputurl);
	void SetInformation(CString filePath);

	afx_msg void OnBnClickedTaskinfoInfileFdlg();
	CEdit m_formatinputformat;
	CEdit m_formatduration;
	CEdit m_formatbitrate;
	CEdit m_codecvframerate;
	CEdit m_codecvname;
	CEdit m_codecvresolution;
	CEdit m_codecachannels;
	CEdit m_codecaname;
	CEdit m_codecasamplerate;
	CEdit m_infoinfile;
	afx_msg void OnBnClickedOk();
};
