///////////////////////////////////////////////////////////////////////
//文件名： MIES.cpp
//功  能： 医保审查系统dll主文件初始化（外部接口函数定义）
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIES.h"
#include "PrintSelfConsentDlg.h"
#include "DailyListDlg.h"
#include "LogicHandler.h"
#include "UserModuleController.h"
#include "sql_dao.h"	// CSqlDao类头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMIESApp

BEGIN_MESSAGE_MAP(CMIESApp, CWinApp)
END_MESSAGE_MAP()


// CMIESApp 构造

CMIESApp::CMIESApp()
{
}


// 唯一的一个 CMIESApp 对象

CMIESApp theApp;

// CMIESApp 初始化

BOOL CMIESApp::InitInstance()
{
	
	
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CWinApp::InitInstance();
	// 修改标题
	free((void*)m_pszAppName);
	CString str=_T("MIES医保审查提示");
	m_pszAppName=(LPCTSTR)malloc((str.GetLength()+1)*sizeof(TCHAR));
	_tcscpy_s((LPTSTR)m_pszAppName, (size_t)str.GetLength()+1, str);
	afxCurrentAppName=m_pszAppName;    

	return TRUE;
}
 
//功  能：字符串消除工具函数
//参  数：目标串，起始标志串，结束标志串
//返回值：无
void replaceStr(char* str,string startStr, string endStr)
{
	string resultStr = string(str);
	string::size_type index1=0, index2=0;
	string subStr = "";
	index1 = resultStr.find(startStr); // 去掉含有0（公费）标志的项目
	if(index2 != string::npos)
	{
		index2 = resultStr.find(endStr, index1);
		if(index2 != string::npos)
		{
			resultStr.replace(index1, index2-index1+1, "");
			strcpy(str, resultStr.c_str());
		}
		else
			strcpy(str,"{state:0}{message:生成结果字符串出错！}");
	}
	else
		strcpy(str,"{state:0}{message:生成结果字符串出错！}");
}

//功  能：护士工作站提示患者农合是否直补接口函数
//参  数：病人id，住院号
//返回值：查询结果
extern "C" __declspec(dllexport) void __cdecl DisplayPatientZBInfo(char* result, char* patient_id, char* visit_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 检测开关
	if(!CUserModuleController::IsModuleOpen(MEDICARE_INFO_MODULE))
	{
		strcpy(result, "{state:1}{message:操作成功}");
		return;
	}

	// 检测参数是否为空
	string str1, str2;
	str1 = string(patient_id);
	str2 = string(visit_id);
	if(str1.empty() || str2.empty())
	{
		AfxMessageBox(_T("患者id或住院号不可为空！"));
		strcpy(result, "{state:0}{message:参数不可为空！}");
		return;
	}

	// =================通过数据库获取查询结果=======================
	RESULT_MAP dbResult, dbParameter;
	// 初始化参数列表
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", str1)); //30323590
	param.insert(pair<string, string>("VISIT_ID", str2)); //6
	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象		
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询

		// 查找参数表
		dbParameter = dao.Query("YBZBParameter", param);
		// 获取直补手续数据
		dbResult = dao.Query("PatientZBInfo",param);
	}
	catch( runtime_error err )
	{
		// 异常处理代码	
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:数据库操作出错！}");
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();
	
	//  =================通过判断逻辑处理查询结果得出显示数据 =================
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandlePatientZBInfoData(result, dbParameter, dbResult);
	
	//  =================显示查询结果窗口定义 =================
	if(displayInfo != "")
	{
		// 读取皮肤
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		
		// 释放皮肤
		skinppExitSkin();
	}
}

