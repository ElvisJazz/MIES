///////////////////////////////////////////////////////////////////////
//文件名： LogicHandler.cpp
//功  能： 医保审查系统数据库获取数据逻辑处理类门诊用药提示函数实现文件
//开发者： 贾治中
//创  建：2014/8/8
//修  改：2014/8/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//功  能：处理门诊用药提示（医保目录）对应的数据库数据
//参  数：处理后的结果字符串指针，项目代码，重症代码，数据库查询参数和数据
//返回值：窗口显示信息
string LogicHandler::HandleOutpatientServiceData(char* result, string zz_code, string limitDay, RESULT_MAP dbResult)
{
	string displayInfo = "", resultStr = "";

	// 重症提示
	RESULT_MAP::iterator iter = dbResult.find("ZZTS");
	string zzts = "", zzxz = "";
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// 获取门诊重症开药天数的参数值（15）
				if(vecIter->first == "ZZSM")
				{
					zzts = vecIter->second;
				}
				else if(vecIter->first == "ZZXZ")
				{
					zzxz = vecIter->second;
				}
			}
		}
	}

	// 用药天数限制提示
	iter = dbResult.find("ZZYYDAYS");
	string visit_date = "", amount = "", units = "";
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// 获取门诊重症开药天数的参数值（15）
				if(vecIter->first == "VISIT_DATE")
				{
					visit_date = vecIter->second;
				}
				else if(vecIter->first == "AMOUNT")
				{
					amount = vecIter->second;
				}
				else if(vecIter->first == "UNITS")
				{
					units = vecIter->second;
				}
			}
		}
	}

	// 显示窗口设置
	string tmpStateStr = "{state:1}";
	if(visit_date != "")
	{
		displayInfo += "该患者在"+visit_date+"开过此项目"+amount+units+"！\n";
		tmpStateStr = "{state:1}";
		resultStr = "{limitDayMark:1}";
	}

	// 测试提示	
	/*string ttmp1 = "zzxz:"+zzxz;
	string ttmp2 = "zzsm:"+zzts;
	string ttmp = ttmp1+ttmp2;
	AfxMessageBox(CString(ttmp.c_str()));*/


	// 重症提示
	CString zzxzCStr(zzxz.c_str());
	CString dstCStr(zz_code.c_str());
	zzxzCStr = zzxzCStr.Trim();
	// 重症限制中含有重症代码
	if(zzxz.find("a") != string::npos)
	{
		//AfxMessageBox(_T("含有a"));
		tmpStateStr = "{state:1}";
	}
	else if((zzxz.find(",")==string::npos && zzxzCStr==dstCStr) || zzxzCStr.Left(zz_code.length()+1)==CString((zz_code+",").c_str()) || zzxz.find(","+zz_code+",")!=string::npos || zzxzCStr.Right(zz_code.length()+1)==CString((","+zz_code).c_str()))
	{
		if(zzts != "") // 提示限制条件
		{
			displayInfo += "重症限制："+zzts+"\n是否符合该限制条件？\n";
			tmpStateStr = "{state:3}";
		}
		else // 无操作
		{
			tmpStateStr = "{state:1}";
		}
	}
	else // 不准开此项目
	{
		displayInfo += "该患者重症代码不在重症限制使用范围内或不在基准库中，不允许开！";
		tmpStateStr = "{state:2}";
	}

	resultStr = tmpStateStr + "{message:操作成功！}" + resultStr;

	strcpy(result, resultStr.c_str());
	return displayInfo;
}