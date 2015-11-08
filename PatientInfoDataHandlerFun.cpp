///////////////////////////////////////////////////////////////////////
//文件名： LogicHandler.cpp
//功  能： 医保审查系统数据库获取数据逻辑处理类实现文件
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//功  能：处理提示类查询的数据库数据
//参  数：处理后的结果字符串指针，数据库查询数据
//返回值：窗口显示信息
string LogicHandler::HandlePatientInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult)
{
	string displayInfo = "";
	// 返回结果数据和显示数据
	sprintf(result, "{state:1}{message:操作成功}");

	// ===================获取并设置数据库阈值配置参数===================
	// 解析参数结果，获得对应基准库的配置参数值
	
	int charge_type_check_hour = 0; // 费别变更时间参数
	string ZB_str = ""; // 办理医保（直补）参数
	string ybjzk = ""; // 医保基准库

	RESULT_MAP::iterator iter = dbParameter.find("parameter");
	if(iter != dbParameter.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// 获取对应的参数值
				if(vecIter->first == "NAME" && vecIter->second == "FBBGJCSJ")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							charge_type_check_hour = atoi(vecIter->second.c_str());
					}
				}
				else if(vecIter->first == "NAME" && vecIter->second == "YBBL")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							ZB_str = vecIter->second;
					}
				}	
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

	// ===================医保（直补）手续审查===================
	string chargeType = "", jzlb = "", patientId1 = "", patientId2 = "";
	iter = dbResult.find(ZB_str);
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "JZLB")
					jzlb = vecIter->second;
				else if(vecIter->first == "PATIENT_ID_1")
					patientId1 = vecIter->second;
				else if(vecIter->first == "PATIENT_ID_2")
					patientId2 = vecIter->second;
			}
		}
	}
	// 判断是否办理直补手续
	if(ZB_str == "ZB_check1")
	{
		if(!jzlb.empty())
			;//displayInfo += "患者【已办理】医保登记手续\n";
		else
		{
			if(ybjzk == "XNH_JZK_VS_PRICE_DICT")
				displayInfo += "患者【未办理】农合直补手续\n";
			else
				displayInfo += "患者【未办理】医保登记手续\n";
		}
	}
	else if(ZB_str == "ZB_check2")
	{
		if(!patientId1.empty())
			;//displayInfo += "患者【已办理】医保登记手续\n";
		else
			displayInfo += "患者【未办理】医保登记手续\n";
	}
	else if(ZB_str == "ZB_check3")
	{
		if(!patientId2.empty())
			;//displayInfo += "患者【已办理】医保登记手续\n";
		else
			displayInfo += "患者【未办理】医保登记手续\n";
	}

	// ===================费别变更审查===================
	string oper_date_time = "";
	string contents = "";
	iter = dbResult.find("charge_type_change");
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "OPER_DATE_TIME")
					oper_date_time = vecIter->second;
				else if(vecIter->first == "CONTENTS")
				{
					contents = vecIter->second;
					string::size_type index = contents.find("PAT_VISIT");
					if(index != string::npos)
						contents.replace(index, index+9, "");
				}
			}
		}
	}
	// 检测时间是否超过阈值
	if(!oper_date_time.empty())
	{
		int offsetHour = CalculateTimeDifference(oper_date_time);
		if(offsetHour>=0 && offsetHour<charge_type_check_hour)
			displayInfo += (contents + "\n");
	}

	
	return displayInfo;
}