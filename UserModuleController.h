///////////////////////////////////////////////////////////////////////
//文件名： UserModuleController.h
//功  能： 医保审查模块开关类头文件
//开发者： 贾治中
//创  建：2014/8/5
//修  改：2014/8/5
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#ifndef _USER_MODULE_CONTROLLER_H_
#define _USER_MODULE_CONTROLLER_H_
#pragma once

#define PATIENT_INFO_MODULE 1 // 提示类模块
#define PRESETTLEMENT_MODULE 2 // 显示类模块
#define MEDICARE_INFO_MODULE 3 // 医保办理模块
#define TREATMENT_TIP_MODULE 4 // 用药提示模块
#define SERIOUS_TIP_MODULE 5 // 重症提示模块

class CUserModuleController
{
public:
	CUserModuleController(void);
	~CUserModuleController(void);

	static bool IsModuleOpen(int module); // 查看指定模块开关是否打开
	static void ReadFile();				  // 读取文件并获取相应开关参量
	static void GetDbInfoIntoFile();	  // 将读取的数据库信息写入文件并获取相应开关参量

private:
	static CString s_writeDate; // 文件写入日期
	static int s_patientInfoModuleValue;    // 提示类模块开关值
	static int s_presettlementModuleValue;  // 显示类模块开关值
	static int s_medicareInfoModuleValue;   // 医保办理模块开关值
	static int s_treatmentTipModuleValue;   // 用药提示模块开关值
	static int s_seriousTipModuleValue;   // 重症提示模块开关值
};


#endif