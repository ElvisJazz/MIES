///////////////////////////////////////////////////////////////////////
//�ļ����� PrintSelfConsentDlg.cpp
//��  �ܣ� ��ӡ�Է�֪��ͬ������ʵ��
//�����ߣ� ������
//��  ����2014/7/24
//��  �ģ�2014/7/27
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIES.h"
#include "PrintSelfConsentDlg.h"
#include "AddItemDlg.h"
#include "MyPrinter.h"
#include "sql_dao.h"	// CSqlDao��ͷ�ļ�
#include "afxdialogex.h"

using namespace std;

// ���������ȫ�ֱ���
// �������
int g_sortColumn;
// ���򷽷���˳������
bool g_method[3];

// д�����ݿ��Ѵ�ӡ����
UINT WriteInfoDB(LPVOID lpParam)
{
	if(lpParam != NULL)
    {
		CPrintSelfConsentDlg* pDlg = (CPrintSelfConsentDlg*)lpParam;
		pDlg->m_isAllowOpt = false;
		map<string,string>::iterator iter = (pDlg->m_itemRecordMap).begin();
		// ��ȡ��ǰʱ��
		CTime currentTime = CTime::GetCurrentTime();
		CString szTime = currentTime.Format("%Y-%m-%d %H:%M:%S");
		char printDate[0x100];
		char patientId[0x100];
		::wsprintfA(printDate, "%ls", (LPCTSTR)szTime);
		::wsprintfA(patientId, "%ls", (LPCTSTR)pDlg->m_patientId);

		// ��ȡ���洢��������
		
		string itemName="", itemCode="", itemClass="", billingDate="", units = "", price = "", itemType = "", recordPrintDate = "", itemNo = "";
		
		int i = 0;
		char subStr[8];

		for(; iter!=(pDlg->m_itemRecordMap).end(); ++iter)
		{
			i = 1;
			string::size_type index0 = 0;
			string::size_type index = (iter->second).find("#1#");
			while(index != string::npos)
			{
				switch(i)
				{
				case 1: billingDate = iter->second.substr(index0, index-index0); break;
				case 2: itemName = iter->second.substr(index0, index-index0); break;
				case 3: itemCode = iter->second.substr(index0, index-index0); break;
				case 4: itemClass = iter->second.substr(index0, index-index0); break;
				case 5: recordPrintDate = iter->second.substr(index0, index-index0); break;
				case 6: units = iter->second.substr(index0, index-index0); break;
				case 7: price = iter->second.substr(index0, index-index0); break;
				case 9: itemType = iter->second.substr(index0, index-index0); break;
				case 11: itemNo = iter->second.substr(index0, index-index0); break;
				default:break;
				}
				i++;
				index0 = index + 3;
				if(i > 10)
					index0++;
				sprintf(subStr, "#%d#", i);
				index = iter->second.find(string(subStr), index0);
			}
			// sql���
			map<string, double>::iterator iter0 = pDlg->m_itemAmountMap.find(itemCode+","+itemNo);
			double amount = 0;
			char amountStr[8];
			if(iter0 != pDlg->m_itemAmountMap.end())
				amount = iter0->second;
			sprintf(amountStr, "%.2f", amount);

			string sql = "";

			// ���ݿ����Ѽ�¼�������
			if(recordPrintDate != "")
			{
				sql = "UPDATE DWYL.PRINT_ITEM_RECORD SET PRINT_DATE=to_date('"+string(printDate)+"','yyyy-mm-dd hh24:mi:ss') WHERE PATIENT_ID='"+string(patientId)+"' AND VISIT_ID='"+pDlg->m_queryVisitID+
					"' AND ITEM_CODE='"+itemCode+"' AND BILLING_DATE_TIME=to_date('"+billingDate+"','yyyy-mm-dd hh24:mi:ss')"+" AND ITEM_NO='"+itemNo+"'";
			}
			else // ����
			{
				if(billingDate != "")
					sql = "INSERT INTO DWYL.PRINT_ITEM_RECORD VALUES('"+string(patientId)+"','"+pDlg->m_queryVisitID+
					"','"+itemCode+"',to_date('"+billingDate+"','yyyy-mm-dd hh24:mi:ss'),to_date('"+string(printDate)+"','yyyy-mm-dd hh24:mi:ss'),'"+itemClass+"','"+itemName+"','"+amountStr+"','"+price+"','"+itemType+"','"+units+"','"+itemNo+"')";
				else
					sql = "INSERT INTO DWYL.PRINT_ITEM_RECORD VALUES('"+string(patientId)+"','"+pDlg->m_queryVisitID+
					"','"+itemCode+"',null,to_date('"+string(printDate)+"','yyyy-mm-dd hh24:mi:ss'),'"+itemClass+"','"+itemName+"','"+amountStr+"','"+price+"','"+itemType+"','"+units+"','"+itemNo+"')";
			}
			// ִ�в������
			CDB db;
			try
			{
				db.Init("MIES_db_config.conf");
				db.Submit(sql);
				db.Close();
			}
			catch( runtime_error err )
			{
				// �쳣�������
				AfxMessageBox(CString(err.what()));
				// �رն���
				db.Close();
				return 0;
			}
		}
		
		(pDlg->m_itemRecordMap).clear();		
		pDlg->m_isAllowOpt = true;
	}
    return 0;
}

