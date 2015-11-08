///////////////////////////////////////////////////////////////////////
//文件名： PrintSelfConsentDlg.cpp
//功  能： 打印自费知情同意书类实现
//开发者： 贾治中
//创  建：2014/7/24
//修  改：2014/7/27
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIES.h"
#include "PrintSelfConsentDlg.h"
#include "AddItemDlg.h"
#include "MyPrinter.h"
#include "sql_dao.h"	// CSqlDao类头文件
#include "afxdialogex.h"

using namespace std;

// 用于排序的全局变量
// 点击的列
int g_sortColumn;
// 排序方法：顺序、逆序
bool g_method[3];

// 写入数据库已打印部分
UINT WriteInfoDB(LPVOID lpParam)
{
	if(lpParam != NULL)
    {
		CPrintSelfConsentDlg* pDlg = (CPrintSelfConsentDlg*)lpParam;
		pDlg->m_isAllowOpt = false;
		map<string,string>::iterator iter = (pDlg->m_itemRecordMap).begin();
		// 获取当前时间
		CTime currentTime = CTime::GetCurrentTime();
		CString szTime = currentTime.Format("%Y-%m-%d %H:%M:%S");
		char printDate[0x100];
		char patientId[0x100];
		::wsprintfA(printDate, "%ls", (LPCTSTR)szTime);
		::wsprintfA(patientId, "%ls", (LPCTSTR)pDlg->m_patientId);

		// 获取待存储各个变量
		
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
			// sql语句
			map<string, double>::iterator iter0 = pDlg->m_itemAmountMap.find(itemCode+","+itemNo);
			double amount = 0;
			char amountStr[8];
			if(iter0 != pDlg->m_itemAmountMap.end())
				amount = iter0->second;
			sprintf(amountStr, "%.2f", amount);

			string sql = "";

			// 数据库中已记录，则更新
			if(recordPrintDate != "")
			{
				sql = "UPDATE DWYL.PRINT_ITEM_RECORD SET PRINT_DATE=to_date('"+string(printDate)+"','yyyy-mm-dd hh24:mi:ss') WHERE PATIENT_ID='"+string(patientId)+"' AND VISIT_ID='"+pDlg->m_queryVisitID+
					"' AND ITEM_CODE='"+itemCode+"' AND BILLING_DATE_TIME=to_date('"+billingDate+"','yyyy-mm-dd hh24:mi:ss')"+" AND ITEM_NO='"+itemNo+"'";
			}
			else // 插入
			{
				if(billingDate != "")
					sql = "INSERT INTO DWYL.PRINT_ITEM_RECORD VALUES('"+string(patientId)+"','"+pDlg->m_queryVisitID+
					"','"+itemCode+"',to_date('"+billingDate+"','yyyy-mm-dd hh24:mi:ss'),to_date('"+string(printDate)+"','yyyy-mm-dd hh24:mi:ss'),'"+itemClass+"','"+itemName+"','"+amountStr+"','"+price+"','"+itemType+"','"+units+"','"+itemNo+"')";
				else
					sql = "INSERT INTO DWYL.PRINT_ITEM_RECORD VALUES('"+string(patientId)+"','"+pDlg->m_queryVisitID+
					"','"+itemCode+"',null,to_date('"+string(printDate)+"','yyyy-mm-dd hh24:mi:ss'),'"+itemClass+"','"+itemName+"','"+amountStr+"','"+price+"','"+itemType+"','"+units+"','"+itemNo+"')";
			}
			// 执行插入操作
			CDB db;
			try
			{
				db.Init("MIES_db_config.conf");
				db.Submit(sql);
				db.Close();
			}
			catch( runtime_error err )
			{
				// 异常处理代码
				AfxMessageBox(CString(err.what()));
				// 关闭对象
				db.Close();
				return 0;
			}
		}
		
		(pDlg->m_itemRecordMap).clear();		
		pDlg->m_isAllowOpt = true;
	}
    return 0;
}

