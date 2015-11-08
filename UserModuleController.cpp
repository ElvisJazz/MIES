///////////////////////////////////////////////////////////////////////
//文件名： UserModuleController.cpp
//功  能： 医保审查模块开关类实现
//开发者： 贾治中
//创  建：2014/8/5
//修  改：2014/8/5
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "UserModuleController.h"
#include <fstream>
#include <string>
#include <map>
#include "sql_dao.h"

using namespace std;


CString CUserModuleController::s_writeDate = _T("");
int CUserModuleController::s_patientInfoModuleValue = 0;
int CUserModuleController::s_presettlementModuleValue = 0;
int CUserModuleController::s_medicareInfoModuleValue = 0;
int CUserModuleController::s_treatmentTipModuleValue = 0;
int CUserModuleController::s_seriousTipModuleValue = 0;

CUserModuleController::CUserModuleController(void)
{
}


CUserModuleController::~CUserModuleController(void)
{
}

// 功能：查看指定模块开关是否打开
// 参数：模块名对应值
// 返回：空
bool CUserModuleController::IsModuleOpen(int module)  
{
	// 获取当前时间
	CTime currentTime = CTime::GetCurrentTime();
	CString szTime = currentTime.Format("%Y-%m-%d");

	// 写入日期变量为空，先去读本地开关参数文件,获取本地参数写入日期
	/*if(s_writeDate == _T(""))
	{
		ReadFile();
	}*/
	
	// 写入日期与当前日期不一致，读取数据库更新本地开关文件
	//if(s_writeDate != szTime)
	//{
		GetDbInfoIntoFile();
	//}

	switch(module)
	{
	case PATIENT_INFO_MODULE:	return s_patientInfoModuleValue;
	case PRESETTLEMENT_MODULE:	return s_presettlementModuleValue;
	case MEDICARE_INFO_MODULE:	return s_medicareInfoModuleValue;
	case TREATMENT_TIP_MODULE:	return s_treatmentTipModuleValue;
	case SERIOUS_TIP_MODULE:	return s_seriousTipModuleValue;
	default: return false;
	}

}

// 功能：读取文件并获取相应开关参量
// 参数：空
// 返回：空
void CUserModuleController::ReadFile()			  
{
	ifstream read("moduleCommand.ini", ios::in);
	// 文件不存在
	if(!read)
		return;
	// 获取读取日期和其他模块参量	
	string writeDate, name, value;
	map<string, string> parameterMap;
	if(!read.eof())
		read>>writeDate;
	else
		return;
	
	while(!read.eof())
	{
		read>>name;
		read>>value;
		parameterMap.insert(pair<string, string>(name, value));
	}
	
	map<string, string>::iterator iter = parameterMap.begin();
	for(; iter!=parameterMap.end(); ++iter)
	{
		if(iter->first == "TSL_MODULE_COMMAND")
			s_patientInfoModuleValue = atoi(iter->second.c_str());
		else if(iter->first == "XSL_MODULE_COMMAND")
			s_presettlementModuleValue = atoi(iter->second.c_str());
		else if(iter->first == "YBBL_MODULE_COMMAND")
			s_medicareInfoModuleValue = atoi(iter->second.c_str());
		else if(iter->first == "YYTS_MODULE_COMMAND")
			s_treatmentTipModuleValue = atoi(iter->second.c_str());

	}
	s_writeDate = CString(writeDate.c_str());
	
}

// 功能：将读取的数据库信息写入文件并获取相应开关参量
// 参数：空
// 返回：空
void CUserModuleController::GetDbInfoIntoFile()	  
{
	/*ofstream output("moduleCommand.ini", ios::out);
	if(!output)
	{
		AfxMessageBox(_T("写入文件失败！"));
	}
*/
	map<string,string> param = map<string,string>(); 
	// 创建查询对象
	CSqlDao dao;
	RESULT_MAP dbParameter;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf", "MIES_sql_config.xml");
		// 执行查询
		
		// 查找参数表
		dbParameter = dao.Query("USER_COMMAND_Parameter", param);
		
		// 解析参数结果，获得对应的配置参数值
		string ybjzk = "";
		RESULT_MAP::iterator iter = dbParameter.find("parameter");
		while(iter != dbParameter.end())
		{
			// 写入当前时间
			/*CTime currentTime = CTime::GetCurrentTime();
			s_writeDate = currentTime.Format("%Y-%m-%d");
			char dateStr[0x100];
			::wsprintfA(dateStr, "%ls", (LPCTSTR)s_writeDate);
			output<<string(dateStr)<<endl;
*/
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					// 获取提示类的参数值
					if(vecIter->first == "NAME" && vecIter->second == "TSL_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_patientInfoModuleValue = atoi(vecIter->second.c_str());
								//output<<"TSL_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// 获取显示类的参数值
					else if(vecIter->first == "NAME" && vecIter->second == "XSL_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_presettlementModuleValue = atoi(vecIter->second.c_str());
								//output<<"XSL_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// 获取医保办理的参数值
					else if(vecIter->first == "NAME" && vecIter->second == "YBBL_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_medicareInfoModuleValue = atoi(vecIter->second.c_str());
								//output<<"YBBL_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// 获取用药提示的参数值
					else if(vecIter->first == "NAME" && vecIter->second == "YYTS_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_treatmentTipModuleValue = atoi(vecIter->second.c_str());
								//output<<"YYTS_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// 获取门诊用药提示的参数值
					else if(vecIter->first == "NAME" && vecIter->second == "MZZZ_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_seriousTipModuleValue = atoi(vecIter->second.c_str());
								//output<<"YYTS_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
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
}