// CPrintSelfConsentDlg �Ի���

IMPLEMENT_DYNAMIC(CPrintSelfConsentDlg, CDialogEx)

CPrintSelfConsentDlg::CPrintSelfConsentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrintSelfConsentDlg::IDD, pParent)
	, m_patientId(_T(""))
	, m_checkDrug(FALSE)
	, m_checkInspect(FALSE)
	, m_checkOther(FALSE)
	, m_startDate(CTime::GetCurrentTime())
	, m_endDate(CTime::GetCurrentTime())
	, m_isAllSelected(FALSE)
	, m_visitId(_T(""))
	, m_optType(_T(""))
	, m_isAllowOpt(true)
{
	for(int i=0; i<3; i++)
		g_method[i] = false;
}

CPrintSelfConsentDlg::~CPrintSelfConsentDlg()
{
	while(!m_isAllowOpt);
}

void CPrintSelfConsentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_patientId);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Check(pDX, IDC_CHECK_DRUG, m_checkDrug);
	DDX_Check(pDX, IDC_CHECK_CURE, m_checkCure);
	DDX_Check(pDX, IDC_CHECK_INSPECT, m_checkInspect);
	DDX_Check(pDX, IDC_CHECK_OTHER, m_checkOther);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_startDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_endDate);
	DDX_Control(pDX, IDC_COMBO2, m_visitIdCtr);
	DDX_CBString(pDX, IDC_COMBO2, m_visitId);
	DDX_CBString(pDX, IDC_OPT_TYPE_COMBO, m_optType);
}

BOOL CPrintSelfConsentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// �����øþ�̬�ı�Ƥ��
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC1)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC2)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC3)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC4)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC5)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC6)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC7)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC8)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC9)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC10)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC11)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC12)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC13)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC14)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_STATIC15)->m_hWnd);

	skinppSetNoSkinHwnd(GetDlgItem(IDC_NAME)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_ID_NUM)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_VISIT_ID)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_SEX)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_DEPARTMENT)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_START_IN_HOSPITAL)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_END_IN_HOSPITAL)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_CHARGETYPE)->m_hWnd);
	//skinppSetNoSkinHwnd(GetDlgItem(IDC_DIAGNOSE)->m_hWnd);


	skinppSetNoSkinHwnd(GetDlgItem(IDC_CHECK_ALL)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_CHECK_DRUG)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_CHECK_CURE)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_CHECK_INSPECT)->m_hWnd);
	skinppSetNoSkinHwnd(GetDlgItem(IDC_CHECK_OTHER)->m_hWnd);

	// �����б���
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT | LVS_EX_CHECKBOXES); 
	// �����б����
	m_list.InsertColumn(0, _T("ȫѡ"), LVCFMT_CENTER, 50, 60);
	m_list.InsertColumn(1, _T("��ӡ��"), LVCFMT_CENTER, 50, 60);
	m_list.InsertColumn(2, _T("����"), LVCFMT_CENTER, 180, 60);
	m_list.InsertColumn(3, _T("��Ŀ"), LVCFMT_LEFT, 540, 60);
	m_list.InsertColumn(4, _T("��ӡ����"), LVCFMT_CENTER, 180, 60);

	// Ĭ�����ô�ӡѡ��Ϊδ��ӡ
	CComboBox* printCtr = (CComboBox*)GetDlgItem(IDC_OPT_TYPE_COMBO);
	printCtr->SetCurSel(1);
	if(m_patientId != _T(""))
		OnBnClickedQueryPatientInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CPrintSelfConsentDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CPrintSelfConsentDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_DRUG, &CPrintSelfConsentDlg::OnBnClickedCheckDrug)
	ON_BN_CLICKED(IDC_CHECK_INSPECT, &CPrintSelfConsentDlg::OnBnClickedCheckInspect)
	ON_BN_CLICKED(IDC_CHECK_OTHER, &CPrintSelfConsentDlg::OnBnClickedCheckOther)
	ON_BN_CLICKED(IDC_QUERY_ITEM, &CPrintSelfConsentDlg::OnBnClickedQueryItem)
	ON_BN_CLICKED(IDC_PRINT, &CPrintSelfConsentDlg::OnBnClickedPrint)
	ON_BN_CLICKED(IDC_QUERY_PATIENT_INFO, &CPrintSelfConsentDlg::OnBnClickedQueryPatientInfo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CPrintSelfConsentDlg::OnColumnclickItemList)
	ON_BN_CLICKED(IDC_ADD_ITEM_BUTTON, &CPrintSelfConsentDlg::OnBnClickedAddItemButton)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CPrintSelfConsentDlg::OnDtnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CPrintSelfConsentDlg::OnDtnDatetimechangeDatetimepicker2)
	ON_BN_CLICKED(IDC_CHECK_CURE, &CPrintSelfConsentDlg::OnBnClickedCheckCure)
	ON_EN_CHANGE(IDC_EDIT1, &CPrintSelfConsentDlg::OnEnChangeEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CPrintSelfConsentDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


bool CPrintSelfConsentDlg::Init()
{
	m_itemMap.clear();
	m_itemAmountMap.clear();
	//m_list.DeleteAllItems();

	if(m_patientId != _T(""))
	{
		RESULT_MAP dbParameter;
		// ��ʼ�������б�
		map<string,string> param;
		char patientIdStr[256];
		::wsprintfA(patientIdStr, "%ls", (LPCTSTR)m_patientId);
		param.insert(pair<string, string>("PATIENT_ID", string(patientIdStr)));
		// ��ӡ��Ŀ����
		string parameterStr = "";
		// ��׼��
		string ybjzk = "";
		// ��׼���������
		string jzkfycs = "";
		// �����������ַ���
		string chargeStr;
		// ������ѯ����
		CSqlDao dao;
		try
		{
			// ��ʼ����ѯ����
			dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
			// ִ�в�ѯ
		
			// ���Ҳ�����
			if(m_visitId == _T("")) 
				dbParameter = dao.Query("ZFZQTYSParameter_NO_VISITID", param);
			else
			{
				char visitIdStr[256];
				::wsprintfA(visitIdStr, "%ls", (LPCTSTR)m_visitId);
				param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
				dbParameter = dao.Query("ZFZQTYSParameter_VISITID", param);
			}

			// �������������������ò���ֵ
			RESULT_MAP::iterator iter = dbParameter.find("parameter");
			while(iter != dbParameter.end())
			{
				RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
				for(;vecVecIter != iter->second.end(); ++vecVecIter)
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						// ��ȡ�Է�֪��ͬ����Ĳ���ֵ
						if(vecIter->first == "NAME" && vecIter->second == "ZFZQTYS")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									parameterStr = vecIter->second;
							}
						}
						// ��ȡ��Ӧ��׼��Ĳ���ֵ
						else if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									ybjzk = vecIter->second;
							}
						}
						// ��ȡ����������
						else if(vecIter->first == "NAME" && vecIter->second == "JZKFYCS")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									jzkfycs = vecIter->second.c_str();
							}
						}
					}
				}
				++iter;
			}
		
		}
		catch( runtime_error err )
		{
			// �쳣�������
			AfxMessageBox(CString(err.what()));
			// �رն���
			dao.Close();
			return false;
		}
		// �رն���
		dao.Close();

		// ������ӡ��Ŀ����,�洢������������
		m_itemClassMap.clear();
		string::size_type index1 = 0, index2 = 0, index = 0;
		while(index2 < parameterStr.length()-1)
		{
			if(index2 == 0)
				index1 = index2;
			else
				index1 = index2 + 1;

			index2 = parameterStr.find(";", index1);

			if(index2 == string::npos)
				break;
			// ��ȡ��
			index = parameterStr.find(":", index1);
			if(index == string::npos)
				break;
			string name = parameterStr.substr(index1, index-index1);
			// �����ȡֵ
			string value = "";
			index1 = index + 1;
			while(index < index2)
			{
				index = parameterStr.find(",", index1);
				if(index!=string::npos && index<index2)
				{
					value = parameterStr.substr(index1, index-index1);
					m_itemClassMap.insert(make_pair<string, string>(name, value));
					index1 = index + 1;
				}
				else
				{
					value = parameterStr.substr(index1, index2-index1);
					m_itemClassMap.insert(make_pair<string, string>(name, value));
					index = index2;
				}
			}
		}

		// �����׼�����
		if(ybjzk == "")
		{
			AfxMessageBox(_T("�ò��������ӡ�Է�֪��ͬ���飡"));
			return false;
		}
		else if(ybjzk!="CHARGE_SPECIAL_ITEM_DICT" && jzkfycs == "")
		{
			AfxMessageBox(_T("δ�鵽��Ӧ��׼����ò������������ñ��Ƿ������⣡"));
			return false;
		}
		m_ybjzk = ybjzk;
		m_jzkfycs = jzkfycs + ";";
		return true;
	}
}

