///////////////////////////////////////////////////////////////////////
//�ļ����� LogicHandler.cpp
//��  �ܣ� ҽ�����ϵͳ���ݿ��ȡ�����߼�������������ҩ��ʾ����ʵ���ļ�
//�����ߣ� ������
//��  ����2014/8/8
//��  �ģ�2014/8/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//��  �ܣ�����������ҩ��ʾ��ҽ��Ŀ¼����Ӧ�����ݿ�����
//��  ���������Ľ���ַ���ָ�룬��Ŀ���룬��֢���룬���ݿ��ѯ����������
//����ֵ��������ʾ��Ϣ
string LogicHandler::HandleOutpatientServiceData(char* result, string zz_code, string limitDay, RESULT_MAP dbResult)
{
	string displayInfo = "", resultStr = "";

	// ��֢��ʾ
	RESULT_MAP::iterator iter = dbResult.find("ZZTS");
	string zzts = "", zzxz = "";
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// ��ȡ������֢��ҩ�����Ĳ���ֵ��15��
				if(vecIter->first == "ZZSM")
				{
					zzts = vecIter->second;
				}
				else if(vecIter->first == "ZZXZ")
				{
					zzxz = vecIter->second;
				}
			}
		}
	}

	// ��ҩ����������ʾ
	iter = dbResult.find("ZZYYDAYS");
	string visit_date = "", amount = "", units = "";
	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// ��ȡ������֢��ҩ�����Ĳ���ֵ��15��
				if(vecIter->first == "VISIT_DATE")
				{
					visit_date = vecIter->second;
				}
				else if(vecIter->first == "AMOUNT")
				{
					amount = vecIter->second;
				}
				else if(vecIter->first == "UNITS")
				{
					units = vecIter->second;
				}
			}
		}
	}

	// ��ʾ��������
	string tmpStateStr = "{state:1}";
	if(visit_date != "")
	{
		displayInfo += "�û�����"+visit_date+"��������Ŀ"+amount+units+"��\n";
		tmpStateStr = "{state:1}";
		resultStr = "{limitDayMark:1}";
	}

	// ������ʾ	
	/*string ttmp1 = "zzxz:"+zzxz;
	string ttmp2 = "zzsm:"+zzts;
	string ttmp = ttmp1+ttmp2;
	AfxMessageBox(CString(ttmp.c_str()));*/


	// ��֢��ʾ
	CString zzxzCStr(zzxz.c_str());
	CString dstCStr(zz_code.c_str());
	zzxzCStr = zzxzCStr.Trim();
	// ��֢�����к�����֢����
	if(zzxz.find("a") != string::npos)
	{
		//AfxMessageBox(_T("����a"));
		tmpStateStr = "{state:1}";
	}
	else if((zzxz.find(",")==string::npos && zzxzCStr==dstCStr) || zzxzCStr.Left(zz_code.length()+1)==CString((zz_code+",").c_str()) || zzxz.find(","+zz_code+",")!=string::npos || zzxzCStr.Right(zz_code.length()+1)==CString((","+zz_code).c_str()))
	{
		if(zzts != "") // ��ʾ��������
		{
			displayInfo += "��֢���ƣ�"+zzts+"\n�Ƿ���ϸ�����������\n";
			tmpStateStr = "{state:3}";
		}
		else // �޲���
		{
			tmpStateStr = "{state:1}";
		}
	}
	else // ��׼������Ŀ
	{
		displayInfo += "�û�����֢���벻����֢����ʹ�÷�Χ�ڻ��ڻ�׼���У���������";
		tmpStateStr = "{state:2}";
	}

	resultStr = tmpStateStr + "{message:�����ɹ���}" + resultStr;

	strcpy(result, resultStr.c_str());
	return displayInfo;
}