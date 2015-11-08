///////////////////////////////////////////////////////////////////////
//�ļ����� MIES.h
//��  �ܣ� ҽ�����ϵͳdll���ļ����ⲿ�ӿں���������
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <string>
using namespace std;

// �ַ����������ߺ���
void replaceStr(char* str, string startStr, string endStr);

// �ӿں�������
// ��ʿ����վ��ʾ����ũ���Ƿ�ֱ���ӿں���
extern "C" __declspec(dllexport) void __cdecl DisplayPatientZBInfo(char* result, char* patient_id, char* visit_id);
// Ԥ�����ѯ�ӿں���
extern "C" __declspec(dllexport) void __cdecl DisplayPatientPreSettlementInfo(char* result, char* patient_id, char* visit_id);
// ��ʾ���ѯ�ӿں���
extern "C" __declspec(dllexport) void __cdecl DisplayPatientInfo(char* result, char* patient_id, char* visit_id);
// ҽ���ύʱ��ʾ��Ҫ��������Ŀ�ӿں���
extern "C" __declspec(dllexport) void __cdecl DisplayProjectCheckInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type , char* item_spec);
// ҽ��Ŀ¼��Ӧ��ز����ӿں���
extern "C" __declspec(dllexport) void __cdecl DisplayMedicalInsuranceInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type , char* item_spec);
// ��ӡ�Է�֪��ͬ����ӿں���
extern "C" __declspec(dllexport) void __cdecl PrintSelfChargeConsentLetter(char* result, char* patient_id);
// ��ӡÿ���嵥�ӿں���
extern "C" __declspec(dllexport) void __cdecl PrintDailyList(char* patient_id);

// ����ҽ��Ŀ¼��Ӧ��ز����ӿں���
extern "C" __declspec(dllexport) int __stdcall DisplayOutpatientServiceInfo(char* result, char* patient_id, char* item_code, char* zz_code);


// CMIESApp
// �йش���ʵ�ֵ���Ϣ������� MIES.cpp
//

class CMIESApp : public CWinApp
{
public:
	CMIESApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