void CPrintSelfConsentDlg::OnBnClickedCheckAll()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
	int state = pBtn->GetCheck();
	// ȫѡ
	if(state == 1)
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_DRUG);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_CURE);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_INSPECT);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
		pBtn->SetCheck(1);
	}
	else // ��ѡ��
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_DRUG);
		pBtn->SetCheck(0);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_CURE);
		pBtn->SetCheck(0);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_INSPECT);
		pBtn->SetCheck(0);
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
		pBtn->SetCheck(0);
	}
}


void CPrintSelfConsentDlg::OnBnClickedCheckDrug()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DRUG);
	int state = pBtn->GetCheck();
	// ��ѡ��
	if(state == 0)
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
		pBtn->SetCheck(0);
	}
}

void CPrintSelfConsentDlg::OnBnClickedCheckCure()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_CURE);
	int state = pBtn->GetCheck();
	// ��ѡ��
	if(state == 0)
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
		pBtn->SetCheck(0);
	}
}

void CPrintSelfConsentDlg::OnBnClickedCheckInspect()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_INSPECT);
	int state = pBtn->GetCheck();
	// ��ѡ��
	if(state == 0)
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
		pBtn->SetCheck(0);
	}
}


void CPrintSelfConsentDlg::OnBnClickedCheckOther()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_OTHER);
	int state = pBtn->GetCheck();
	// ��ѡ��
	if(state == 0)
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
		pBtn->SetCheck(0);
	}
}