//功  能：预结算查询接口函数
//参  数：病人id，住院号
//返回值：查询结果
extern "C" __declspec(dllexport) void __cdecl DisplayPatientPreSettlementInfo(char* result, char*  patient_id, char* visit_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 检测开关
	if(!CUserModuleController::IsModuleOpen(PRESETTLEMENT_MODULE))
	{
		strcpy(result, "{state:1}{message:操作成功}");
		return;
	}

	// 检测参数是否为空
	string str1, str2;
	str1 = string(patient_id);
	str2 = string(visit_id);
	if(str1.empty() || str2.empty())
	{
		AfxMessageBox(_T("患者id或住院号不可为空！"));
		strcpy(result, "{state:0}{message:参数不可为空！}");
		return;
	}

	// =================通过数据库获取查询结果=======================
	RESULT_MAP dbResult, dbResult2, dbResult3, dbParameter;
	// 初始化参数列表
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", str1)); //30323590
	param.insert(pair<string, string>("VISIT_ID", str2)); //6
	// 创建查询对象
	CSqlDao dao;
	// 参数临时变量：预结算查询参数，医保基准库参数
	string yjscx = "";
	string ybjzk = "";

	try
	{
		// 初始化查询对象		
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询

		// ===============查找预结算参数表===============
		dbParameter = dao.Query("YBPreSettlementInfoParameter", param);
		RESULT_MAP::iterator iter = dbParameter.find("parameter");
		if(iter != dbParameter.end())
		{
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					// 获取预结算查询的参数值
					if(vecIter->first == "NAME" && vecIter->second == "YJSCX")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
								yjscx = vecIter->second;
						}
					}
					// 获取医保基准库的参数值
					else if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
								ybjzk = vecIter->second;
						}
					}
				}
			}
		}
		// 找到对应的预结算参数值，在sql_config.xml中存在对应的yjscx值得group查询
		if(!yjscx.empty())
			dbResult = dao.Query(yjscx, param);

		// ===============根据参数列表进行涉及到charge_type关联参数的查询，主要是自费药占比、自费项目比例的计算===============
		
		// 找到对应的医保基准库参数值，在sql_config.xml中存在对应的ybjzk值得group查询
		if(!ybjzk.empty())
			dbResult2 = dao.Query(ybjzk, param);
	}
	catch( runtime_error err )
	{
		// 异常处理代码	
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:数据库操作出错！}");
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();
	//  =================通过判断逻辑处理查询结果得出显示数据 =================
	LogicHandler logicHandler;
	logicHandler.HandlePatientPreSettlementInfoData(result, dbParameter, dbResult, dbResult2);
	//  =================显示查询结果窗口定义 =================
	//CDisplayInfoDlg dlg;
	//// 设置显示信息
	//dlg.SetDisplayInfo(CString(displayInfo.c_str()));
	//dlg.DoModal();
}
 
 
//功  能：提示类查询接口函数
//参  数：病人id，住院号
//返回值：查询结果
extern "C" __declspec(dllexport) void __cdecl DisplayPatientInfo(char* result, char* patient_id, char* visit_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 检测开关
	if(!CUserModuleController::IsModuleOpen(PATIENT_INFO_MODULE))
	{
		strcpy(result, "{state:1}{message:操作成功}");
		return;
	}

	// 检测参数是否为空
	string str1, str2;
	str1 = string(patient_id);
	str2 = string(visit_id);
	if(str1.empty() || str2.empty())
	{
		AfxMessageBox(_T("患者id或住院号不可为空！"));
		strcpy(result, "{state:0}{message:参数不可为空！}");
		return;
	}

	// =================通过数据库获取查询结果=======================
	RESULT_MAP dbParameter, dbResult1;
	// 初始化参数列表
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", str1)); //30323590
	param.insert(pair<string, string>("VISIT_ID", str2)); //6
	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询
		
		// 查找参数表
		dbParameter = dao.Query("YBInfoParameter", param);
		// 获取部分提示类信息
		dbResult1 = dao.Query("PatientInfo",param);
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:数据库操作出错！}");
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();

	// =================通过判断逻辑处理查询结果得出显示数据=================
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandlePatientInfoData(result, dbParameter, dbResult1);
	
	// =================显示查询结果窗口定义=================
	if(displayInfo != "")
	{
		// 读取皮肤
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		
		// 释放皮肤
		skinppExitSkin();
	}
}

//功  能：医生提交时提示需要审批的项目接口函数
//参  数：病人id，住院号，医院项目类别，医院项目编码，医保费别，医院项目规格
//返回值：查询结果
extern "C" __declspec(dllexport) void __cdecl DisplayProjectCheckInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type, char* item_spec)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//// 检测开关
	//if(!CUserModuleController::IsModuleOpen(PATIENT_INFO_MODULE))
	//	return;
	
	// 通过数据库获取查询结果
	map<string, vector<vector<pair<string, string> > > > dbResult = RESULT_MAP();
	
	// 通过判断逻辑处理查询结果得出显示数据
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandleProjectCheckInfoData(result, dbResult);
	
	// 显示查询结果窗口定义
	if(displayInfo != "")
	{
		//Line 1  , load the skin
		//bool is = SkinStartW(_T("skins\\basic_urf\\office2007_af0.urf"),WINDOW_TYPE_VC,_T(""),GTP_LOAD_FILE,NULL,NULL);
		//Enable use URF font settings 
		//SkinWindowSetEx(NULL,SKIN_SET_UNICODE_URF_FONT,1,0,NULL,0,NULL);
		// 读取皮肤
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		
		// 释放皮肤
		skinppExitSkin();
		//Line 2  , cleanup
		//SkinRemove() ;
	}
}