// CPrintSelfConsentDlg 对话框

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
	
	// 不设置该静态文本皮肤
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

	// 设置列表风格
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT | LVS_EX_CHECKBOXES); 
	// 设置列表标题
	m_list.InsertColumn(0, _T("全选"), LVCFMT_CENTER, 50, 60);
	m_list.InsertColumn(1, _T("打印号"), LVCFMT_CENTER, 50, 60);
	m_list.InsertColumn(2, _T("日期"), LVCFMT_CENTER, 180, 60);
	m_list.InsertColumn(3, _T("项目"), LVCFMT_LEFT, 540, 60);
	m_list.InsertColumn(4, _T("打印日期"), LVCFMT_CENTER, 180, 60);

	// 默认设置打印选项为未打印
	CComboBox* printCtr = (CComboBox*)GetDlgItem(IDC_OPT_TYPE_COMBO);
	printCtr->SetCurSel(1);
	if(m_patientId != _T(""))
		OnBnClickedQueryPatientInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
		// 初始化参数列表
		map<string,string> param;
		char patientIdStr[256];
		::wsprintfA(patientIdStr, "%ls", (LPCTSTR)m_patientId);
		param.insert(pair<string, string>("PATIENT_ID", string(patientIdStr)));
		// 打印项目参数
		string parameterStr = "";
		// 基准库
		string ybjzk = "";
		// 基准库费用类型
		string jzkfycs = "";
		// 费用类别参数字符串
		string chargeStr;
		// 创建查询对象
		CSqlDao dao;
		try
		{
			// 初始化查询对象
			dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
			// 执行查询
		
			// 查找参数表
			if(m_visitId == _T("")) 
				dbParameter = dao.Query("ZFZQTYSParameter_NO_VISITID", param);
			else
			{
				char visitIdStr[256];
				::wsprintfA(visitIdStr, "%ls", (LPCTSTR)m_visitId);
				param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
				dbParameter = dao.Query("ZFZQTYSParameter_VISITID", param);
			}

			// 解析参数结果，获得配置参数值
			RESULT_MAP::iterator iter = dbParameter.find("parameter");
			while(iter != dbParameter.end())
			{
				RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
				for(;vecVecIter != iter->second.end(); ++vecVecIter)
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						// 获取自费知情同意书的参数值
						if(vecIter->first == "NAME" && vecIter->second == "ZFZQTYS")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									parameterStr = vecIter->second;
							}
						}
						// 获取对应基准库的参数值
						else if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									ybjzk = vecIter->second;
							}
						}
						// 获取费用类别参数
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
			// 异常处理代码
			AfxMessageBox(CString(err.what()));
			// 关闭对象
			dao.Close();
			return false;
		}
		// 关闭对象
		dao.Close();

		// 解析打印项目参数,存储到参数容器中
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
			// 获取键
			index = parameterStr.find(":", index1);
			if(index == string::npos)
				break;
			string name = parameterStr.substr(index1, index-index1);
			// 逐个获取值
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

		// 保存基准库参数
		if(ybjzk == "")
		{
			AfxMessageBox(_T("该病人无需打印自费知情同意书！"));
			return false;
		}
		else if(ybjzk!="CHARGE_SPECIAL_ITEM_DICT" && jzkfycs == "")
		{
			AfxMessageBox(_T("未查到对应基准库费用参数，请检查配置表是否有问题！"));
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
	// 全选
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
	else // 不选中
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
	// 不选中
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
	// 不选中
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
	// 不选中
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
	// 不选中
	if(state == 0)
	{
		pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
		pBtn->SetCheck(0);
	}
}