void CPrintSelfConsentDlg::OnBnClickedQueryItem()
{
	UpdateData(true);
	// ����id��ѡ�е�ѡ��������ݿ��ѯ����ȡ����ӡ��Ŀ
	// ���id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("����id����Ϊ�գ�"));
		return;
	}

	if(!m_checkDrug && !m_checkCure && !m_checkInspect && !m_checkOther)
	{
		AfxMessageBox(_T("δѡ���κδ�ӡ��Ŀ��"));
		return;
	}

	// ��ѯ��Ŀ
	RESULT_MAP itemDbResult;
	map<string,string> param;
	// ��ȡ��ʼ���ڣ����������ַ���,visit_id
	char startDateStr[0x100];
	char endDateStr[0x100];
	// ��������Ĭ�ϼ�һ��
	CTime endDate = m_endDate + CTimeSpan(1, 0, 0, 0);
	::wsprintfA(startDateStr, "%ls", (LPCTSTR)m_startDate.Format("%Y-%m-%d"));
	::wsprintfA(endDateStr, "%ls", (LPCTSTR)endDate.Format("%Y-%m-%d"));

	string itemClassStr = "####";
	if(m_checkDrug)
	{
		multimap<string, string>::iterator iter = m_itemClassMap.find("drug");
		for(; iter!=m_itemClassMap.end(); ++iter)
		{
			if(iter->first != "drug")
				break;
			itemClassStr += "'"+iter->second+"',";
		}
	}
	if(m_checkCure) // ����
	{
		multimap<string, string>::iterator iter = m_itemClassMap.find("material");
		for(; iter!=m_itemClassMap.end(); ++iter)
		{
			if(iter->first != "material")
				break;
			itemClassStr += "'"+iter->second+"',";
		}
	}
	if(m_checkInspect) // ����
	{
		multimap<string, string>::iterator iter = m_itemClassMap.find("nursing");
		for(; iter!=m_itemClassMap.end(); ++iter)
		{
			if(iter->first != "nursing")
				break;
			itemClassStr += "'"+iter->second+"',";
		}
	}
	if(m_checkOther)
	{
		multimap<string, string>::iterator iter = m_itemClassMap.find("other");
		for(; iter!=m_itemClassMap.end(); ++iter)
		{
			if(iter->first != "other")
				break;
			itemClassStr += "'"+iter->second+"',";
		}
	}
	itemClassStr = itemClassStr.substr(0, itemClassStr.length()-1);

	// ��ȡ����id
	map<string, string>::iterator iter0 = m_infoMap.find("id");
	string patientId;
	if(iter0 != m_infoMap.end())
		patientId = iter0->second;
	else
	{
		AfxMessageBox(_T("����Ч����id��"));
		return;
	}
	// ��ȡvisit_id
	string visitId;
	iter0 = m_infoMap.find("visit_id");
	if(iter0 != m_infoMap.end())
		visitId = iter0->second;
	else
	{
		AfxMessageBox(_T("����ЧסԺ�����ţ�"));
		return;
	}
	// ����
	//AfxMessageBox(CString(("sql��visitId:"+visitId).c_str()));

	map<string, string>::iterator iterOfInfo = m_infoMap.find("chargeType");
	if(iterOfInfo != m_infoMap.end())
		param.insert(pair<string, string>("CHARGE_TYPE", iterOfInfo->second));
	param.insert(pair<string, string>("PATIENT_ID", patientId));  
	param.insert(pair<string, string>("VISIT_ID", visitId)); 
	param.insert(pair<string, string>("START_DATE", string(startDateStr)));  // ����ת��
	param.insert(pair<string, string>("END_DATE", string(endDateStr)));  
	param.insert(pair<string, string>("ITEM_CLASS_LIST", itemClassStr)); // ��Ŀ�����б�
	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ���˻�����Ϣ	
		string queryGroupName = "";
		if(m_optType == _T("����"))
			queryGroupName = m_ybjzk + "_SELF_CHARGE_CONSENT_ITEM_ALL";
		else if(m_optType == _T("δ��ӡ"))
			queryGroupName = m_ybjzk + "_SELF_CHARGE_CONSENT_ITEM_NO_PRINT";
		else if(m_optType == _T("�Ѵ�ӡ"))
			queryGroupName = "SELF_CHARGE_CONSENT_ITEM_PRINT";

		itemDbResult = dao.Query(queryGroupName, param);
		
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();

	// ������ѯ�������ʾ����ӡ��Ŀ
	// ����ѯ������Ŀ����������
	m_itemMap.clear();
	// ��Ŀ�����洢����
	m_itemAmountMap.clear();
	m_list.DeleteAllItems();
	// ��Ŀ�кű���,
	int col = 0;
	RESULT_MAP::iterator iter = itemDbResult.find("item");
	if(iter != itemDbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(; iter!=itemDbResult.end(); ++iter)
		{
			if(vecVecIter != iter->second.end())
			{
				for(;vecVecIter != iter->second.end(); ++vecVecIter)
				{
					m_list.InsertItem(col, _T("")); 
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					string itemStr = "";
					string dateStr = "";
					string classStr = "";
					string itemClassStr = "";
					string itemCodeStr = "";
					string printDate = "";
					string itemNo = "";
					string units = "";
					string fylbz = "";
					string numStr = "";
					double charges = 0.00;
					double cost = 0.00;
					double price = 0.00;
					char costStr[32] = "";
					double nums = 1.00;
					char indexStr[8];

					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						sprintf(indexStr, "%d", col);
						m_list.SetItemText(col, 1, CString(indexStr));
						if(vecIter->first == "ITEM_NAME") // ��Ŀ����
						{							  
							m_list.SetItemText(col, 3, CString(vecIter->second.c_str()));
							itemStr = vecIter->second;
						}
						else if(vecIter->first == "TIME") // ʱ��
						{
							m_list.SetItemText(col, 2, CString(vecIter->second.c_str())); 
							dateStr = vecIter->second;
						}
						else if(vecIter->first == "CLASS") // ���
						{
							classStr = vecIter->second+"��";
						}
						else if(vecIter->first == "ITEM_CODE") // ��Ŀ��
						{
							itemCodeStr = vecIter->second;
						}
						else if(vecIter->first == "ITEM_CLASS") // �����
						{
							itemClassStr = vecIter->second;
						}
						else if(vecIter->first == "PRINT_DATE") // ��ӡ����
						{
							m_list.SetItemText(col, 4, CString(vecIter->second.c_str()));
							printDate = vecIter->second;
						}
						else if(vecIter->first == "AMOUNT") // ����
						{
							numStr = vecIter->second;
							nums = atof(numStr.c_str());
						}
						else if(vecIter->first == "UNITS") // ��λ
						{
							units = vecIter->second;
						}
						else if(vecIter->first == "CHARGES") // ���
						{
							charges = atof((vecIter->second).c_str());
						}
						else if(vecIter->first == "PRICE") // ����
						{
							price = atof((vecIter->second).c_str());
						}
						else if(vecIter->first == "ITEM_TYPE") // �������
						{
							fylbz = vecIter->second;
						}
						else if(vecIter->first == "ITEM_NO") // ��Ŀ���
						{
							itemNo = vecIter->second;
						}
					}
					string itemType = fylbz; // �������ֵ����
					// ===================����������===================
					if(!m_ybjzk.empty() && m_ybjzk!="" && m_ybjzk!="CHARGE_SPECIAL_ITEM_DICT")
					{
						string::size_type index = m_jzkfycs.find(fylbz+":");
						if(fylbz!="" && index!=string::npos)
						{
							string::size_type index2 = m_jzkfycs.find(";", index);
							// ��ȡ�����жϵĲ�����,ʾ������ .10:����,0;0.00:����,0;-1:�Է�,1; �н�ȡ .10:����,0
							string checkStr = m_jzkfycs.substr(index, index2-index);
							// ��ȡ��Ӧ�������Ϣ
							index = checkStr.find(":", 0);
							index2 = checkStr.find(",", index);
							if(index!=string::npos && index2!=string::npos)
								fylbz = checkStr.substr(index+1, index2-index-1);
							else
								AfxMessageBox(_T("�Է�֪��ͬ�������ò�������"));
						}
						else // ������Ŀ������������������Ӧ����
						{
							index = m_jzkfycs.find("-1");
							string::size_type index2 = m_jzkfycs.find(";", index);
							// ��ȡ�����жϵĲ�����,ʾ������ .10:����,0;0.00:����,0;-1:�Է�,1; �н�ȡ .10:����,0			
							if(index!=string::npos && index2!=string::npos)
							{
								string checkStr = m_jzkfycs.substr(index, index2-index);
								// ��ȡ��Ӧ�������Ϣ
								index = checkStr.find(":", 0);
								index2 = checkStr.find(",", index);
								if(index!=string::npos && index2!=string::npos)
									fylbz = checkStr.substr(index+1, index2-index-1);
								else
									AfxMessageBox(_T("�Է�֪��ͬ�������ò�������"));
							}
							else
								AfxMessageBox(_T("�Է�֪��ͬ�������ò�������"));
						}
					}
					else 
					{
						fylbz = "�Է�";
					}
					// ===================�����͵��ۼ���===================
					if(price != 0.0)
						cost = price;
					else
					{
						if(nums == 0)
							cost = 0;
						else
							cost = charges / (double)nums;
					}
					sprintf(costStr, "%.2f", cost);

					// ===================��ӵ�������===================
					pair<string, string> pairVal = make_pair<string,string>(itemClassStr+classStr, dateStr+"#1#"+itemStr+"#2#"+itemCodeStr+"#3#"+itemClassStr+"#4#"+printDate+"#5#"+units+"#6#"+string(costStr)+"#7#"+fylbz+"#8#"+itemType+"#9#"+numStr+"#10#"+itemNo+"#11#"); // ��λ+����+�����������+�������ֵ+����
					m_itemMap.insert(pair<int, pair<string, string> >(col, pairVal));
					col++;
				}
			}
			else // δ�ҵ���¼
			{
				AfxMessageBox(_T("δ��ѯ������Ŀ��Ϣ��"));
			}
		}
		
	}
	// δ�ҵ���¼
	else
	{
		AfxMessageBox(_T("δ��ѯ������Ŀ��Ϣ��"));
	}

	UpdateData(false);
}


