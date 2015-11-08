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


//��  �ܣ�������ʾ���ѯ�����ݿ�����
//��  ���������Ľ���ַ���ָ�룬���ݿ��ѯ����
//����ֵ��������ʾ��Ϣ
string LogicHandler::HandlePatientInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult)
{
	string displayInfo = "";
	// ���ؽ�����ݺ���ʾ����
	sprintf(result, "{state:1}{message:�����ɹ�}");

	// ===================��ȡ���������ݿ���ֵ���ò���===================
	// ���������������ö�Ӧ��׼������ò���ֵ
	
	int charge_type_check_hour = 0; // �ѱ���ʱ�����
	string ZB_str = ""; // ����ҽ����ֱ��������
	string ybjzk = ""; // ҽ����׼��

	RESULT_MAP::iterator iter = dbParameter.find("parameter");
	if(iter != dbParameter.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// ��ȡ��Ӧ�Ĳ���ֵ
				if(vecIter->first == "NAME" && vecIter->second == "FBBGJCSJ")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							charge_type_check_hour = atoi(vecIter->second.c_str());
					}
				}
				else if(vecIter->first == "NAME" && vecIter->second == "YBBL")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							ZB_str = vecIter->second;
					}
				}	
				else if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							ybjzk = vecIter->second;
					}
				}	
			}
		}
	}

	// ===================ҽ����ֱ�����������===================
	string chargeType = "", jzlb = "", patientId1 = "", patientId2 = "";
	iter = dbResult.find(ZB_str);
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "JZLB")
					jzlb = vecIter->second;
				else if(vecIter->first == "PATIENT_ID_1")
					patientId1 = vecIter->second;
				else if(vecIter->first == "PATIENT_ID_2")
					patientId2 = vecIter->second;
			}
		}
	}
	// �ж��Ƿ����ֱ������
	if(ZB_str == "ZB_check1")
	{
		if(!jzlb.empty())
			;//displayInfo += "���ߡ��Ѱ���ҽ���Ǽ�����\n";
		else
		{
			if(ybjzk == "XNH_JZK_VS_PRICE_DICT")
				displayInfo += "���ߡ�δ����ũ��ֱ������\n";
			else
				displayInfo += "���ߡ�δ����ҽ���Ǽ�����\n";
		}
	}
	else if(ZB_str == "ZB_check2")
	{
		if(!patientId1.empty())
			;//displayInfo += "���ߡ��Ѱ���ҽ���Ǽ�����\n";
		else
			displayInfo += "���ߡ�δ����ҽ���Ǽ�����\n";
	}
	else if(ZB_str == "ZB_check3")
	{
		if(!patientId2.empty())
			;//displayInfo += "���ߡ��Ѱ���ҽ���Ǽ�����\n";
		else
			displayInfo += "���ߡ�δ����ҽ���Ǽ�����\n";
	}

	// ===================�ѱ������===================
	string oper_date_time = "";
	string contents = "";
	iter = dbResult.find("charge_type_change");
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "OPER_DATE_TIME")
					oper_date_time = vecIter->second;
				else if(vecIter->first == "CONTENTS")
				{
					contents = vecIter->second;
					string::size_type index = contents.find("PAT_VISIT");
					if(index != string::npos)
						contents.replace(index, index+9, "");
				}
			}
		}
	}
	// ���ʱ���Ƿ񳬹���ֵ
	if(!oper_date_time.empty())
	{
		int offsetHour = CalculateTimeDifference(oper_date_time);
		if(offsetHour>=0 && offsetHour<charge_type_check_hour)
			displayInfo += (contents + "\n");
	}

	
	return displayInfo;
}