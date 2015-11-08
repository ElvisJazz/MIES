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


//功  能：处理护士工作站提示患者农合是否直补的数据库数据
//参  数：处理后的结果字符串指针，数据库查询数据
//返回值：窗口显示信息
string LogicHandler::HandlePatientZBInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult)
{
	string ZB_str = ""; // 办理医保（直补）参数
	string displayInfo = "", ybjzk = "";

	// 获得直补参数
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
				if(vecIter->first == "NAME" && vecIter->second == "YBBL")
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
	// 返回结果数据和显示数据
	sprintf(result, "{state:1}{message:操作成功}");

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
	/*string tmpStr = ZB_str + " " + jzlb + " " + patientId1 + " " + patientId2;
	AfxMessageBox(CString(tmpStr.c_str()));
	AfxMessageBox(_T("医保检查类型：")+CString(ZB_str.c_str()));*/
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
	return displayInfo;
}