///////////////////////////////////////////////////////////////////////
//文件名： AddItemDlg.cpp
//功  能： 添加项目类头文件（用于打印自费知情同意书）
//开发者： 贾治中
//创  建：2014/7/27
//修  改：2014/7/27
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"

#include <map>
#include <string>
using namespace std;
// CAddItemDlg 对话框

class CAddItemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddItemDlg)

public:
	CAddItemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddItemDlg();

// 对话框数据
	enum { IDD = IDD_ADD_ITEM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 要添加的项目
	pair<string, string> m_addItem;

public:
	// 初始化函数
	void Init(CString patientId, CString visitId, string chargeType);
	// 获取要添加的项目,返回类别+项目名
	pair<string, string> GetAddItem() { return m_addItem; }
	// 获取项目数量
	int GetAmount() { return m_amount; }
	// 获取项目code
	string GetItemCode() { return m_itemCode; }
	// 数据库查询项目
	bool QueryItem();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEditchangeCombo1();
	// 选择框控件
	CComboBox m_comboCtr;
	afx_msg void OnEnChangeInputCode();
	afx_msg void OnBnClickedPrePageButton();
	afx_msg void OnBnClickedNextPageButton();
private:
	// 输入拼音
	CString m_itemName;
	// 页码变量
	int m_page;
	// 是否翻到尾页了
	bool m_isEnd;
	// 存储项目名对应类别变量
	multimap<string, string> m_itemClassMap;
	// 添加项目
	CString m_item;
	// 基准库参数
	string m_ybjzk;
	// 基准库费用类别参数
	string m_jzkfycs;
	// 费别
	string m_chargeType;
public:
	afx_msg void OnEnKillfocusEdit1();
private:
	// 数量
	int m_amount;
	// item_code
	string m_itemCode;
};
