///////////////////////////////////////////////////////////////////////
//�ļ����� AddItemDlg.cpp
//��  �ܣ� �����Ŀ��ʵ�֣����ڴ�ӡ�Է�֪��ͬ���飩
//�����ߣ� ������
//��  ����2014/7/27
//��  �ģ�2014/7/27
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIES.h"
#include "AddItemDlg.h"
#include "sql_dao.h"
#include "afxdialogex.h"
#include <sstream>

using namespace std;

// �̺߳���
UINT ThreadFunc(LPVOID lpParam)
{
	if(lpParam != NULL)
    {
		CAddItemDlg* pDlg=(CAddItemDlg*)lpParam;
		pDlg->QueryItem();
	}
    return 0;
}

// CAddItemDlg �Ի���

IMPLEMENT_DYNAMIC(CAddItemDlg, CDialogEx)

CAddItemDlg::CAddItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddItemDlg::IDD, pParent)
	, m_itemName(_T(""))
	, m_page(0)
	, m_item(_T(""))
	, m_isEnd(false)
	, m_amount(1)
{
	m_addItem = make_pair<string, string>("", "");
}

CAddItemDlg::~CAddItemDlg()
{
}

void CAddItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboCtr);
	DDX_Text(pDX, IDC_EDIT1, m_itemName);
	DDX_CBString(pDX, IDC_COMBO1, m_item);
	DDX_Text(pDX, IDC_EDIT2, m_amount);
	DDV_MinMaxInt(pDX, m_amount, 1, 100000);
}


BEGIN_MESSAGE_MAP(CAddItemDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddItemDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddItemDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CAddItemDlg::OnEnChangeInputCode)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddItemDlg::OnBnClickedPrePageButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddItemDlg::OnBnClickedNextPageButton)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CAddItemDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CAddItemDlg ��Ϣ�������

// ��ʼ������
void CAddItemDlg::Init(CString patientId, CString visitId, string chargeType)
{
	m_chargeType = chargeType;
	RESULT_MAP dbParameter;
	// ��ʼ�������б�
	map<string,string> param;
	char patientIdStr[256];
	::wsprintfA(patientIdStr, "%ls", (LPCTSTR)patientId);
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
		
		char visitIdStr[256];
		::wsprintfA(visitIdStr, "%ls", (LPCTSTR)visitId);
		param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
		dbParameter = dao.Query("ZFZQTYSParameter_VISITID", param);

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
					// ��ȡ��Ӧ��׼��Ĳ���ֵ
					if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
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
		return;
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
	if(ybjzk=="" || jzkfycs=="")
	{
		AfxMessageBox(_T("δ�鵽��Ӧ��׼��������������ñ��Ƿ������⣡"));
		return;
	}
	m_ybjzk = ybjzk;
	m_jzkfycs = jzkfycs + ";";
}

void CAddItemDlg::OnBnClickedOk()
{
	UpdateData(true);

	if(m_amount < 1)
		AfxMessageBox(_T("��������Ч������"));
	string className = "";
	// ��CStringת��string
	char itemName[256];
	::wsprintfA(itemName, "%ls", (LPCTSTR)m_item);
	string itemNameStr(itemName);
	string valueStr = "";
	try
	{
		multimap<string, string>::iterator iter = m_itemClassMap.find(itemNameStr);
		if(iter != m_itemClassMap.end())
			valueStr = iter->second;
		else
			throw runtime_error("��Ŀ��ȡ�����⣡");

		// ���»ָ�itemName��ֵ
		string::size_type index1 = itemNameStr.find("����λ��");
		if(index1 != string::npos)
			itemNameStr = itemNameStr.substr(0, index1);
		else
			throw runtime_error("δ��������Ŀ��λ��");
		// ��ȡclassName;
		index1 = valueStr.find("#2#");
		if(index1 != string::npos)
		{
			className = valueStr.substr(0, index1);
			valueStr.replace(0, index1, "#1#"+itemNameStr);
		}
		else
			throw runtime_error("��Ŀ��ȡ�����⣡");

		// ���������Ŀ�ֶ�
		char amountStr[10];
		sprintf(amountStr, "%d", m_amount);
		m_addItem = make_pair<string,string>(className+"��", valueStr+string(amountStr)+"#10##11#");

		if(m_item != _T(""))
			CDialogEx::OnOK();
		else
			throw runtime_error("δѡ���κ���Ŀ��");
	}
	catch(runtime_error err)
	{
		AfxMessageBox(CString(err.what()));
	}
	UpdateData(false);
}


void CAddItemDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CAddItemDlg::OnEnChangeInputCode()
{
	UpdateData(true);
	m_comboCtr.ResetContent();
	m_page = 0;
	m_isEnd = false;
	// ��ѯ��Ŀ
	AfxBeginThread(ThreadFunc, this);    //����MFC�߳�
	// ��������ѡ���
	m_comboCtr.ShowDropDown();
	::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
	UpdateData(false);
}


void CAddItemDlg::OnBnClickedPrePageButton()
{
	UpdateData(true);
	if(m_page <= 0)
	{
		m_comboCtr.ShowDropDown();
		return;
	}
	else
	{
		m_page--;
		if(QueryItem())
			m_isEnd = false;
	}
	// ��������ѡ���
	UpdateData(false);
	m_comboCtr.ShowDropDown();
	m_comboCtr.SetFocus();
	::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
	UpdateData(false);
}


void CAddItemDlg::OnBnClickedNextPageButton()
{
	UpdateData(true);
	if(m_isEnd)
	{
		m_comboCtr.ShowDropDown();
		return;
	}
	else
	{
		m_page++;
		if(QueryItem())
			m_isEnd = false;
		else
			m_isEnd = true;
	}
	// ��������ѡ���
	UpdateData(false);
	m_comboCtr.ShowDropDown();
	m_comboCtr.SetFocus();
	::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
	UpdateData(false);
}

//�� ��:���ݿ��ѯ��Ŀ
//�� ��:��
//�� ��:�Ƿ�鵽��Ŀ
bool CAddItemDlg::QueryItem()
{
	// �Ƿ�鵽������
	bool isNullItem = false;
	// ��ѯ���ݿ��ȡ��Ŀ
	char codeStr[0x100];
	::wsprintfA(codeStr, "%ls", (LPCTSTR)m_itemName);
	RESULT_MAP dbResult;
	// ��ʼ�������б�
	string code(codeStr);
	string pageStr1, pageStr2;
	int page1 = m_page*10+1, page2 = (m_page+1)*10;
	stringstream ss;
	ss.clear();
	ss<<page1;
	ss>>pageStr1;
	ss.clear();
	ss<<page2;
	ss>>pageStr2;

	map<string,string> param;
	param.insert(pair<string, string>("CODE", "####"+code));
	param.insert(pair<string, string>("CHARGE_TYPE", m_chargeType));
	param.insert(pair<string, string>("RECORD1", "####"+pageStr1));
	param.insert(pair<string, string>("RECORD2", "####"+pageStr2));
	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ
		
		// ���Ҳ�����
		
		dbResult = dao.Query(m_ybjzk+"_ITEM_QUERY", param);
		
		// ���������������ö�Ӧ��׼������ò���ֵ
		RESULT_MAP::iterator iter = dbResult.find("item");
		while(iter != dbResult.end())
		{
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter == iter->second.end())
			{
				isNullItem = true;
				break;
			}
			
			m_comboCtr.ResetContent();
			m_itemClassMap.clear();

			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				string nums = "", itemName = "", className = "", itemCode = "", itemClass = "", units = "", price = "", fylbz = "";
				
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "ITEM_NAME")
					{
						itemName = vecIter->second;
					}
					else if(vecIter->first == "CLASS_NAME")
					{
						className = vecIter->second;
					}
					else if(vecIter->first == "ITEM_CODE") // ��Ŀ��
					{
						itemCode = vecIter->second;
					}
					else if(vecIter->first == "ITEM_CLASS") // �����
					{
						itemClass = vecIter->second;
					}
					else if(vecIter->first == "UNITS") // ��λ
					{
						units = vecIter->second;
					}
					else if(vecIter->first == "AMOUNT") // ����
					{
						string numsStr = vecIter->second;
						nums = atoi(numsStr.c_str());
					}
					else if(vecIter->first == "PRICE") // ����
					{
						price = vecIter->second;
					}
					else if(vecIter->first == "ITEM_TYPE") // �������ֵ
					{
						fylbz = vecIter->second;
					}
				}
				string itemType = fylbz; // �������ֵ����
				itemName += ("����λ��"+units);
				m_comboCtr.AddString(CString(itemName.c_str()));
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
				// ��ӵ�������
				m_itemClassMap.insert(make_pair<string, string>(itemName, itemClass+className+"#2#"+itemCode+"#3#"+itemClass+"#4##5#"+units+"#6#"+price+"#7#"+fylbz+"#8#"+itemType+"#9#")); // ��λ+����+�������
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
	return !isNullItem;
}

void CAddItemDlg::OnEnKillfocusEdit1()
{
	// ��������ѡ���
	m_comboCtr.ShowDropDown();
	m_comboCtr.SetFocus();
}