//功  能：医保目录对应相关操作接口函数
//参  数：病人id，住院号，医院项目类别，医院项目编码，医保费别，医院项目规格，是否返回数据(此处应为1)
//返回值：查询结果
extern "C" __declspec(dllexport) void __cdecl DisplayMedicalInsuranceInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type, char* item_spec)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	/*string tmpss = string(patient_id)+" "+string(visit_id)+" "+string(item_code)+" "+string(charge_type)+" "+string(item_class)+" ";
	AfxMessageBox(CString(tmpss.c_str()));
*/
	// 检测开关
	if(!CUserModuleController::IsModuleOpen(TREATMENT_TIP_MODULE))
	{
		strcpy(result, "{state:1}{message:操作成功}");
		return;
	}

	// 判断是否为A,C,D,I类
	string itemClass(item_class);
	if(itemClass != "A" && itemClass != "C" && itemClass != "D" && itemClass != "I")
	{
		//AfxMessageBox(_T("不是ACDI之一，应返回：")+CString(item_class));
		strcpy(result, "{state:1}{message:操作成功！}");
		return;
	}

	//AfxMessageBox(_T("是ACDI之一：")+CString(item_class));
	// =================通过数据库获取查询结果=======================
	RESULT_MAP dbParameter, dbResult;
	// 初始化参数列表
	string chargeType(charge_type);
	string itemCode(item_code);
	string itemSpec(item_spec);

	map<string,string> param;
	param.insert(pair<string, string>("CHARGE_TYPE", chargeType)); 
	param.insert(pair<string, string>("ITEM_CLASS", itemClass)); 
	param.insert(pair<string, string>("ITEM_CODE", itemCode)); 
	param.insert(pair<string, string>("ITEM_SPEC", itemSpec)); 
	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询
		
		// 查找参数表
		dbParameter = dao.Query("YYTSParameter", param);
		
		// 解析参数结果，获得对应基准库的配置参数值
		string ybjzk = "";
		RESULT_MAP::iterator iter = dbParameter.find("parameter");
		while(iter != dbParameter.end())
		{
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					// 获取医保基准库的参数值
					if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
								ybjzk = vecIter->second;
						}
					}
				}
			}
			++iter;
		}
		// 找到对应的医保基准库参数值，在sql_config.xml中存在对应的ybjzk值得group查询
		if(!ybjzk.empty())
			dbResult = dao.Query(ybjzk+"_YYTS", param);
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:数据库操作出错！}");
		// 关闭对象
		dao.Close();
		return;
	}
	// 关闭对象
	dao.Close();

	// =================通过判断逻辑处理查询结果得出显示数据=================
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandleMedicalInsuranceInfoData(result, string(patient_id), string(visit_id), itemCode, dbParameter, dbResult, chargeType, itemClass);
	
	// =================显示查询结果窗口定义=================
	if(displayInfo != "")
	{
		//CDisplayInfoDlg dlg;
		//// 设置显示信息
		//dlg.SetResultInfo(result);
		//dlg.SetDisplayInfo(CString(displayInfo.c_str()));
		//dlg.DoModal();

		//Line 1  , load the skin
		//SkinStart("skins\\basic_urf\\office2007_af.urf",WINDOW_TYPE_VC,"",GTP_LOAD_FILE,NULL,NULL);
		// 读取皮肤
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		string resultStr = string(result);
		if(resultStr.find("state:3") != string::npos)
		{
			int a = AfxMessageBox(CString(displayInfo.c_str()), MB_YESNO|MB_ICONQUESTION);
			// 否替换成自费药品
			if(a == IDNO) 
			{
				//replaceStr(result, "0{", "}"); // 去掉含有0（公费）标志的项目
				//replaceStr(result, "1{","1");
				string tmpStr(result);
				tmpStr.replace(1, 7, "state:6");
				strcpy(result,tmpStr.c_str());
			}
			// 是替换成报销内项目
			else
			{
				//replaceStr(result, "1{", "}"); // 去掉含有1（自费）标志的项目
				//replaceStr(result, "0{","0");
				string tmpStr(result);
				tmpStr.replace(1, 7, "state:5");
				strcpy(result, tmpStr.c_str());
			}
		}
		else
		{
			AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		}
		// 释放皮肤
		skinppExitSkin();

		//Line 2  , cleanup
		//SkinRemove() ;
	}
}

