///////////////////////////////////////////////////////////////////////
//文件名： MIES.h
//功  能： 医保审查系统dll主文件（外部接口函数申明）
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <string>
using namespace std;

// 字符串消除工具函数
void replaceStr(char* str, string startStr, string endStr);

// 接口函数申明
// 护士工作站提示患者农合是否直补接口函数
extern "C" __declspec(dllexport) void __cdecl DisplayPatientZBInfo(char* result, char* patient_id, char* visit_id);
// 预结算查询接口函数
extern "C" __declspec(dllexport) void __cdecl DisplayPatientPreSettlementInfo(char* result, char* patient_id, char* visit_id);
// 提示类查询接口函数
extern "C" __declspec(dllexport) void __cdecl DisplayPatientInfo(char* result, char* patient_id, char* visit_id);
// 医生提交时提示需要审批的项目接口函数
extern "C" __declspec(dllexport) void __cdecl DisplayProjectCheckInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type , char* item_spec);
// 医保目录对应相关操作接口函数
extern "C" __declspec(dllexport) void __cdecl DisplayMedicalInsuranceInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type , char* item_spec);
// 打印自费知情同意书接口函数
extern "C" __declspec(dllexport) void __cdecl PrintSelfChargeConsentLetter(char* result, char* patient_id);
// 打印每日清单接口函数
extern "C" __declspec(dllexport) void __cdecl PrintDailyList(char* patient_id);

// 门诊医保目录对应相关操作接口函数
extern "C" __declspec(dllexport) int __stdcall DisplayOutpatientServiceInfo(char* result, char* patient_id, char* item_code, char* zz_code);


// CMIESApp
// 有关此类实现的信息，请参阅 MIES.cpp
//

class CMIESApp : public CWinApp
{
public:
	CMIESApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
