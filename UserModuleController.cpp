///////////////////////////////////////////////////////////////////////
//�ļ����� UserModuleController.cpp
//��  �ܣ� ҽ�����ģ�鿪����ʵ��
//�����ߣ� ������
//��  ����2014/8/5
//��  �ģ�2014/8/5
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "UserModuleController.h"
#include <fstream>
#include <string>
#include <map>
#include "sql_dao.h"

using namespace std;


CString CUserModuleController::s_writeDate = _T("");
int CUserModuleController::s_patientInfoModuleValue = 0;
int CUserModuleController::s_presettlementModuleValue = 0;
int CUserModuleController::s_medicareInfoModuleValue = 0;
int CUserModuleController::s_treatmentTipModuleValue = 0;
int CUserModuleController::s_seriousTipModuleValue = 0;

CUserModuleController::CUserModuleController(void)
{
}


CUserModuleController::~CUserModuleController(void)
{
}

// ���ܣ��鿴ָ��ģ�鿪���Ƿ��
// ������ģ������Ӧֵ
// ���أ���
bool CUserModuleController::IsModuleOpen(int module)  
{
	// ��ȡ��ǰʱ��
	CTime currentTime = CTime::GetCurrentTime();
	CString szTime = currentTime.Format("%Y-%m-%d");

	// д�����ڱ���Ϊ�գ���ȥ�����ؿ��ز����ļ�,��ȡ���ز���д������
	/*if(s_writeDate == _T(""))
	{
		ReadFile();
	}*/
	
	// д�������뵱ǰ���ڲ�һ�£���ȡ���ݿ���±��ؿ����ļ�
	//if(s_writeDate != szTime)
	//{
		GetDbInfoIntoFile();
	//}

	switch(module)
	{
	case PATIENT_INFO_MODULE:	return s_patientInfoModuleValue;
	case PRESETTLEMENT_MODULE:	return s_presettlementModuleValue;
	case MEDICARE_INFO_MODULE:	return s_medicareInfoModuleValue;
	case TREATMENT_TIP_MODULE:	return s_treatmentTipModuleValue;
	case SERIOUS_TIP_MODULE:	return s_seriousTipModuleValue;
	default: return false;
	}

}

// ���ܣ���ȡ�ļ�����ȡ��Ӧ���ز���
// ��������
// ���أ���
void CUserModuleController::ReadFile()			  
{
	ifstream read("moduleCommand.ini", ios::in);
	// �ļ�������
	if(!read)
		return;
	// ��ȡ��ȡ���ں�����ģ�����	
	string writeDate, name, value;
	map<string, string> parameterMap;
	if(!read.eof())
		read>>writeDate;
	else
		return;
	
	while(!read.eof())
	{
		read>>name;
		read>>value;
		parameterMap.insert(pair<string, string>(name, value));
	}
	
	map<string, string>::iterator iter = parameterMap.begin();
	for(; iter!=parameterMap.end(); ++iter)
	{
		if(iter->first == "TSL_MODULE_COMMAND")
			s_patientInfoModuleValue = atoi(iter->second.c_str());
		else if(iter->first == "XSL_MODULE_COMMAND")
			s_presettlementModuleValue = atoi(iter->second.c_str());
		else if(iter->first == "YBBL_MODULE_COMMAND")
			s_medicareInfoModuleValue = atoi(iter->second.c_str());
		else if(iter->first == "YYTS_MODULE_COMMAND")
			s_treatmentTipModuleValue = atoi(iter->second.c_str());

	}
	s_writeDate = CString(writeDate.c_str());
	
}

// ���ܣ�����ȡ�����ݿ���Ϣд���ļ�����ȡ��Ӧ���ز���
// ��������
// ���أ���
void CUserModuleController::GetDbInfoIntoFile()	  
{
	/*ofstream output("moduleCommand.ini", ios::out);
	if(!output)
	{
		AfxMessageBox(_T("д���ļ�ʧ�ܣ�"));
	}
*/
	map<string,string> param = map<string,string>(); 
	// ������ѯ����
	CSqlDao dao;
	RESULT_MAP dbParameter;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf", "MIES_sql_config.xml");
		// ִ�в�ѯ
		
		// ���Ҳ�����
		dbParameter = dao.Query("USER_COMMAND_Parameter", param);
		
		// ���������������ö�Ӧ�����ò���ֵ
		string ybjzk = "";
		RESULT_MAP::iterator iter = dbParameter.find("parameter");
		while(iter != dbParameter.end())
		{
			// д�뵱ǰʱ��
			/*CTime currentTime = CTime::GetCurrentTime();
			s_writeDate = currentTime.Format("%Y-%m-%d");
			char dateStr[0x100];
			::wsprintfA(dateStr, "%ls", (LPCTSTR)s_writeDate);
			output<<string(dateStr)<<endl;
*/
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					// ��ȡ��ʾ��Ĳ���ֵ
					if(vecIter->first == "NAME" && vecIter->second == "TSL_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_patientInfoModuleValue = atoi(vecIter->second.c_str());
								//output<<"TSL_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// ��ȡ��ʾ��Ĳ���ֵ
					else if(vecIter->first == "NAME" && vecIter->second == "XSL_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_presettlementModuleValue = atoi(vecIter->second.c_str());
								//output<<"XSL_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// ��ȡҽ������Ĳ���ֵ
					else if(vecIter->first == "NAME" && vecIter->second == "YBBL_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_medicareInfoModuleValue = atoi(vecIter->second.c_str());
								//output<<"YBBL_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// ��ȡ��ҩ��ʾ�Ĳ���ֵ
					else if(vecIter->first == "NAME" && vecIter->second == "YYTS_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_treatmentTipModuleValue = atoi(vecIter->second.c_str());
								//output<<"YYTS_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
					// ��ȡ������ҩ��ʾ�Ĳ���ֵ
					else if(vecIter->first == "NAME" && vecIter->second == "MZZZ_MODULE_COMMAND")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
							{
								s_seriousTipModuleValue = atoi(vecIter->second.c_str());
								//output<<"YYTS_MODULE_COMMAND"<<" "<<vecIter->second<<endl;
							}
						}
					}
				}
			}
			++iter;
		}
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();
}