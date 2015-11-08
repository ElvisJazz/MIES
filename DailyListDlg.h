#pragma once
#include "afxcmn.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

// CDailyListDlg 对话框

class CDailyListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDailyListDlg)

public:
	CDailyListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDailyListDlg();

// 对话框数据
	enum { IDD = IDD_DAILY_LIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 患者id
	CString m_patientId;
	// 每日清单打印项目列表
	CListCtrl m_list;

	afx_msg void OnBnClickedQueryPatientInfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

private:
	// 存储待打印的项目，行号作为键，类型+项目名称为值
	map<string, string> m_infoMap;
	// 存储待打印的患者基本信息容器, 标题+内容
	map<string, vector<string> > m_itemMap;
};