//功  能：门诊医保目录对应相关操作接口函数
//参  数：病人id
//返回值：无
extern "C" __declspec(dllexport) int __stdcall DisplayOutpatientServiceInfo(char* result, char* patient_id, char* item_code, char* zz_code)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 检测开关
	if(!CUserModuleController::IsModuleOpen(SERIOUS_TIP_MODULE))
	{
		strcpy(result, "{state:1}{message:操作成功}");
		return 0;
	}
	// -------------------------------------------药品替换----------------------------------------------------
	// =================通过数据库获取查询结果=======================
	RESULT_MAP dbResult, dbParameter;
	string itemCode(item_code);
	// 初始化参数列表
	/*string itemCodeStr(item_code);

	map<string,string> param;
	param.insert(pair<string, string>("ITEM_CODE", itemCodeStr)); 
	// 创建查询对象
	CSqlDao dao;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询
		
		// 寻找可能的替换药品
		dbResult = dao.Query("MZ_REPLACE_ITEM", param);
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:数据库操作出错！}");
		// 关闭对象
		dao.Close();
		return 0;
	}
	// 关闭对象
	dao.Close();

	// 解析参数结果，获得对应基准库的配置参数值
	string itemCode(item_code), tmpCode = "", tmpType = "";
	string itemCode1(item_code); // 原始item_code
	RESULT_MAP::iterator iter = dbResult.find("replace_item");
	while(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// 获取item_code
				if(vecIter->first == "ITEM_CODE")
				{
					tmpCode = vecIter->second;
				}
				// 获取type
				else if(vecIter->first == "ZFBL")
				{
					tmpType = vecIter->second;
				}
			}
			if(atof(tmpType.c_str()) != 1.00) // 此为可替换项目
				itemCode = tmpCode;
		}
		++iter;
	}
*/
	string displayInfo = "";
	// 重症代码为空，则返回
	if(string(zz_code) == "")
	{
		// 若没找到可替换项目
		if(itemCode == string(item_code))
		{
			strcpy(result, "{state:1}{message:操作成功！}");
			// 查询是否为自费
			map<string,string> param0; 
			param0.insert(pair<string, string>("ITEM_CODE", itemCode));
			// 创建查询对象
			CSqlDao dao0;
			try
			{
				// 初始化查询对象
				dao0.Init("MIES_db_config.conf","MIES_sql_config.xml");
				// 执行查询
		
				// 查找参数表
				RESULT_MAP dbResult0 = dao0.Query("MZFYCX", param0);

				RESULT_MAP::iterator iter = dbResult0.find("FYLX");
				double zfbl = 0;
				if(iter != dbResult0.end())
				{
					RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
					for(;vecVecIter != iter->second.end(); ++vecVecIter)
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "ZFBL")
							{
								zfbl = atof(vecIter->second.c_str());
							}
						}
					}
				}
				if(zfbl == 1.00)
					displayInfo = "该项目类别为：自费";
			}
			catch( runtime_error err )
			{
				// 异常处理代码
				AfxMessageBox(CString(err.what()));
				strcpy(result, "{state:0}{message:数据库操作出错！}");
				// 关闭对象
				dao0.Close();
				return 0;
			}
			// 关闭对象
			dao0.Close();
		}
		else	
		{
			strcpy(result, string("{state:1}{message:操作成功！}{itemCode:"+itemCode+"}").c_str());
			return 0;
		}
	}
	else
	{
		// -------------------------------------------重症审查-----------------------------------------------------
		// =================通过数据库获取查询结果=======================
		// 初始化参数列表
		map<string,string> param1;
		param1.insert(pair<string, string>("PATIENT_ID", string(patient_id))); 
		param1.insert(pair<string, string>("ITEM_CODE", itemCode));
		// 创建查询对象
		CSqlDao dao1;
		// 限制天数
		string limitDays = "";
		try
		{
			// 初始化查询对象
			dao1.Init("MIES_db_config.conf","MIES_sql_config.xml");
			// 执行查询
		
			// 查找参数表
			dbParameter = dao1.Query("MZYYParameter", param1);
			// 解析参数结果，获得对应基准库的配置参数值
			RESULT_MAP::iterator iter = dbParameter.find("parameter");
			while(iter != dbParameter.end())
			{
				RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
				for(;vecVecIter != iter->second.end(); ++vecVecIter)
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						// 获取门诊重症开药天数的参数值（15）
						if(vecIter->first == "NAME" && vecIter->second == "MZZZDAYS")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									limitDays = vecIter->second;
							}
						}
					}
				}
				++iter;
			}

			if(!limitDays.empty())
			{
				char limitDateStr[256];
				CTime limitDate = CTime::GetCurrentTime() - CTimeSpan(atoi(limitDays.c_str()), 0, 0, 0);
				::wsprintfA(limitDateStr, "%ls", (LPCTSTR)limitDate.Format("%Y-%m-%d"));
				param1.insert(pair<string, string>("LIMIT_DATE", string(limitDateStr)));
				param1.insert(pair<string, string>("ITEM_CODE1", itemCode));
				dbResult = dao1.Query("MZYYTS", param1);
			}
		
		}
		catch( runtime_error err )
		{
			// 异常处理代码
			AfxMessageBox(CString(err.what()));
			strcpy(result, "{state:0}{message:数据库操作出错！}");
			// 关闭对象
			dao1.Close();
			return 0;
		}
		// 关闭对象
		dao1.Close();

		// =================通过判断逻辑处理查询结果得出显示数据=================
		LogicHandler logicHandler;
		displayInfo = logicHandler.HandleOutpatientServiceData(result, string(zz_code), limitDays, dbResult);
		// 若找到可替换项目
		if(itemCode != string(item_code))
		{
			string resultStr(result);
			resultStr += "{itemCode:"+itemCode+"}";
			strcpy(result, resultStr.c_str());
		}
	}
	// =================显示查询结果窗口定义=================
	if(displayInfo != "")
	{
		//Line 1  , load the skin
		//SkinStart("skins\\basic_urf\\office2007_af.urf",WINDOW_TYPE_VC,"",GTP_LOAD_FILE,NULL,NULL);
		// 读取皮肤
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		string resultStr = string(result);
		if(resultStr.find("state:3") != string::npos)
		{
			int a = AfxMessageBox(CString(displayInfo.c_str()), MB_YESNO|MB_ICONQUESTION);
			// 否：不符合限制性条件，不允许开
			if(a == IDNO) 
			{
				replaceStr(result, "{state:3", "}"); // 去掉含有3标志的项目
				string tmpStr(result);
				tmpStr = "{state:2}" + tmpStr;
				strcpy(result, tmpStr.c_str());
			}
			// 是：允许开
			else
			{
				replaceStr(result, "{state:3", "}"); // 去掉含有3标志的项目
				string tmpStr(result);
				tmpStr = "{state:1}" + tmpStr;
				strcpy(result, tmpStr.c_str());
			}
		}
		else
		{
			AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		}
		//Line 2  , cleanup
		//SkinRemove() ;
		// 释放皮肤
		skinppExitSkin();
	}
	
	return 0;
}