void CPrintSelfConsentDlg::OnBnClickedPrint()
{
	UpdateData(true);
	// ��ȡѡ����Ŀ ����-��Ŀ��Ϣ�ַ���
	map<string, vector<string> > itemMap;
	map<int, pair<string, string> >::iterator iter;

	//����ӡ�����Ƿ�����
	if(m_patientId == "")
	{
		AfxMessageBox(_T("����id����Ϊ�գ�"));
		return;
	}
	else if(m_list.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("��ѡ���ӡ��Ŀ��"));
		return;
	}
	else 
	{
		bool isSelected = false;
		for(int i=0; i<m_list.GetItemCount(); i++)
		{
			if(m_list.GetCheck(i))
			{
				isSelected = true;
				break;
			}
		}
		if(!isSelected)
		{
			AfxMessageBox(_T("��ѡ���ӡ��Ŀ��"));
			return;
		}
	}
	
	// ѭ����ȡ��ӡ��Ŀ
	string itemCodeStr="", numsStr="", printDate="", dateStr="", itemNo=""; 
	double nums = 0;
	// �洢�Ѵ�ӡ��Ŀ����
	while(!m_isAllowOpt); // �ȴ�����
	m_itemRecordMap.clear();
	m_itemAmountMap.clear();

	char indexStr[8];
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{
			::wsprintfA(indexStr, "%ls", (LPCTSTR)m_list.GetItemText(i, 1));
			int indexNum = atoi(indexStr);
			iter = m_itemMap.find(indexNum);
			if(iter != m_itemMap.end())
			{
				// ��ȡҽ������
				string::size_type index0 = iter->second.second.find("#1#");
				dateStr = iter->second.second.substr(0, index0);
				// ��ȡitem_code,���¶�Ӧ����Ŀͳ��
				index0 = iter->second.second.find("#2#");
				string::size_type index = iter->second.second.find("#3#", index0);
				if(index != string::npos)
					itemCodeStr = iter->second.second.substr(index0+3, index-index0-3);
				// ��ȡ��ӡ����
				index0 = iter->second.second.find("#4#", index);
				index = iter->second.second.find("#5#", index0);
				if(index != string::npos)
					printDate = iter->second.second.substr(index0+3, index-index0-3);
				// ��ȡ����
				index0 = iter->second.second.find("#9#", index);
				index = iter->second.second.find("#10#", index0);
				if(index != string::npos)
				{
					numsStr = iter->second.second.substr(index0+3, index-index0-3);
					nums = atof(numsStr.c_str());
				}
				// ��ȡitem_no
				index0 = iter->second.second.find("#10#", index);
				index = iter->second.second.find("#11#", index0);
				if(index != string::npos)
					itemNo = iter->second.second.substr(index0+4, index-index0-4);
				// ���¼���
				m_itemAmountMap.insert(pair<string, double>(itemCodeStr+","+itemNo, nums));
				map<string, double>::iterator amountIter = m_itemAmountMap.find(itemCodeStr);
				// �Ѵ���
				if(amountIter != m_itemAmountMap.end())
				{
					amountIter->second += nums;
				}
				else
				{
					m_itemAmountMap.insert(pair<string, double>(itemCodeStr, nums));
					// �������ӡ������
					// �Ƿ��Ѵ�����������
					map<string, vector<string> >::iterator tmpIter = itemMap.find(iter->second.first);
					// �����������vector��
					if(tmpIter != itemMap.end())
						tmpIter->second.push_back(iter->second.second);
					else // ���������½�
					{
						vector<string> tmpVec;
						tmpVec.push_back(iter->second.second);
						pair<string, vector<string> > tmpPair = make_pair<string, vector<string> >(iter->second.first, tmpVec);
						itemMap.insert(tmpPair);
					}
				}
				// �����д�����ݿ��������
				while(!m_isAllowOpt); // �ȴ�����
			
				// �鿴�ü�¼�Ƿ��Ѳ�������
				map<string, string>::iterator recordIter = m_itemRecordMap.find(itemCodeStr+","+itemNo);
				if(recordIter == m_itemRecordMap.end())
					m_itemRecordMap.insert(pair<string, string>(itemCodeStr+","+itemNo, iter->second.second));
				else if(printDate != "")// �����������ڸ���Ϊ�����ӡ��
				{
					string recordPrintDate = "", recordDateStr = "";
					string::size_type index0 = recordIter->second.find("#1#");
					if(index0 != string::npos)
						recordDateStr = recordIter->second.substr(0, index0);
					string::size_type index1 = recordIter->second.find("#4#");
					string::size_type index2 = iter->second.second.find("#5#", index1);
					if(index2 != string::npos)
						recordPrintDate = recordIter->second.substr(index1+3, index2-index1-3);
					// �����ӡ���ڱ��Ѵ洢�����滻
					if(dateStr==recordDateStr && printDate>recordPrintDate)
						recordIter->second = iter->second.second;
				}
			}
		}
	}

	// ��ȡ��ӡ��ӿڽ��д�ӡ����
	try
	{
		CMyPrinter printer;
		printer.ReadHtmlTemplate(PRINT_SELF_CHARGE_CONSENT_LETTER);
		printer.ProducePrintContent(PRINT_SELF_CHARGE_CONSENT_LETTER, m_infoMap, itemMap, m_itemAmountMap);
		printer.ProduceHtmlFile();

		// ��ӡ��Ŀ����������ʾ�б����
		m_itemMap.clear();
		m_list.DeleteAllItems();

		// ���߳̽���¼д�����ݿ�
		AfxBeginThread(WriteInfoDB, this);
	}
	catch(runtime_error err)
	{
		m_isAllowOpt = true;
		AfxMessageBox(CString(err.what()));
	}
	UpdateData(false);
}


