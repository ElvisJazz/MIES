///////////////////////////////////////////////////////////////////////
//�ļ����� LogicHandler.cpp
//��  �ܣ� ҽ�����ϵͳ���ݿ��ȡ�����߼�������ʵ���ļ�
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//��  �ܣ�����ҽ���ύʱ��ʾ��Ҫ��������Ŀ�����ݿ�����
//��  ���������Ľ���ַ���ָ�룬���ݿ��ѯ����
//����ֵ��������ʾ��Ϣ
string LogicHandler::HandleProjectCheckInfoData(char* result, RESULT_MAP dbResult)
{
	string displayInfo = "������ʾ";
	string returnInfo = "{state:1}{message:�����ɹ�}{test:����}";
	// ���ؽ�����ݺ���ʾ����
	strcpy(result, returnInfo.c_str());
	
	return displayInfo;
}