//功  能：打印自费知情同意书接口函数
//参  数：病人id
//返回值：无
extern "C" __declspec(dllexport) void __cdecl PrintSelfChargeConsentLetter(char* result, char* patient_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//Line 1  , load the skin
	//SkinStartW(_T("skins\\basic_urf\\bluepinna.urf"),WINDOW_TYPE_VC,_T(""),GTP_LOAD_FILE,NULL,NULL);
	//SkinWindowSetEx(NULL,SKIN_SET_UNICODE_URF_FONT,1,0,NULL,0,NULL);

	// 打开打印窗口
	// 读取皮肤
	ifstream readSkin("skinConfig.ini", ios::in);
	string skin;
	readSkin>>skin;
	readSkin.close();
	skinppLoadSkin(const_cast<char*>(skin.c_str()));
	
	// 设置显示信息
	CPrintSelfConsentDlg dlg; 
	dlg.m_patientId = CString(patient_id);
	if(dlg.Init())
	{
		dlg.DoModal();
	}
	// 释放皮肤
	skinppExitSkin();
	
}

//功  能：打印每日清单接口函数
//参  数：病人id
//返回值：无
extern "C" __declspec(dllexport) void __cdecl PrintDailyList(char* patient_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDailyListDlg dlg;
	dlg.m_patientId = CString(patient_id);
	// 设置显示信息
	dlg.DoModal();
}