void CPrintSelfConsentDlg::OnBnClickedQueryPatientInfo()
{
	UpdateData(true);
	// ���id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("����id����Ϊ�գ�"));
		return;
	}
	
	char patientIdStr[0x100];
	::wsprintfA(patientIdStr, "%ls", (LPCTSTR)m_patientId);

	// ����id��ѯ������Ϣ
	RESULT_MAP basicInfoDbResult;
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", string(patientIdStr)));

	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ���˻�����Ϣ
		CString ss = _T("");
		
		if(m_visitId == _T(""))
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_NO_VISIT_ID", param);
		else
		{
			char visitIdStr[0x100];
			::wsprintfA(visitIdStr, "%ls", (LPCTSTR)m_visitId);
			//AfxMessageBox(m_visitId+_T(":��ȡ��visitId"));
			param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_VISIT_ID", param);
		}
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();

	// ����������Ϣ
	// ��ȡ��ѯ����item_name��item_code
	RESULT_MAP::iterator iter = basicInfoDbResult.find("basicInfo");
	if(iter != basicInfoDbResult.end())
	{
		// �ҵ���Ŀ����ȡ��Ӧ����
		// ��������Ϣ���뵽������,�������Ӧ����ʾ������
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			
			m_infoMap.clear();
			m_infoMap.insert(make_pair<string, string>("id", string(patientIdStr)));
			GetDlgItem(IDC_ID_NUM)->SetWindowTextW(m_patientId);

			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "NAME") // ����
					{
						GetDlgItem(IDC_NAME)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("name", vecIter->second));
					}
					else if(vecIter->first == "SEX") // �Ա�
					{
						GetDlgItem(IDC_SEX)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("sex", vecIter->second));
					}
					else if(vecIter->first == "CHARGE_TYPE") // �ѱ�
					{
						m_chargeType = vecIter->second;
						GetDlgItem(IDC_CHARGETYPE)->SetWindowTextW(CString(m_chargeType.c_str()));
						m_infoMap.insert(make_pair<string, string>("chargeType", m_chargeType));
					}
					else if(vecIter->first == "DIAGNOSIS") // ���
					{
						GetDlgItem(IDC_DIAGNOSE)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("diagenosis", vecIter->second));
					}
					else if(vecIter->first == "DEPT_NAME") // ����
					{
						GetDlgItem(IDC_DEPARTMENT)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("department", vecIter->second));
					}
					else if(vecIter->first == "ADMISSION_DATE_TIME") // סԺ����
					{
						string date = vecIter->second;
						string::size_type index = date.find(" ");
						if(index != string::npos)
							date = date.substr(0, index);
						GetDlgItem(IDC_START_IN_HOSPITAL)->SetWindowTextW(CString(date.c_str()));
					}
					else if(vecIter->first == "DISCHARGE_DATE_TIME") // ��Ժ����
					{
						string date = vecIter->second;
						string::size_type index = date.find(" ");
						if(index != string::npos)
							date = date.substr(0, index);
						GetDlgItem(IDC_END_IN_HOSPITAL)->SetWindowTextW(CString(date.c_str()));
					}
					else if(vecIter->first == "VISIT_ID") // visitId
					{
						GetDlgItem(IDC_VISIT_ID)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_visitIdCtr.SetWindowTextW(CString(vecIter->second.c_str()));
						m_visitId = CString(vecIter->second.c_str());
						m_infoMap.insert(make_pair<string, string>("visit_id", vecIter->second));
						m_queryVisitID = vecIter->second;
						// ����
						//AfxMessageBox(CString(("��ȡ����ѯvisitId:"+vecIter->second).c_str()));
					}
				}
			}
		}
		else // δ�ҵ���¼
		{
			AfxMessageBox(_T("δ��ѯ���û�����Ϣ��"));
			return;
		}
		
	}
	// δ�ҵ���¼
	else
	{
		AfxMessageBox(_T("δ��ѯ���û�����Ϣ��"));
		return;
	}

	// ��ȡ��ѯ��������visit_id
	iter = basicInfoDbResult.find("visitInfo");
	m_visitIdCtr.ResetContent();
	if(iter != basicInfoDbResult.end())
	{
		// �ҵ���Ŀ����ȡ��Ӧ����
		// ��������Ϣ���뵽������,�������Ӧ����ʾ������
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "VISIT_ID") // סԺ����
					{
						m_visitIdCtr.AddString(CString(vecIter->second.c_str()));
					}
				
				}
			}
		}
		else // δ�ҵ���¼
		{
			AfxMessageBox(_T("δ��ѯ���û�����Ϣ��"));
			return;
		}
		
	}
	// δ�ҵ���¼
	else
	{
		AfxMessageBox(_T("δ��ѯ���û�����Ϣ��"));
		return;
	}
	
	// ��ѯ��Ŀǰ��ʼ��
	if(Init())
		m_list.DeleteAllItems();

	UpdateData(false);
	
	// ����ȫѡ
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
	pBtn->SetCheck(1);
	OnBnClickedCheckAll();

}

