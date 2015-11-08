///////////////////////////////////////////////////////////////////////
//�ļ����� LogicHandler.h
//��  �ܣ� ҽ�����ϵͳ���ݿ��ȡ�����߼�������ͷ�ļ�
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
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

	// ����ʿ����վ��ʾ����ũ���Ƿ�ֱ�������ݿ�����
	string HandlePatientZBInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult);
	// ����Ԥ�����ѯ�����ݿ�����
	void HandlePatientPreSettlementInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult, RESULT_MAP dbResult2);
	// ������ʾ���ѯ�����ݿ�����
	string HandlePatientInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult);
	// ����ҽ���ύʱ��ʾ��Ҫ��������Ŀ�����ݿ�����
	string HandleProjectCheckInfoData(char* result, RESULT_MAP dbResult);
	// ������ҩ��ʾ��ҽ��Ŀ¼����Ӧ�����ݿ�����
	string HandleMedicalInsuranceInfoData(char* result, string patientId, string visitId, string item_code, RESULT_MAP dbParameter, RESULT_MAP dbResult, string chargeType, string classCode);
	
	// ��ȡ��Ŀ��������
	string GetFeeType(string ybjzk, string ybjzkcs, string itemClass, string itemCode);

	// ����������ҩ
	string HandleOutpatientServiceData(char* result, string zz_code, string limitDay, RESULT_MAP dbResult);
	
	// �����뵱ǰʱ����ʽ�磺2014-02-13 9:33:50�����������Сʱ����������ʾС�ڵ�ǰʱ�䣬������ʾ���ڵ�ǰʱ��
	int CalculateTimeDifference(string time);
	// �����ֶ���������Ӧ����
	string GetChineseName(string field);
	// ����ֵ����ɫӳ���ϵ��������ɫ�ַ���
	string GetColorStr(string numColorCs, double rate);
	// �滻Ŀ�괮����ɫ����
	void ReplaceColorStr(string &desStr, string desField, string numColorCs, double rate);
};

