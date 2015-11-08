///////////////////////////////////////////////////////////////////////
//�ļ����� UserModuleController.h
//��  �ܣ� ҽ�����ģ�鿪����ͷ�ļ�
//�����ߣ� ������
//��  ����2014/8/5
//��  �ģ�2014/8/5
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#ifndef _USER_MODULE_CONTROLLER_H_
#define _USER_MODULE_CONTROLLER_H_
#pragma once

#define PATIENT_INFO_MODULE 1 // ��ʾ��ģ��
#define PRESETTLEMENT_MODULE 2 // ��ʾ��ģ��
#define MEDICARE_INFO_MODULE 3 // ҽ������ģ��
#define TREATMENT_TIP_MODULE 4 // ��ҩ��ʾģ��
#define SERIOUS_TIP_MODULE 5 // ��֢��ʾģ��

class CUserModuleController
{
public:
	CUserModuleController(void);
	~CUserModuleController(void);

	static bool IsModuleOpen(int module); // �鿴ָ��ģ�鿪���Ƿ��
	static void ReadFile();				  // ��ȡ�ļ�����ȡ��Ӧ���ز���
	static void GetDbInfoIntoFile();	  // ����ȡ�����ݿ���Ϣд���ļ�����ȡ��Ӧ���ز���

private:
	static CString s_writeDate; // �ļ�д������
	static int s_patientInfoModuleValue;    // ��ʾ��ģ�鿪��ֵ
	static int s_presettlementModuleValue;  // ��ʾ��ģ�鿪��ֵ
	static int s_medicareInfoModuleValue;   // ҽ������ģ�鿪��ֵ
	static int s_treatmentTipModuleValue;   // ��ҩ��ʾģ�鿪��ֵ
	static int s_seriousTipModuleValue;   // ��֢��ʾģ�鿪��ֵ
};


#endif