// ������
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{

	// �Ӳ�������ȡ����Ƚ�lc����������
	int row1 = (int) lParam1;
	int row2 = (int) lParam2;

	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1, g_sortColumn);
	CString lp2 = lc->GetItemText(row2, g_sortColumn);

	// �Ƚϣ��Բ�ͬ���У���ͬ�Ƚϣ�ע���¼ǰһ����������һ��Ҫ�෴����
	// �����ͱȽ�
	if (g_method[g_sortColumn-2])
		return lp1.CompareNoCase(lp2);
	else
		return lp2.CompareNoCase(lp1);

	return 0;
}

void CPrintSelfConsentDlg::OnColumnclickItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// ���ȫѡ
	if(pNMListView->iItem==-1 && pNMListView->iSubItem==0)
	{
		if(m_isAllSelected)
			m_isAllSelected = false;
		else
			m_isAllSelected = true;

		for(int i=0; i<m_list.GetItemCount(); i++)
		{
			if(m_isAllSelected)
				m_list.SetCheck(i, 1);
			else
				m_list.SetCheck(i, 0);
		}
	}
	else if(pNMListView->iSubItem >= 2)
	{
		g_sortColumn = pNMListView->iSubItem;
		int count = m_list.GetItemCount();
		// ÿ�еıȽϹؼ��֣��˴�Ϊ����ţ�������кţ�����������Ϊ�����ȽϺ����ĵ�һ��������
		for (int i=0;i<count;i++)
			m_list.SetItemData(i,i); 

		g_method[g_sortColumn-2] = !g_method[g_sortColumn-2];
		m_list.SortItems(MyCompareProc,(DWORD_PTR)&m_list);

	}
	*pResult = 0;
}


