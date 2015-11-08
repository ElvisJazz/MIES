///////////////////////////////////////////////////////////////////////
//文件名： LogicHandler.h
//功  能： 医保审查系统数据库获取数据逻辑处理类头文件
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
#include <vector>
#include <string>
#include "sql_dao.h"

using namespace std;

class LogicHandler
{
public:
	LogicHandler(void);
	~LogicHandler(void);

	// 处理护士工作站提示患者农合是否直补的数据库数据
	string HandlePatientZBInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult);
	// 处理预结算查询的数据库数据
	void HandlePatientPreSettlementInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult, RESULT_MAP dbResult2);
	// 处理提示类查询的数据库数据
	string HandlePatientInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult);
	// 处理医生提交时提示需要审批的项目的数据库数据
	string HandleProjectCheckInfoData(char* result, RESULT_MAP dbResult);
	// 处理用药提示（医保目录）对应的数据库数据
	string HandleMedicalInsuranceInfoData(char* result, string patientId, string visitId, string item_code, RESULT_MAP dbParameter, RESULT_MAP dbResult, string chargeType, string classCode);
	
	// 获取项目费用类型
	string GetFeeType(string ybjzk, string ybjzkcs, string itemClass, string itemCode);

	// 处理门诊用药
	string HandleOutpatientServiceData(char* result, string zz_code, string limitDay, RESULT_MAP dbResult);
	
	// 计算与当前时间差，格式如：2014-02-13 9:33:50，返回所差的小时数，正数表示小于当前时间，负数表示大于当前时间
	int CalculateTimeDifference(string time);
	// 根据字段名解析对应中文
	string GetChineseName(string field);
	// 解析值与颜色映射关系并返回颜色字符串
	string GetColorStr(string numColorCs, double rate);
	// 替换目标串中颜色参数
	void ReplaceColorStr(string &desStr, string desField, string numColorCs, double rate);
};

