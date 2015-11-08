///////////////////////////////////////////////////////////////////////
//文件名： PrintSelfConsentDlg.h
//功  能： 打印自费知情同意书类头文件
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#pragma once
#include "afxcmn.h"
#include "atltime.h"
#include <map>
#include <vector>
#include "afxwin.h"

// CDisplayInfoDlg 对话框

class CPrintSelfConsentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintSelfConsentDlg)

public:
	CPrintSelfConsentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrintSelfConsentDlg();


// 对话框数据
	enum { IDD = IDD_SELF_CHARGE_CONSENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	// 患者id
	CString m_patientId;
	// 查询项目列表
	CListCtrl m_list;	
	// 打印记录存储容器
	map<string, string> m_itemRecordMap;
	// 项目数量计数器, item_code amount
	map<string, double> m_itemAmountMap;
	// 是否可以操作打印记录存储容器标志位
	bool m_isAllowOpt;

	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedCheckDrug();
	afx_msg void OnBnClickedCheckCure();
	afx_msg void OnBnClickedCheckInspect();
	afx_msg void OnBnClickedCheckOther();
	afx_msg void OnBnClickedQueryItem();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnBnClickedQueryPatientInfo();

	afx_msg void OnColumnclickItemList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddItemButton();
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);

	// 初始化，获取参数
	bool Init();
	
	afx_msg void OnEnChangeEdit1();

	// 已查询的visit_id
	string m_queryVisitID;
	afx_msg void OnEnUpdateEdit1();

private:
	// 费别
	string m_chargeType;
	// 是否选中药品
	BOOL m_checkDrug;
	// 是否选中治疗
	BOOL m_checkCure;
	// 是否选中检查
	BOOL m_checkInspect;
	// 是否选中其他项目
	BOOL m_checkOther;
	// 开始时间
	CTime m_startDate;
	// 结束时间
	CTime m_endDate;
	// 初始默认没有全部选中
	BOOL m_isAllSelected;
	// 存储待打印的项目，行号作为键，每条记录：类型、项目名称、项目code、计费日期为值
	map<int, pair<string, string> > m_itemMap;
	
	// 存储待打印的患者基本信息容器, 标题+内容
	map<string, string> m_infoMap;
	// 参数列表，主要是item_class与class_name的对应
	multimap<string, string> m_itemClassMap;
	// 基准库参数
	string m_ybjzk;
	// 基准库费用类别参数
	string m_jzkfycs;
	// 住院次数控件
	CComboBox m_visitIdCtr;
	// visit_id
	CString m_visitId;
	// 打印项目类别（所有，未打印，已打印）
	CString m_optType;

public:
	afx_msg void OnEnKillfocusEdit1();
};