void CPrintSelfConsentDlg::OnBnClickedAddItemButton()
{
	if(m_patientId==_T("") || m_visitId==_T(""))
	{
		AfxMessageBox(_T("����ID��סԺ�Ų���Ϊ�գ����Ȳ�ѯ��������Ϣ���������Ŀ��"));
		return;
	}
	
	// ��ʾ�����Ŀ����
	CAddItemDlg dlg;
	dlg.Init(m_patientId, m_visitId, m_chargeType);
	dlg.DoModal();
	// ��ȡ��ӵ���Ŀ
	pair<string, string> addItem = dlg.GetAddItem();
	// ���itemCode
	string itemCode = dlg.GetItemCode();

	// �����Ŀ���Ʋ�Ϊ�գ���ӵ�����ӡ������ȥ������ʾ����
	if(addItem.second != "")
	{		
		UpdateData(true);
		int index = m_list.GetItemCount();

		// ��ӵ��б�����ʾ
		string itemStr = addItem.second.substr(3);
		string::size_type index0 = itemStr.find("#2#", 0);
		if(index0 != string::npos)
			itemStr = itemStr.substr(0, index0);
		else
		{
			AfxMessageBox(_T("�����Ŀʧ�ܣ�"));
			return;
		}

		char indexStr[8];
		int col = m_list.GetItemCount();
		sprintf(indexStr, "%d", col);
		m_list.InsertItem(index,  _T(""));   
		m_list.SetItemText(index, 1, CString(indexStr)); 
		m_list.SetItemText(index, 2, _T(""));   
		m_list.SetItemText(index, 3, CString(itemStr.c_str()));
		m_list.SetItemText(index, 4, _T(""));
		
		// ��ӵ�������
		m_itemMap.insert(pair<int, pair<string, string> >(index, addItem));
		UpdateData(false);
	}
}


void CPrintSelfConsentDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(true);
	if(m_startDate > m_endDate)
	{
		m_startDate = m_endDate;		
	}
	UpdateData(false);
	*pResult = 0;
}


void CPrintSelfConsentDlg::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	UpdateData(true);
	if(m_startDate > m_endDate)
	{
		m_endDate = m_startDate;		
	}
	UpdateData(false);
	*pResult = 0;
}



void CPrintSelfConsentDlg::OnEnChangeEdit1()
{
	m_visitIdCtr.ResetContent();
}


void CPrintSelfConsentDlg::OnEnKillfocusEdit1()
{
	UpdateData(true);
	char patientIdStr[0x100];
	::wsprintfA(patientIdStr, "%ls", (LPCTSTR)m_patientId);
	// ����id��ѯ������Ϣ
	RESULT_MAP basicInfoDbResult;
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", string(patientIdStr)));

	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ���˻�����Ϣ
		CString ss = _T("");
		
		if(m_visitId == _T(""))
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_NO_VISIT_ID", param);
		else
		{
			char visitIdStr[0x100];
			::wsprintfA(visitIdStr, "%ls", (LPCTSTR)m_visitId);
			//AfxMessageBox(m_visitId+_T(":��ȡ��visitId"));
			param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_VISIT_ID", param);
		}
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();
	
	// ��ȡ��ѯ��������visit_id
	RESULT_MAP::iterator iter = basicInfoDbResult.find("visitInfo");
	m_visitIdCtr.ResetContent();
	string visitId = "";
	if(iter != basicInfoDbResult.end())
	{
		// �ҵ���Ŀ����ȡ��Ӧ����
		// ��������Ϣ���뵽������,�������Ӧ����ʾ������
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "VISIT_ID") // סԺ����
					{
						visitId = vecIter->second;
						m_visitIdCtr.AddString(CString(visitId.c_str()));
					}
				
				}
			}
			m_visitIdCtr.SetWindowTextW(CString(visitId.c_str()));
		}
		else // δ�ҵ���¼
		{
			AfxMessageBox(_T("δ��ѯ���û�����Ϣ��"));
			return;
		}
		
	}
	// δ�ҵ���¼
	else
	{
		AfxMessageBox(_T("δ��ѯ���û�����Ϣ��"));
		return;
	}
}
