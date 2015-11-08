// DailyListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MIES.h"
#include "DailyListDlg.h"
#include "MyPrinter.h"
#include "afxdialogex.h"


// CDailyListDlg 对话框

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


// CDailyListDlg 消息处理程序

BOOL CDailyListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置列表风格
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT); 
	// 设置列表标题
	m_list.InsertColumn(0, _T("类别"), LVCFMT_CENTER, 120, 60);
	m_list.InsertColumn(1, _T("编码/规格"), LVCFMT_CENTER, 150, 60);
	m_list.InsertColumn(2, _T("名称"), LVCFMT_CENTER, 400, 60);
	m_list.InsertColumn(3, _T("数量"), LVCFMT_CENTER, 60, 60);
	m_list.InsertColumn(4, _T("单价"), LVCFMT_CENTER, 80, 60);
	m_list.InsertColumn(5, _T("金额"), LVCFMT_CENTER, 80, 60);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDailyListDlg::OnBnClickedQueryPatientInfo()
{
	UpdateData(true);
	// 根据id和选中的选项进行数据库查询，获取待打印项目
	// 检查id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("患者id不可为空！"));
		return;
	}
	// 根据id查询基本信息
	GetDlgItem(IDC_NAME)->SetWindowTextW(_T("程明中"));	
	GetDlgItem(IDC_BED_NUM)->SetWindowTextW(_T("6"));	
	GetDlgItem(IDC_START_DATE)->SetWindowTextW(_T("2014/07/18"));
	GetDlgItem(IDC_CHARGE)->SetWindowTextW(_T("1700.37"));
	GetDlgItem(IDC_DEPARTMENT2)->SetWindowTextW(_T("眼科"));
	GetDlgItem(IDC_HOSPITAL_DAYS)->SetWindowTextW(_T("5天"));

	// 将基本信息插入到容器中
	m_infoMap.clear();
	m_infoMap.insert(make_pair<string, string>("name", "程明中"));
	m_infoMap.insert(make_pair<string, string>("bedNum", "6"));
	m_infoMap.insert(make_pair<string, string>("startDate", "2014/07/18"));
	m_infoMap.insert(make_pair<string, string>("department", "眼科"));
	m_infoMap.insert(make_pair<string, string>("id", "30323590"));
	m_infoMap.insert(make_pair<string, string>("hospitalDays", "5"));
	m_infoMap.insert(make_pair<string, string>("allCharge", "1700.37"));
	m_infoMap.insert(make_pair<string, string>("allDayCharge", "1700.37"));
	m_infoMap.insert(make_pair<string, string>("printer", "邓婷"));
	m_infoMap.insert(make_pair<string, string>("chargeDate", "2014/07/18"));
	m_infoMap.insert(make_pair<string, string>("printDate", "2014/08/01"));

	// 显示项目
	m_list.DeleteAllItems();
	m_list.InsertItem(0,  _T("甲类"));   
    m_list.SetItemText(0, 1, _T("XYZ02GNAI2\n氯化钠100g"));   
    m_list.SetItemText(0, 2, _T("氯化钠0"));
	m_list.SetItemText(0, 3, _T("1.00"));
	m_list.SetItemText(0, 4, _T("6.00"));
	m_list.SetItemText(0, 5, _T("6.00"));

	m_list.InsertItem(1,  _T("乙类"));   
    m_list.SetItemText(1, 1, _T("XYZ02GNAI2\n氯化钠100g"));   
    m_list.SetItemText(1, 2, _T("氯化钠1"));
	m_list.SetItemText(1, 3, _T("1.00"));
	m_list.SetItemText(1, 4, _T("6.00"));
	m_list.SetItemText(1, 5, _T("1.00"));

	m_list.InsertItem(2,  _T("不可报销"));   
    m_list.SetItemText(2, 1, _T("XYZ02GNAI2(氯化钠100g)"));   
    m_list.SetItemText(2, 2, _T("氯化钠2"));
	m_list.SetItemText(2, 3, _T("1.00"));
	m_list.SetItemText(2, 4, _T("6.00"));
	m_list.SetItemText(2, 5, _T("6.00"));

	m_list.InsertItem(3,  _T("药品费小计"));   
    m_list.SetItemText(3, 1, _T("1001.37")); 

	// 将查询到的项目存入容器中
	m_itemMap.clear();
	vector<string> tmpVec;
	tmpVec.push_back("甲类##XYZ02GNAI2##氯化钠0##氯化钠100g##1.00##6.00##6.00##");
	pair<string, vector<string> > pairVal0 = make_pair<string, vector<string> >("药品:1001.37", tmpVec);
	pair<string, vector<string> > pairVal1 = make_pair<string, vector<string> >("材料:1001.37", tmpVec);
	pair<string, vector<string> > pairVal2 = make_pair<string, vector<string> >("检查:1001.37", tmpVec);
	m_itemMap.insert(pairVal0);
	m_itemMap.insert(pairVal1);
	m_itemMap.insert(pairVal2);

	UpdateData(false);
}


void CDailyListDlg::OnBnClickedOk()
{
	UpdateData(true);
	// 根据id和选中的选项进行数据库查询，获取待打印项目
	// 检查id
	if(m_patientId == "")
	{
		AfxMessageBox(_T("患者id不可为空！"));
		return;
	}

	// 获取打印类接口进行打印操作
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


