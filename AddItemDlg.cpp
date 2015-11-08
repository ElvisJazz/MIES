///////////////////////////////////////////////////////////////////////
//文件名： AddItemDlg.cpp
//功  能： 添加项目类实现（用于打印自费知情同意书）
//开发者： 贾治中
//创  建：2014/7/27
//修  改：2014/7/27
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIES.h"
#include "AddItemDlg.h"
#include "sql_dao.h"
#include "afxdialogex.h"
#include <sstream>

using namespace std;

// 线程函数
UINT ThreadFunc(LPVOID lpParam)
{
	if(lpParam != NULL)
    {
		CAddItemDlg* pDlg=(CAddItemDlg*)lpParam;
		pDlg->QueryItem();
	}
    return 0;
}

// CAddItemDlg 对话框

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


// CAddItemDlg 消息处理程序

// 初始化函数
void CAddItemDlg::Init(CString patientId, CString visitId, string chargeType)
{
	m_chargeType = chargeType;
	RESULT_MAP dbParameter;
	// 初始化参数列表
	map<string,string> param;
	char patientIdStr[256];
	::wsprintfA(patientIdStr, "%ls", (LPCTSTR)patientId);
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
		
		char visitIdStr[256];
		::wsprintfA(visitIdStr, "%ls", (LPCTSTR)visitId);
		param.insert(pair<string, string>("VISIT_ID", string(visitIdStr)));
		dbParameter = dao.Query("ZFZQTYSParameter_VISITID", param);

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
					// 获取对应基准库的参数值
					if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
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
		return;
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
	if(ybjzk=="" || jzkfycs=="")
	{
		AfxMessageBox(_T("未查到对应基准库参数，请检查配置表是否有问题！"));
		return;
	}
	m_ybjzk = ybjzk;
	m_jzkfycs = jzkfycs + ";";
}

void CAddItemDlg::OnBnClickedOk()
{
	UpdateData(true);

	if(m_amount < 1)
		AfxMessageBox(_T("请输入有效数量！"));
	string className = "";
	// 将CString转成string
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
			throw runtime_error("项目获取出问题！");

		// 重新恢复itemName的值
		string::size_type index1 = itemNameStr.find("【单位】");
		if(index1 != string::npos)
			itemNameStr = itemNameStr.substr(0, index1);
		else
			throw runtime_error("未解析到项目单位！");
		// 获取className;
		index1 = valueStr.find("#2#");
		if(index1 != string::npos)
		{
			className = valueStr.substr(0, index1);
			valueStr.replace(0, index1, "#1#"+itemNameStr);
		}
		else
			throw runtime_error("项目获取出问题！");

		// 更新添加数目字段
		char amountStr[10];
		sprintf(amountStr, "%d", m_amount);
		m_addItem = make_pair<string,string>(className+"类", valueStr+string(amountStr)+"#10##11#");

		if(m_item != _T(""))
			CDialogEx::OnOK();
		else
			throw runtime_error("未选择任何项目！");
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
	// 查询项目
	AfxBeginThread(ThreadFunc, this);    //创建MFC线程
	// 拉开下拉选择框
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
	// 拉开下拉选择框
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
	// 拉开下拉选择框
	UpdateData(false);
	m_comboCtr.ShowDropDown();
	m_comboCtr.SetFocus();
	::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
	UpdateData(false);
}

//功 能:数据库查询项目
//输 入:无
//输 出:是否查到项目
bool CAddItemDlg::QueryItem()
{
	// 是否查到空数据
	bool isNullItem = false;
	// 查询数据库获取条目
	char codeStr[0x100];
	::wsprintfA(codeStr, "%ls", (LPCTSTR)m_itemName);
	RESULT_MAP dbResult;
	// 初始化参数列表
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
	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询
		
		// 查找参数表
		
		dbResult = dao.Query(m_ybjzk+"_ITEM_QUERY", param);
		
		// 解析参数结果，获得对应基准库的配置参数值
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
					else if(vecIter->first == "ITEM_CODE") // 项目码
					{
						itemCode = vecIter->second;
					}
					else if(vecIter->first == "ITEM_CLASS") // 类别码
					{
						itemClass = vecIter->second;
					}
					else if(vecIter->first == "UNITS") // 单位
					{
						units = vecIter->second;
					}
					else if(vecIter->first == "AMOUNT") // 数量
					{
						string numsStr = vecIter->second;
						nums = atoi(numsStr.c_str());
					}
					else if(vecIter->first == "PRICE") // 单价
					{
						price = vecIter->second;
					}
					else if(vecIter->first == "ITEM_TYPE") // 费用类别值
					{
						fylbz = vecIter->second;
					}
				}
				string itemType = fylbz; // 费用类别值备份
				itemName += ("【单位】"+units);
				m_comboCtr.AddString(CString(itemName.c_str()));
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
				// 添加到容器中
				m_itemClassMap.insert(make_pair<string, string>(itemName, itemClass+className+"#2#"+itemCode+"#3#"+itemClass+"#4##5#"+units+"#6#"+price+"#7#"+fylbz+"#8#"+itemType+"#9#")); // 单位+单价+费用类别
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
	return !isNullItem;
}

void CAddItemDlg::OnEnKillfocusEdit1()
{
	// 拉开下拉选择框
	m_comboCtr.ShowDropDown();
	m_comboCtr.SetFocus();
}