void CPrintSelfConsentDlg::OnBnClickedQueryItem()
{
	UpdateData(true);
	// 根据id和选中的选项进行数据库查询，获取待打印项目
	// 检查id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("患者id不可为空！"));
		return;
	}

	if(!m_checkDrug && !m_checkCure && !m_checkInspect && !m_checkOther)
	{
		AfxMessageBox(_T("未选中任何打印项目！"));
		return;
	}

	// 查询项目
	RESULT_MAP itemDbResult;
	map<string,string> param;
	// 获取起始日期，结束日期字符串,visit_id
	char startDateStr[0x100];
	char endDateStr[0x100];
	// 结束日期默认加一天
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
	if(m_checkCure) // 材料
	{
		multimap<string, string>::iterator iter = m_itemClassMap.find("material");
		for(; iter!=m_itemClassMap.end(); ++iter)
		{
			if(iter->first != "material")
				break;
			itemClassStr += "'"+iter->second+"',";
		}
	}
	if(m_checkInspect) // 护理
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

	// 获取患者id
	map<string, string>::iterator iter0 = m_infoMap.find("id");
	string patientId;
	if(iter0 != m_infoMap.end())
		patientId = iter0->second;
	else
	{
		AfxMessageBox(_T("无有效患者id！"));
		return;
	}
	// 获取visit_id
	string visitId;
	iter0 = m_infoMap.find("visit_id");
	if(iter0 != m_infoMap.end())
		visitId = iter0->second;
	else
	{
		AfxMessageBox(_T("无有效住院次数号！"));
		return;
	}
	// 测试
	//AfxMessageBox(CString(("sql处visitId:"+visitId).c_str()));

	map<string, string>::iterator iterOfInfo = m_infoMap.find("chargeType");
	if(iterOfInfo != m_infoMap.end())
		param.insert(pair<string, string>("CHARGE_TYPE", iterOfInfo->second));
	param.insert(pair<string, string>("PATIENT_ID", patientId));  
	param.insert(pair<string, string>("VISIT_ID", visitId)); 
	param.insert(pair<string, string>("START_DATE", string(startDateStr)));  // 日期转换
	param.insert(pair<string, string>("END_DATE", string(endDateStr)));  
	param.insert(pair<string, string>("ITEM_CLASS_LIST", itemClassStr)); // 项目类型列表
	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询病人基本信息	
		string queryGroupName = "";
		if(m_optType == _T("所有"))
			queryGroupName = m_ybjzk + "_SELF_CHARGE_CONSENT_ITEM_ALL";
		else if(m_optType == _T("未打印"))
			queryGroupName = m_ybjzk + "_SELF_CHARGE_CONSENT_ITEM_NO_PRINT";
		else if(m_optType == _T("已打印"))
			queryGroupName = "SELF_CHARGE_CONSENT_ITEM_PRINT";

		itemDbResult = dao.Query(queryGroupName, param);
		
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();

	// 解析查询结果并显示待打印项目
	// 将查询到的项目存入容器中
	m_itemMap.clear();
	// 项目数量存储容器
	m_itemAmountMap.clear();
	m_list.DeleteAllItems();
	// 项目行号变量,
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
						if(vecIter->first == "ITEM_NAME") // 项目名称
						{							  
							m_list.SetItemText(col, 3, CString(vecIter->second.c_str()));
							itemStr = vecIter->second;
						}
						else if(vecIter->first == "TIME") // 时间
						{
							m_list.SetItemText(col, 2, CString(vecIter->second.c_str())); 
							dateStr = vecIter->second;
						}
						else if(vecIter->first == "CLASS") // 类别
						{
							classStr = vecIter->second+"类";
						}
						else if(vecIter->first == "ITEM_CODE") // 项目码
						{
							itemCodeStr = vecIter->second;
						}
						else if(vecIter->first == "ITEM_CLASS") // 类别码
						{
							itemClassStr = vecIter->second;
						}
						else if(vecIter->first == "PRINT_DATE") // 打印日期
						{
							m_list.SetItemText(col, 4, CString(vecIter->second.c_str()));
							printDate = vecIter->second;
						}
						else if(vecIter->first == "AMOUNT") // 数量
						{
							numStr = vecIter->second;
							nums = atof(numStr.c_str());
						}
						else if(vecIter->first == "UNITS") // 单位
						{
							units = vecIter->second;
						}
						else if(vecIter->first == "CHARGES") // 金额
						{
							charges = atof((vecIter->second).c_str());
						}
						else if(vecIter->first == "PRICE") // 单价
						{
							price = atof((vecIter->second).c_str());
						}
						else if(vecIter->first == "ITEM_TYPE") // 费用类别
						{
							fylbz = vecIter->second;
						}
						else if(vecIter->first == "ITEM_NO") // 项目编号
						{
							itemNo = vecIter->second;
						}
					}
					string itemType = fylbz; // 费用类别值备份
					// ===================费用类别解析===================
					if(!m_ybjzk.empty() && m_ybjzk!="" && m_ybjzk!="CHARGE_SPECIAL_ITEM_DICT")
					{
						string::size_type index = m_jzkfycs.find(fylbz+":");
						if(fylbz!="" && index!=string::npos)
						{
							string::size_type index2 = m_jzkfycs.find(";", index);
							// 获取用于判断的参数段,示例：从 .10:甲类,0;0.00:乙类,0;-1:自费,1; 中截取 .10:甲类,0
							string checkStr = m_jzkfycs.substr(index, index2-index);
							// 获取对应的类别信息
							index = checkStr.find(":", 0);
							index2 = checkStr.find(",", index);
							if(index!=string::npos && index2!=string::npos)
								fylbz = checkStr.substr(index+1, index2-index-1);
							else
								AfxMessageBox(_T("自费知情同意书配置参数出错！"));
						}
						else // 搜索项目费用类别其他的情况对应参数
						{
							index = m_jzkfycs.find("-1");
							string::size_type index2 = m_jzkfycs.find(";", index);
							// 获取用于判断的参数段,示例：从 .10:甲类,0;0.00:乙类,0;-1:自费,1; 中截取 .10:甲类,0			
							if(index!=string::npos && index2!=string::npos)
							{
								string checkStr = m_jzkfycs.substr(index, index2-index);
								// 获取对应的类别信息
								index = checkStr.find(":", 0);
								index2 = checkStr.find(",", index);
								if(index!=string::npos && index2!=string::npos)
									fylbz = checkStr.substr(index+1, index2-index-1);
								else
									AfxMessageBox(_T("自费知情同意书配置参数出错！"));
							}
							else
								AfxMessageBox(_T("自费知情同意书配置参数出错！"));
						}
					}
					else 
					{
						fylbz = "自费";
					}
					// ===================数量和单价计数===================
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

					// ===================添加到容器中===================
					pair<string, string> pairVal = make_pair<string,string>(itemClassStr+classStr, dateStr+"#1#"+itemStr+"#2#"+itemCodeStr+"#3#"+itemClassStr+"#4#"+printDate+"#5#"+units+"#6#"+string(costStr)+"#7#"+fylbz+"#8#"+itemType+"#9#"+numStr+"#10#"+itemNo+"#11#"); // 单位+单价+费用类别文字+费用类别值+数量
					m_itemMap.insert(pair<int, pair<string, string> >(col, pairVal));
					col++;
				}
			}
			else // 未找到记录
			{
				AfxMessageBox(_T("未查询到此项目信息！"));
			}
		}
		
	}
	// 未找到记录
	else
	{
		AfxMessageBox(_T("未查询到此项目信息！"));
	}

	UpdateData(false);
}


