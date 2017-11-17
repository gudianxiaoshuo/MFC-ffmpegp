
// MFCApplication5Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "TabRecordDlg.h"
#include "TabVedioDlg.h"
#include "TabSeniorDlg.h"


// CMFCApplication5Dlg �Ի���
class CMFCApplication5Dlg : public CDialogEx
{
// ����
public:
	CMFCApplication5Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATION5_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
