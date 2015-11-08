#pragma once
#include "afxcmn.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

// CDailyListDlg �Ի���

class CDailyListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDailyListDlg)

public:
	CDailyListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDailyListDlg();

// �Ի�������
	enum { IDD = IDD_DAILY_LIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����id
	CString m_patientId;
	// ÿ���嵥��ӡ��Ŀ�б�
	CListCtrl m_list;

	afx_msg void OnBnClickedQueryPatientInfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

private:
	// �洢����ӡ����Ŀ���к���Ϊ��������+��Ŀ����Ϊֵ
	map<string, string> m_infoMap;
	// �洢����ӡ�Ļ��߻�����Ϣ����, ����+����
	map<string, vector<string> > m_itemMap;
};