void CPrintSelfConsentDlg::OnBnClickedPrint()
{
	UpdateData(true);
	// 获取选中项目 类型-项目信息字符串
	map<string, vector<string> > itemMap;
	map<int, pair<string, string> >::iterator iter;

	//检测打印条件是否满足
	if(m_patientId == "")
	{
		AfxMessageBox(_T("患者id不可为空！"));
		return;
	}
	else if(m_list.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("请选择打印项目！"));
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
			AfxMessageBox(_T("请选择打印项目！"));
			return;
		}
	}
	
	// 循环获取打印项目
	string itemCodeStr="", numsStr="", printDate="", dateStr="", itemNo=""; 
	double nums = 0;
	// 存储已打印项目容器
	while(!m_isAllowOpt); // 等待操作
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
				// 获取医嘱日期
				string::size_type index0 = iter->second.second.find("#1#");
				dateStr = iter->second.second.substr(0, index0);
				// 获取item_code,更新对应的数目统计
				index0 = iter->second.second.find("#2#");
				string::size_type index = iter->second.second.find("#3#", index0);
				if(index != string::npos)
					itemCodeStr = iter->second.second.substr(index0+3, index-index0-3);
				// 获取打印日期
				index0 = iter->second.second.find("#4#", index);
				index = iter->second.second.find("#5#", index0);
				if(index != string::npos)
					printDate = iter->second.second.substr(index0+3, index-index0-3);
				// 获取数量
				index0 = iter->second.second.find("#9#", index);
				index = iter->second.second.find("#10#", index0);
				if(index != string::npos)
				{
					numsStr = iter->second.second.substr(index0+3, index-index0-3);
					nums = atof(numsStr.c_str());
				}
				// 获取item_no
				index0 = iter->second.second.find("#10#", index);
				index = iter->second.second.find("#11#", index0);
				if(index != string::npos)
					itemNo = iter->second.second.substr(index0+4, index-index0-4);
				// 更新计数
				m_itemAmountMap.insert(pair<string, double>(itemCodeStr+","+itemNo, nums));
				map<string, double>::iterator amountIter = m_itemAmountMap.find(itemCodeStr);
				// 已存在
				if(amountIter != m_itemAmountMap.end())
				{
					amountIter->second += nums;
				}
				else
				{
					m_itemAmountMap.insert(pair<string, double>(itemCodeStr, nums));
					// 插入待打印容器中
					// 是否已存在类型名键
					map<string, vector<string> >::iterator tmpIter = itemMap.find(iter->second.first);
					// 若存在则插入vector中
					if(tmpIter != itemMap.end())
						tmpIter->second.push_back(iter->second.second);
					else // 不存在则新建
					{
						vector<string> tmpVec;
						tmpVec.push_back(iter->second.second);
						pair<string, vector<string> > tmpPair = make_pair<string, vector<string> >(iter->second.first, tmpVec);
						itemMap.insert(tmpPair);
					}
				}
				// 存入待写入数据库的容器中
				while(!m_isAllowOpt); // 等待操作
			
				// 查看该记录是否已插入容器
				map<string, string>::iterator recordIter = m_itemRecordMap.find(itemCodeStr+","+itemNo);
				if(recordIter == m_itemRecordMap.end())
					m_itemRecordMap.insert(pair<string, string>(itemCodeStr+","+itemNo, iter->second.second));
				else if(printDate != "")// 存在则检查日期更新为最近打印的
				{
					string recordPrintDate = "", recordDateStr = "";
					string::size_type index0 = recordIter->second.find("#1#");
					if(index0 != string::npos)
						recordDateStr = recordIter->second.substr(0, index0);
					string::size_type index1 = recordIter->second.find("#4#");
					string::size_type index2 = iter->second.second.find("#5#", index1);
					if(index2 != string::npos)
						recordPrintDate = recordIter->second.substr(index1+3, index2-index1-3);
					// 如果打印日期比已存储新则替换
					if(dateStr==recordDateStr && printDate>recordPrintDate)
						recordIter->second = iter->second.second;
				}
			}
		}
	}

	// 获取打印类接口进行打印操作
	try
	{
		CMyPrinter printer;
		printer.ReadHtmlTemplate(PRINT_SELF_CHARGE_CONSENT_LETTER);
		printer.ProducePrintContent(PRINT_SELF_CHARGE_CONSENT_LETTER, m_infoMap, itemMap, m_itemAmountMap);
		printer.ProduceHtmlFile();

		// 打印项目、数量、显示列表清空
		m_itemMap.clear();
		m_list.DeleteAllItems();

		// 多线程将记录写入数据库
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
	// 检查id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("患者id不可为空！"));
		return;
	}
	
	char patientIdStr[0x100];
	::wsprintfA(patientIdStr, "%ls", (LPCTSTR)m_patientId);

	// 根据id查询基本信息
	RESULT_MAP basicInfoDbResult;
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", string(patientIdStr)));

	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询病人基本信息
		CString ss = _T("");
		
		if(m_visitId == _T(""))
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_NO_VISIT_ID", param);
		else
		{
			char visitIdStr[0x100];
			::wsprintfA(visitIdStr, "%ls", (LPCTSTR)m_visitId);
			//AfxMessageBox(m_visitId+_T(":获取的visitId"));
			param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_VISIT_ID", param);
		}
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();

	// 解析基本信息
	// 获取查询到的item_name和item_code
	RESULT_MAP::iterator iter = basicInfoDbResult.find("basicInfo");
	if(iter != basicInfoDbResult.end())
	{
		// 找到项目，获取对应变量
		// 将基本信息插入到容器中,并赋予对应的显示容器中
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
					if(vecIter->first == "NAME") // 姓名
					{
						GetDlgItem(IDC_NAME)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("name", vecIter->second));
					}
					else if(vecIter->first == "SEX") // 性别
					{
						GetDlgItem(IDC_SEX)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("sex", vecIter->second));
					}
					else if(vecIter->first == "CHARGE_TYPE") // 费别
					{
						m_chargeType = vecIter->second;
						GetDlgItem(IDC_CHARGETYPE)->SetWindowTextW(CString(m_chargeType.c_str()));
						m_infoMap.insert(make_pair<string, string>("chargeType", m_chargeType));
					}
					else if(vecIter->first == "DIAGNOSIS") // 诊断
					{
						GetDlgItem(IDC_DIAGNOSE)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("diagenosis", vecIter->second));
					}
					else if(vecIter->first == "DEPT_NAME") // 科室
					{
						GetDlgItem(IDC_DEPARTMENT)->SetWindowTextW(CString(vecIter->second.c_str()));
						m_infoMap.insert(make_pair<string, string>("department", vecIter->second));
					}
					else if(vecIter->first == "ADMISSION_DATE_TIME") // 住院日期
					{
						string date = vecIter->second;
						string::size_type index = date.find(" ");
						if(index != string::npos)
							date = date.substr(0, index);
						GetDlgItem(IDC_START_IN_HOSPITAL)->SetWindowTextW(CString(date.c_str()));
					}
					else if(vecIter->first == "DISCHARGE_DATE_TIME") // 出院日期
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
						// 测试
						//AfxMessageBox(CString(("获取待查询visitId:"+vecIter->second).c_str()));
					}
				}
			}
		}
		else // 未找到记录
		{
			AfxMessageBox(_T("未查询到该患者信息！"));
			return;
		}
		
	}
	// 未找到记录
	else
	{
		AfxMessageBox(_T("未查询到该患者信息！"));
		return;
	}

	// 获取查询到的所有visit_id
	iter = basicInfoDbResult.find("visitInfo");
	m_visitIdCtr.ResetContent();
	if(iter != basicInfoDbResult.end())
	{
		// 找到项目，获取对应变量
		// 将基本信息插入到容器中,并赋予对应的显示容器中
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "VISIT_ID") // 住院次序
					{
						m_visitIdCtr.AddString(CString(vecIter->second.c_str()));
					}
				
				}
			}
		}
		else // 未找到记录
		{
			AfxMessageBox(_T("未查询到该患者信息！"));
			return;
		}
		
	}
	// 未找到记录
	else
	{
		AfxMessageBox(_T("未查询到该患者信息！"));
		return;
	}
	
	// 查询项目前初始化
	if(Init())
		m_list.DeleteAllItems();

	UpdateData(false);
	
	// 设置全选
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALL);
	pBtn->SetCheck(1);
	OnBnClickedCheckAll();

}

