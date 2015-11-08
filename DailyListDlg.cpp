// DailyListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MIES.h"
#include "DailyListDlg.h"
#include "MyPrinter.h"
#include "afxdialogex.h"


// CDailyListDlg �Ի���

IMPLEMENT_DYNAMIC(CDailyListDlg, CDialogEx)

CDailyListDlg::CDailyListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDailyListDlg::IDD, pParent)
	, m_patientId(_T(""))
{

}

CDailyListDlg::~CDailyListDlg()
{
}

void CDailyListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_patientId);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDailyListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_QUERY_PATIENT_INFO, &CDailyListDlg::OnBnClickedQueryPatientInfo)
	ON_BN_CLICKED(IDOK, &CDailyListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDailyListDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDailyListDlg ��Ϣ�������

BOOL CDailyListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �����б���
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT); 
	// �����б����
	m_list.InsertColumn(0, _T("���"), LVCFMT_CENTER, 120, 60);
	m_list.InsertColumn(1, _T("����/���"), LVCFMT_CENTER, 150, 60);
	m_list.InsertColumn(2, _T("����"), LVCFMT_CENTER, 400, 60);
	m_list.InsertColumn(3, _T("����"), LVCFMT_CENTER, 60, 60);
	m_list.InsertColumn(4, _T("����"), LVCFMT_CENTER, 80, 60);
	m_list.InsertColumn(5, _T("���"), LVCFMT_CENTER, 80, 60);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDailyListDlg::OnBnClickedQueryPatientInfo()
{
	UpdateData(true);
	// ����id��ѡ�е�ѡ��������ݿ��ѯ����ȡ����ӡ��Ŀ
	// ���id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("����id����Ϊ�գ�"));
		return;
	}
	// ����id��ѯ������Ϣ
	GetDlgItem(IDC_NAME)->SetWindowTextW(_T("������"));	
	GetDlgItem(IDC_BED_NUM)->SetWindowTextW(_T("6"));	
	GetDlgItem(IDC_START_DATE)->SetWindowTextW(_T("2014/07/18"));
	GetDlgItem(IDC_CHARGE)->SetWindowTextW(_T("1700.37"));
	GetDlgItem(IDC_DEPARTMENT2)->SetWindowTextW(_T("�ۿ�"));
	GetDlgItem(IDC_HOSPITAL_DAYS)->SetWindowTextW(_T("5��"));

	// ��������Ϣ���뵽������
	m_infoMap.clear();
	m_infoMap.insert(make_pair<string, string>("name", "������"));
	m_infoMap.insert(make_pair<string, string>("bedNum", "6"));
	m_infoMap.insert(make_pair<string, string>("startDate", "2014/07/18"));
	m_infoMap.insert(make_pair<string, string>("department", "�ۿ�"));
	m_infoMap.insert(make_pair<string, string>("id", "30323590"));
	m_infoMap.insert(make_pair<string, string>("hospitalDays", "5"));
	m_infoMap.insert(make_pair<string, string>("allCharge", "1700.37"));
	m_infoMap.insert(make_pair<string, string>("allDayCharge", "1700.37"));
	m_infoMap.insert(make_pair<string, string>("printer", "����"));
	m_infoMap.insert(make_pair<string, string>("chargeDate", "2014/07/18"));
	m_infoMap.insert(make_pair<string, string>("printDate", "2014/08/01"));

	// ��ʾ��Ŀ
	m_list.DeleteAllItems();
	m_list.InsertItem(0,  _T("����"));   
    m_list.SetItemText(0, 1, _T("XYZ02GNAI2\n�Ȼ���100g"));   
    m_list.SetItemText(0, 2, _T("�Ȼ���0"));
	m_list.SetItemText(0, 3, _T("1.00"));
	m_list.SetItemText(0, 4, _T("6.00"));
	m_list.SetItemText(0, 5, _T("6.00"));

	m_list.InsertItem(1,  _T("����"));   
    m_list.SetItemText(1, 1, _T("XYZ02GNAI2\n�Ȼ���100g"));   
    m_list.SetItemText(1, 2, _T("�Ȼ���1"));
	m_list.SetItemText(1, 3, _T("1.00"));
	m_list.SetItemText(1, 4, _T("6.00"));
	m_list.SetItemText(1, 5, _T("1.00"));

	m_list.InsertItem(2,  _T("���ɱ���"));   
    m_list.SetItemText(2, 1, _T("XYZ02GNAI2(�Ȼ���100g)"));   
    m_list.SetItemText(2, 2, _T("�Ȼ���2"));
	m_list.SetItemText(2, 3, _T("1.00"));
	m_list.SetItemText(2, 4, _T("6.00"));
	m_list.SetItemText(2, 5, _T("6.00"));

	m_list.InsertItem(3,  _T("ҩƷ��С��"));   
    m_list.SetItemText(3, 1, _T("1001.37")); 

	// ����ѯ������Ŀ����������
	m_itemMap.clear();
	vector<string> tmpVec;
	tmpVec.push_back("����##XYZ02GNAI2##�Ȼ���0##�Ȼ���100g##1.00##6.00##6.00##");
	pair<string, vector<string> > pairVal0 = make_pair<string, vector<string> >("ҩƷ:1001.37", tmpVec);
	pair<string, vector<string> > pairVal1 = make_pair<string, vector<string> >("����:1001.37", tmpVec);
	pair<string, vector<string> > pairVal2 = make_pair<string, vector<string> >("���:1001.37", tmpVec);
	m_itemMap.insert(pairVal0);
	m_itemMap.insert(pairVal1);
	m_itemMap.insert(pairVal2);

	UpdateData(false);
}


void CDailyListDlg::OnBnClickedOk()
{
	UpdateData(true);
	// ����id��ѡ�е�ѡ��������ݿ��ѯ����ȡ����ӡ��Ŀ
	// ���id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("����id����Ϊ�գ�"));
		return;
	}

	// ��ȡ��ӡ��ӿڽ��д�ӡ����
	/*try
	{
		CMyPrinter printer;
		printer.ReadHtmlTemplate(PRINT_DAILY_LIST);
		printer.ProducePrintContent(PRINT_DAILY_LIST, m_infoMap, m_itemMap);
		printer.ProduceHtmlFile();
	}
	catch(runtime_error err)
	{
		AfxMessageBox(CString(err.what()));
	}*/
	CDialogEx::OnOK();
}


void CDailyListDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


