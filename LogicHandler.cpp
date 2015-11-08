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
#include <time.h>


LogicHandler::LogicHandler(void)
{

}


LogicHandler::~LogicHandler(void)
{
}

//功 能：计算与当前时间差，格式如：2014-02-13 9:33:50，返回所差的小时数，正数表示小于当前时间，-1表示大于当前时间
//输 入：待比较时间
//输 出：时间差
int LogicHandler::CalculateTimeDifference(string timeStr)
{
	timeStr += ' ';
	string::size_type index1 = 0, index2 = 0;
	string::size_type length = timeStr.length();
	
	struct tm* timePtr = new struct tm();

	// 跳过可能存在的空格
	for(; index1<length; index1++)
	{
		if(timeStr[index1] != ' ')
			break;
	}
	// 获得年份
	index2 = timeStr.find('-', index1);
	timePtr->tm_year = atoi((timeStr.substr(index1, index2-index1)).c_str())-1900;
	// 获得月份
	index1 = index2 + 1;
	index2 = timeStr.find('-', index1);
	timePtr->tm_mon = atoi((timeStr.substr(index1, index2-index1)).c_str())-1;
	// 获得日
	index1 = index2 + 1;
	index2 = timeStr.find(' ', index1);
	timePtr->tm_mday = atoi((timeStr.substr(index1, index2-index1)).c_str());

	// 跳过可能存在的空格
	index1 = index2;
	for(; index1<length; index1++)
	{
		if(timeStr[index1] != ' ')
			break;
	}
	// 获得时
	index2 = timeStr.find(':', index1);
	timePtr->tm_hour = atoi((timeStr.substr(index1, index2-index1)).c_str());
	// 获得分
	index1 = index2 + 1;
	index2 = timeStr.find(':', index1);
	timePtr->tm_min = atoi((timeStr.substr(index1, index2-index1)).c_str());
	// 获得秒
	index1 = index2 + 1;
	index2 = timeStr.find(' ', index1);
	timePtr->tm_sec = atoi((timeStr.substr(index1, index2-index1)).c_str());

	timePtr->tm_isdst = -1;
	// 转换成秒数
	time_t otime = mktime(timePtr);
	time_t stime = time(NULL);
	struct tm* sblock = localtime(&stime);
	stime = mktime(sblock);

	double offsetSecond = difftime(stime, otime);

	// 释放指针内存
	delete timePtr;
	if(offsetSecond < 0)
		return -1;
	else
		return ((long)offsetSecond) / 3600;
}


//功  能：根据字段名解析对应中文
//参  数：待处理的字段名，预结算查询参数
//返回值：对应的中文名称
string LogicHandler::GetChineseName(string field)
{
	if(field == "NAME")
		return "患者姓名";
	else if(field == "JZLB")
		return "医保类型";
	else if(field == "YLFYZE")
		return "本次医疗费用";
	else if(field == "PREPAYMENTS")
		return "预交金";
	else if(field == "GRZF")
		return "个人自付";
	else if(field == "GRFDL")
		return "自付费用比例(%)";
	else if(field == "CZFJE")
		return "纯自费费用";
	else if(field == "ELZFZF")
		return "纯自费费用";
	else if(field == "JBYLFDFY")
		return "统筹医疗费用";
	else if(field == "TCZFHJ")
		return "统筹支付合计";
	else if(field == "YBZFYJZ")
		return "医保支付金额预警值";
	else if(field == "SBPFHJ")
		return "商保赔付（大额）";
	else if(field == "JBYLTCBLF")
		return "统筹支付1";
	else if(field == "JBYLBLZHF")
		return "统筹支付2（置换材料）";
	else if(field == "TCZF")
		return "统筹支付";
	else if(field == "JJZF")
		return "基金支付";
	else if(field == "GRBLZF")
		return "个人比例支付";
	else if(field == "NDYLFY")
		return "年度累计医保费用";


	return field;
}

//功 能：解析值与颜色映射关系并返回颜色字符串
//输 入：颜色参数字符串，比例值
//输 出：颜色字符串
string LogicHandler::GetColorStr(string numColorCs, double rate)
{
	string::size_type index1=0, index2=0;
	// 上一个解析的颜色变量
	string lastColorCsStr = "";
	// 解析的数值参数变量
	double rateCs = 0.0;

	index2 = numColorCs.find(":");
	while(index2 != string::npos)
	{
		rateCs = atof(numColorCs.substr(index1, index2-index1).c_str());
		if(rate > rateCs)
		{
			index1 = numColorCs.find(";", index2);
			if(index1 != string::npos)
				lastColorCsStr = numColorCs.substr(index2+1, index1-index2-1);
			else
			{
				AfxMessageBox(_T("比例参数解析出问题！"));
				return "";
			}
			index2 = numColorCs.find(":", index1);
			index1++;
		}
		else
			return lastColorCsStr;
	}

	return lastColorCsStr;
}
 
//功 能：替换目标串中颜色参数
//输 入：目标串，颜色参数字符串，比例值
//输 出：颜色字符串
void LogicHandler::ReplaceColorStr(string &desStr, string desField, string numColorCs, double rate)
{
	string colorStr = GetColorStr(numColorCs, rate);
	if(!colorStr.empty())
	{
		string chiName = GetChineseName(desField);
		string::size_type index1 = desStr.find(chiName);
		if(index1 != string::npos)
		{
			string::size_type index2 = desStr.find("<<", index1);
			index1 = desStr.find(">>", index2);
			desStr.replace(index2+2, index1-index2-2, colorStr);
		}
	}
}