// 排序函数
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{

	// 从参数中提取所需比较lc的两行数据
	int row1 = (int) lParam1;
	int row2 = (int) lParam2;

	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1, g_sortColumn);
	CString lp2 = lc->GetItemText(row2, g_sortColumn);

	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	// 文字型比较
	if (g_method[g_sortColumn-2])
		return lp1.CompareNoCase(lp2);
	else
		return lp2.CompareNoCase(lp1);

	return 0;
}

void CPrintSelfConsentDlg::OnColumnclickItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// 点击全选
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
		// 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他比较函数的第一二个参数
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
		AfxMessageBox(_T("患者ID和住院号不可为空，请先查询到患者信息后再添加项目！"));
		return;
	}
	
	// 显示添加项目窗口
	CAddItemDlg dlg;
	dlg.Init(m_patientId, m_visitId, m_chargeType);
	dlg.DoModal();
	// 获取添加的项目
	pair<string, string> addItem = dlg.GetAddItem();
	// 获得itemCode
	string itemCode = dlg.GetItemCode();

	// 如果项目名称不为空，添加到待打印容器中去，并显示出来
	if(addItem.second != "")
	{		
		UpdateData(true);
		int index = m_list.GetItemCount();

		// 添加到列表中显示
		string itemStr = addItem.second.substr(3);
		string::size_type index0 = itemStr.find("#2#", 0);
		if(index0 != string::npos)
			itemStr = itemStr.substr(0, index0);
		else
		{
			AfxMessageBox(_T("添加项目失败！"));
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
		
		// 添加到容器中
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
	// 根据id查询基本信息
	RESULT_MAP basicInfoDbResult;
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", string(patientIdStr)));

	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询病人基本信息
		CString ss = _T("");
		
		if(m_visitId == _T(""))
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_NO_VISIT_ID", param);
		else
		{
			char visitIdStr[0x100];
			::wsprintfA(visitIdStr, "%ls", (LPCTSTR)m_visitId);
			//AfxMessageBox(m_visitId+_T(":获取的visitId"));
			param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
			basicInfoDbResult = dao.Query("SELF_CHARGE_CONSENT_BASIC_INFO_BY_VISIT_ID", param);
		}
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();
	
	// 获取查询到的所有visit_id
	RESULT_MAP::iterator iter = basicInfoDbResult.find("visitInfo");
	m_visitIdCtr.ResetContent();
	string visitId = "";
	if(iter != basicInfoDbResult.end())
	{
		// 找到项目，获取对应变量
		// 将基本信息插入到容器中,并赋予对应的显示容器中
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "VISIT_ID") // 住院次序
					{
						visitId = vecIter->second;
						m_visitIdCtr.AddString(CString(visitId.c_str()));
					}
				
				}
			}
			m_visitIdCtr.SetWindowTextW(CString(visitId.c_str()));
		}
		else // 未找到记录
		{
			AfxMessageBox(_T("未查询到该患者信息！"));
			return;
		}
		
	}
	// 未找到记录
	else
	{
		AfxMessageBox(_T("未查询到该患者信息！"));
		return;
	}
}
