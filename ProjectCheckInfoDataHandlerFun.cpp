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


//功  能：处理医生提交时提示需要审批的项目的数据库数据
//参  数：处理后的结果字符串指针，数据库查询数据
//返回值：窗口显示信息
string LogicHandler::HandleProjectCheckInfoData(char* result, RESULT_MAP dbResult)
{
	string displayInfo = "测试显示";
	string returnInfo = "{state:1}{message:操作成功}{test:测试}";
	// 返回结果数据和显示数据
	strcpy(result, returnInfo.c_str());
	
	return displayInfo;
}