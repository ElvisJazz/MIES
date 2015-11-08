///////////////////////////////////////////////////////////////////////
//�ļ����� AddItemDlg.cpp
//��  �ܣ� �����Ŀ��ͷ�ļ������ڴ�ӡ�Է�֪��ͬ���飩
//�����ߣ� ������
//��  ����2014/7/27
//��  �ģ�2014/7/27
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"

#include <map>
#include <string>
using namespace std;
// CAddItemDlg �Ի���

class CAddItemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddItemDlg)

public:
	CAddItemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddItemDlg();

// �Ի�������
	enum { IDD = IDD_ADD_ITEM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// Ҫ��ӵ���Ŀ
	pair<string, string> m_addItem;

public:
	// ��ʼ������
	void Init(CString patientId, CString visitId, string chargeType);
	// ��ȡҪ��ӵ���Ŀ,�������+��Ŀ��
	pair<string, string> GetAddItem() { return m_addItem; }
	// ��ȡ��Ŀ����
	int GetAmount() { return m_amount; }
	// ��ȡ��Ŀcode
	string GetItemCode() { return m_itemCode; }
	// ���ݿ��ѯ��Ŀ
	bool QueryItem();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEditchangeCombo1();
	// ѡ���ؼ�
	CComboBox m_comboCtr;
	afx_msg void OnEnChangeInputCode();
	afx_msg void OnBnClickedPrePageButton();
	afx_msg void OnBnClickedNextPageButton();
private:
	// ����ƴ��
	CString m_itemName;
	// ҳ�����
	int m_page;
	// �Ƿ񷭵�βҳ��
	bool m_isEnd;
	// �洢��Ŀ����Ӧ������
	multimap<string, string> m_itemClassMap;
	// �����Ŀ
	CString m_item;
	// ��׼�����
	string m_ybjzk;
	// ��׼�����������
	string m_jzkfycs;
	// �ѱ�
	string m_chargeType;
public:
	afx_msg void OnEnKillfocusEdit1();
private:
	// ����
	int m_amount;
	// item_code
	string m_itemCode;
};
