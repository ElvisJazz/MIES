///////////////////////////////////////////////////////////////////////
//�ļ����� PrintSelfConsentDlg.h
//��  �ܣ� ��ӡ�Է�֪��ͬ������ͷ�ļ�
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#pragma once
#include "afxcmn.h"
#include "atltime.h"
#include <map>
#include <vector>
#include "afxwin.h"

// CDisplayInfoDlg �Ի���

class CPrintSelfConsentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintSelfConsentDlg)

public:
	CPrintSelfConsentDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrintSelfConsentDlg();


// �Ի�������
	enum { IDD = IDD_SELF_CHARGE_CONSENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	// ����id
	CString m_patientId;
	// ��ѯ��Ŀ�б�
	CListCtrl m_list;	
	// ��ӡ��¼�洢����
	map<string, string> m_itemRecordMap;
	// ��Ŀ����������, item_code amount
	map<string, double> m_itemAmountMap;
	// �Ƿ���Բ�����ӡ��¼�洢������־λ
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

	// ��ʼ������ȡ����
	bool Init();
	
	afx_msg void OnEnChangeEdit1();

	// �Ѳ�ѯ��visit_id
	string m_queryVisitID;
	afx_msg void OnEnUpdateEdit1();

private:
	// �ѱ�
	string m_chargeType;
	// �Ƿ�ѡ��ҩƷ
	BOOL m_checkDrug;
	// �Ƿ�ѡ������
	BOOL m_checkCure;
	// �Ƿ�ѡ�м��
	BOOL m_checkInspect;
	// �Ƿ�ѡ��������Ŀ
	BOOL m_checkOther;
	// ��ʼʱ��
	CTime m_startDate;
	// ����ʱ��
	CTime m_endDate;
	// ��ʼĬ��û��ȫ��ѡ��
	BOOL m_isAllSelected;
	// �洢����ӡ����Ŀ���к���Ϊ����ÿ����¼�����͡���Ŀ���ơ���Ŀcode���Ʒ�����Ϊֵ
	map<int, pair<string, string> > m_itemMap;
	
	// �洢����ӡ�Ļ��߻�����Ϣ����, ����+����
	map<string, string> m_infoMap;
	// �����б���Ҫ��item_class��class_name�Ķ�Ӧ
	multimap<string, string> m_itemClassMap;
	// ��׼�����
	string m_ybjzk;
	// ��׼�����������
	string m_jzkfycs;
	// סԺ�����ؼ�
	CComboBox m_visitIdCtr;
	// visit_id
	CString m_visitId;
	// ��ӡ��Ŀ������У�δ��ӡ���Ѵ�ӡ��
	CString m_optType;

public:
	afx_msg void OnEnKillfocusEdit1();
};

