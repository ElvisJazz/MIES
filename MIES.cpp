///////////////////////////////////////////////////////////////////////
//�ļ����� MIES.cpp
//��  �ܣ� ҽ�����ϵͳdll���ļ���ʼ�����ⲿ�ӿں������壩
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MIES.h"
#include "PrintSelfConsentDlg.h"
#include "DailyListDlg.h"
#include "LogicHandler.h"
#include "UserModuleController.h"
#include "sql_dao.h"	// CSqlDao��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMIESApp

BEGIN_MESSAGE_MAP(CMIESApp, CWinApp)
END_MESSAGE_MAP()


// CMIESApp ����

CMIESApp::CMIESApp()
{
}


// Ψһ��һ�� CMIESApp ����

CMIESApp theApp;

// CMIESApp ��ʼ��

BOOL CMIESApp::InitInstance()
{
	
	
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CWinApp::InitInstance();
	// �޸ı���
	free((void*)m_pszAppName);
	CString str=_T("MIESҽ�������ʾ");
	m_pszAppName=(LPCTSTR)malloc((str.GetLength()+1)*sizeof(TCHAR));
	_tcscpy_s((LPTSTR)m_pszAppName, (size_t)str.GetLength()+1, str);
	afxCurrentAppName=m_pszAppName;    

	return TRUE;
}
 
//��  �ܣ��ַ����������ߺ���
//��  ����Ŀ�괮����ʼ��־����������־��
//����ֵ����
void replaceStr(char* str,string startStr, string endStr)
{
	string resultStr = string(str);
	string::size_type index1=0, index2=0;
	string subStr = "";
	index1 = resultStr.find(startStr); // ȥ������0�����ѣ���־����Ŀ
	if(index2 != string::npos)
	{
		index2 = resultStr.find(endStr, index1);
		if(index2 != string::npos)
		{
			resultStr.replace(index1, index2-index1+1, "");
			strcpy(str, resultStr.c_str());
		}
		else
			strcpy(str,"{state:0}{message:���ɽ���ַ�������}");
	}
	else
		strcpy(str,"{state:0}{message:���ɽ���ַ�������}");
}

//��  �ܣ���ʿ����վ��ʾ����ũ���Ƿ�ֱ���ӿں���
//��  ��������id��סԺ��
//����ֵ����ѯ���
extern "C" __declspec(dllexport) void __cdecl DisplayPatientZBInfo(char* result, char* patient_id, char* visit_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��⿪��
	if(!CUserModuleController::IsModuleOpen(MEDICARE_INFO_MODULE))
	{
		strcpy(result, "{state:1}{message:�����ɹ�}");
		return;
	}

	// �������Ƿ�Ϊ��
	string str1, str2;
	str1 = string(patient_id);
	str2 = string(visit_id);
	if(str1.empty() || str2.empty())
	{
		AfxMessageBox(_T("����id��סԺ�Ų���Ϊ�գ�"));
		strcpy(result, "{state:0}{message:��������Ϊ�գ�}");
		return;
	}

	// =================ͨ�����ݿ��ȡ��ѯ���=======================
	RESULT_MAP dbResult, dbParameter;
	// ��ʼ�������б�
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", str1)); //30323590
	param.insert(pair<string, string>("VISIT_ID", str2)); //6
	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����		
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ

		// ���Ҳ�����
		dbParameter = dao.Query("YBZBParameter", param);
		// ��ȡֱ����������
		dbResult = dao.Query("PatientZBInfo",param);
	}
	catch( runtime_error err )
	{
		// �쳣�������	
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:���ݿ��������}");
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();
	
	//  =================ͨ���ж��߼������ѯ����ó���ʾ���� =================
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandlePatientZBInfoData(result, dbParameter, dbResult);
	
	//  =================��ʾ��ѯ������ڶ��� =================
	if(displayInfo != "")
	{
		// ��ȡƤ��
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		
		// �ͷ�Ƥ��
		skinppExitSkin();
	}
}

//��  �ܣ�Ԥ�����ѯ�ӿں���
//��  ��������id��סԺ��
//����ֵ����ѯ���
extern "C" __declspec(dllexport) void __cdecl DisplayPatientPreSettlementInfo(char* result, char*  patient_id, char* visit_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��⿪��
	if(!CUserModuleController::IsModuleOpen(PRESETTLEMENT_MODULE))
	{
		strcpy(result, "{state:1}{message:�����ɹ�}");
		return;
	}

	// �������Ƿ�Ϊ��
	string str1, str2;
	str1 = string(patient_id);
	str2 = string(visit_id);
	if(str1.empty() || str2.empty())
	{
		AfxMessageBox(_T("����id��סԺ�Ų���Ϊ�գ�"));
		strcpy(result, "{state:0}{message:��������Ϊ�գ�}");
		return;
	}

	// =================ͨ�����ݿ��ȡ��ѯ���=======================
	RESULT_MAP dbResult, dbResult2, dbResult3, dbParameter;
	// ��ʼ�������б�
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", str1)); //30323590
	param.insert(pair<string, string>("VISIT_ID", str2)); //6
	// ������ѯ����
	CSqlDao dao;
	// ������ʱ������Ԥ�����ѯ������ҽ����׼�����
	string yjscx = "";
	string ybjzk = "";

	try
	{
		// ��ʼ����ѯ����		
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ

		// ===============����Ԥ���������===============
		dbParameter = dao.Query("YBPreSettlementInfoParameter", param);
		RESULT_MAP::iterator iter = dbParameter.find("parameter");
		if(iter != dbParameter.end())
		{
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					// ��ȡԤ�����ѯ�Ĳ���ֵ
					if(vecIter->first == "NAME" && vecIter->second == "YJSCX")
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "VALUE")
								yjscx = vecIter->second;
						}
					}
					// ��ȡҽ����׼��Ĳ���ֵ
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
		// �ҵ���Ӧ��Ԥ�������ֵ����sql_config.xml�д��ڶ�Ӧ��yjscxֵ��group��ѯ
		if(!yjscx.empty())
			dbResult = dao.Query(yjscx, param);

		// ===============���ݲ����б�����漰��charge_type���������Ĳ�ѯ����Ҫ���Է�ҩռ�ȡ��Է���Ŀ�����ļ���===============
		
		// �ҵ���Ӧ��ҽ����׼�����ֵ����sql_config.xml�д��ڶ�Ӧ��ybjzkֵ��group��ѯ
		if(!ybjzk.empty())
			dbResult2 = dao.Query(ybjzk, param);
	}
	catch( runtime_error err )
	{
		// �쳣�������	
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:���ݿ��������}");
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();
	//  =================ͨ���ж��߼������ѯ����ó���ʾ���� =================
	LogicHandler logicHandler;
	logicHandler.HandlePatientPreSettlementInfoData(result, dbParameter, dbResult, dbResult2);
	//  =================��ʾ��ѯ������ڶ��� =================
	//CDisplayInfoDlg dlg;
	//// ������ʾ��Ϣ
	//dlg.SetDisplayInfo(CString(displayInfo.c_str()));
	//dlg.DoModal();
}
 
 
//��  �ܣ���ʾ���ѯ�ӿں���
//��  ��������id��סԺ��
//����ֵ����ѯ���
extern "C" __declspec(dllexport) void __cdecl DisplayPatientInfo(char* result, char* patient_id, char* visit_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��⿪��
	if(!CUserModuleController::IsModuleOpen(PATIENT_INFO_MODULE))
	{
		strcpy(result, "{state:1}{message:�����ɹ�}");
		return;
	}

	// �������Ƿ�Ϊ��
	string str1, str2;
	str1 = string(patient_id);
	str2 = string(visit_id);
	if(str1.empty() || str2.empty())
	{
		AfxMessageBox(_T("����id��סԺ�Ų���Ϊ�գ�"));
		strcpy(result, "{state:0}{message:��������Ϊ�գ�}");
		return;
	}

	// =================ͨ�����ݿ��ȡ��ѯ���=======================
	RESULT_MAP dbParameter, dbResult1;
	// ��ʼ�������б�
	map<string,string> param;
	param.insert(pair<string, string>("PATIENT_ID", str1)); //30323590
	param.insert(pair<string, string>("VISIT_ID", str2)); //6
	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ
		
		// ���Ҳ�����
		dbParameter = dao.Query("YBInfoParameter", param);
		// ��ȡ������ʾ����Ϣ
		dbResult1 = dao.Query("PatientInfo",param);
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:���ݿ��������}");
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();

	// =================ͨ���ж��߼������ѯ����ó���ʾ����=================
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandlePatientInfoData(result, dbParameter, dbResult1);
	
	// =================��ʾ��ѯ������ڶ���=================
	if(displayInfo != "")
	{
		// ��ȡƤ��
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		
		// �ͷ�Ƥ��
		skinppExitSkin();
	}
}

//��  �ܣ�ҽ���ύʱ��ʾ��Ҫ��������Ŀ�ӿں���
//��  ��������id��סԺ�ţ�ҽԺ��Ŀ���ҽԺ��Ŀ���룬ҽ���ѱ�ҽԺ��Ŀ���
//����ֵ����ѯ���
extern "C" __declspec(dllexport) void __cdecl DisplayProjectCheckInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type, char* item_spec)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//// ��⿪��
	//if(!CUserModuleController::IsModuleOpen(PATIENT_INFO_MODULE))
	//	return;
	
	// ͨ�����ݿ��ȡ��ѯ���
	map<string, vector<vector<pair<string, string> > > > dbResult = RESULT_MAP();
	
	// ͨ���ж��߼������ѯ����ó���ʾ����
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandleProjectCheckInfoData(result, dbResult);
	
	// ��ʾ��ѯ������ڶ���
	if(displayInfo != "")
	{
		//Line 1  , load the skin
		//bool is = SkinStartW(_T("skins\\basic_urf\\office2007_af0.urf"),WINDOW_TYPE_VC,_T(""),GTP_LOAD_FILE,NULL,NULL);
		//Enable use URF font settings 
		//SkinWindowSetEx(NULL,SKIN_SET_UNICODE_URF_FONT,1,0,NULL,0,NULL);
		// ��ȡƤ��
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		
		// �ͷ�Ƥ��
		skinppExitSkin();
		//Line 2  , cleanup
		//SkinRemove() ;
	}
}

//��  �ܣ�ҽ��Ŀ¼��Ӧ��ز����ӿں���
//��  ��������id��סԺ�ţ�ҽԺ��Ŀ���ҽԺ��Ŀ���룬ҽ���ѱ�ҽԺ��Ŀ����Ƿ񷵻�����(�˴�ӦΪ1)
//����ֵ����ѯ���
extern "C" __declspec(dllexport) void __cdecl DisplayMedicalInsuranceInfo(char* result, char* patient_id, char* visit_id, char* item_class, char* item_code, char* charge_type, char* item_spec)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	/*string tmpss = string(patient_id)+" "+string(visit_id)+" "+string(item_code)+" "+string(charge_type)+" "+string(item_class)+" ";
	AfxMessageBox(CString(tmpss.c_str()));
*/
	// ��⿪��
	if(!CUserModuleController::IsModuleOpen(TREATMENT_TIP_MODULE))
	{
		strcpy(result, "{state:1}{message:�����ɹ�}");
		return;
	}

	// �ж��Ƿ�ΪA,C,D,I��
	string itemClass(item_class);
	if(itemClass != "A" && itemClass != "C" && itemClass != "D" && itemClass != "I")
	{
		//AfxMessageBox(_T("����ACDI֮һ��Ӧ���أ�")+CString(item_class));
		strcpy(result, "{state:1}{message:�����ɹ���}");
		return;
	}

	//AfxMessageBox(_T("��ACDI֮һ��")+CString(item_class));
	// =================ͨ�����ݿ��ȡ��ѯ���=======================
	RESULT_MAP dbParameter, dbResult;
	// ��ʼ�������б�
	string chargeType(charge_type);
	string itemCode(item_code);
	string itemSpec(item_spec);

	map<string,string> param;
	param.insert(pair<string, string>("CHARGE_TYPE", chargeType)); 
	param.insert(pair<string, string>("ITEM_CLASS", itemClass)); 
	param.insert(pair<string, string>("ITEM_CODE", itemCode)); 
	param.insert(pair<string, string>("ITEM_SPEC", itemSpec)); 
	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ
		
		// ���Ҳ�����
		dbParameter = dao.Query("YYTSParameter", param);
		
		// ���������������ö�Ӧ��׼������ò���ֵ
		string ybjzk = "";
		RESULT_MAP::iterator iter = dbParameter.find("parameter");
		while(iter != dbParameter.end())
		{
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			for(;vecVecIter != iter->second.end(); ++vecVecIter)
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					// ��ȡҽ����׼��Ĳ���ֵ
					if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
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
			++iter;
		}
		// �ҵ���Ӧ��ҽ����׼�����ֵ����sql_config.xml�д��ڶ�Ӧ��ybjzkֵ��group��ѯ
		if(!ybjzk.empty())
			dbResult = dao.Query(ybjzk+"_YYTS", param);
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:���ݿ��������}");
		// �رն���
		dao.Close();
		return;
	}
	// �رն���
	dao.Close();

	// =================ͨ���ж��߼������ѯ����ó���ʾ����=================
	LogicHandler logicHandler;
	string displayInfo = logicHandler.HandleMedicalInsuranceInfoData(result, string(patient_id), string(visit_id), itemCode, dbParameter, dbResult, chargeType, itemClass);
	
	// =================��ʾ��ѯ������ڶ���=================
	if(displayInfo != "")
	{
		//CDisplayInfoDlg dlg;
		//// ������ʾ��Ϣ
		//dlg.SetResultInfo(result);
		//dlg.SetDisplayInfo(CString(displayInfo.c_str()));
		//dlg.DoModal();

		//Line 1  , load the skin
		//SkinStart("skins\\basic_urf\\office2007_af.urf",WINDOW_TYPE_VC,"",GTP_LOAD_FILE,NULL,NULL);
		// ��ȡƤ��
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		string resultStr = string(result);
		if(resultStr.find("state:3") != string::npos)
		{
			int a = AfxMessageBox(CString(displayInfo.c_str()), MB_YESNO|MB_ICONQUESTION);
			// ���滻���Է�ҩƷ
			if(a == IDNO) 
			{
				//replaceStr(result, "0{", "}"); // ȥ������0�����ѣ���־����Ŀ
				//replaceStr(result, "1{","1");
				string tmpStr(result);
				tmpStr.replace(1, 7, "state:6");
				strcpy(result,tmpStr.c_str());
			}
			// ���滻�ɱ�������Ŀ
			else
			{
				//replaceStr(result, "1{", "}"); // ȥ������1���Էѣ���־����Ŀ
				//replaceStr(result, "0{","0");
				string tmpStr(result);
				tmpStr.replace(1, 7, "state:5");
				strcpy(result, tmpStr.c_str());
			}
		}
		else
		{
			AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		}
		// �ͷ�Ƥ��
		skinppExitSkin();

		//Line 2  , cleanup
		//SkinRemove() ;
	}
}

//��  �ܣ�����ҽ��Ŀ¼��Ӧ��ز����ӿں���
//��  ��������id
//����ֵ����
extern "C" __declspec(dllexport) int __stdcall DisplayOutpatientServiceInfo(char* result, char* patient_id, char* item_code, char* zz_code)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// ��⿪��
	if(!CUserModuleController::IsModuleOpen(SERIOUS_TIP_MODULE))
	{
		strcpy(result, "{state:1}{message:�����ɹ�}");
		return 0;
	}
	// -------------------------------------------ҩƷ�滻----------------------------------------------------
	// =================ͨ�����ݿ��ȡ��ѯ���=======================
	RESULT_MAP dbResult, dbParameter;
	string itemCode(item_code);
	// ��ʼ�������б�
	/*string itemCodeStr(item_code);

	map<string,string> param;
	param.insert(pair<string, string>("ITEM_CODE", itemCodeStr)); 
	// ������ѯ����
	CSqlDao dao;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ
		
		// Ѱ�ҿ��ܵ��滻ҩƷ
		dbResult = dao.Query("MZ_REPLACE_ITEM", param);
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		strcpy(result, "{state:0}{message:���ݿ��������}");
		// �رն���
		dao.Close();
		return 0;
	}
	// �رն���
	dao.Close();

	// ���������������ö�Ӧ��׼������ò���ֵ
	string itemCode(item_code), tmpCode = "", tmpType = "";
	string itemCode1(item_code); // ԭʼitem_code
	RESULT_MAP::iterator iter = dbResult.find("replace_item");
	while(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// ��ȡitem_code
				if(vecIter->first == "ITEM_CODE")
				{
					tmpCode = vecIter->second;
				}
				// ��ȡtype
				else if(vecIter->first == "ZFBL")
				{
					tmpType = vecIter->second;
				}
			}
			if(atof(tmpType.c_str()) != 1.00) // ��Ϊ���滻��Ŀ
				itemCode = tmpCode;
		}
		++iter;
	}
*/
	string displayInfo = "";
	// ��֢����Ϊ�գ��򷵻�
	if(string(zz_code) == "")
	{
		// ��û�ҵ����滻��Ŀ
		if(itemCode == string(item_code))
		{
			strcpy(result, "{state:1}{message:�����ɹ���}");
			// ��ѯ�Ƿ�Ϊ�Է�
			map<string,string> param0; 
			param0.insert(pair<string, string>("ITEM_CODE", itemCode));
			// ������ѯ����
			CSqlDao dao0;
			try
			{
				// ��ʼ����ѯ����
				dao0.Init("MIES_db_config.conf","MIES_sql_config.xml");
				// ִ�в�ѯ
		
				// ���Ҳ�����
				RESULT_MAP dbResult0 = dao0.Query("MZFYCX", param0);

				RESULT_MAP::iterator iter = dbResult0.find("FYLX");
				double zfbl = 0;
				if(iter != dbResult0.end())
				{
					RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
					for(;vecVecIter != iter->second.end(); ++vecVecIter)
					{
						vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
						for(;vecIter!=vecVecIter->end(); ++vecIter)
						{
							if(vecIter->first == "ZFBL")
							{
								zfbl = atof(vecIter->second.c_str());
							}
						}
					}
				}
				if(zfbl == 1.00)
					displayInfo = "����Ŀ���Ϊ���Է�";
			}
			catch( runtime_error err )
			{
				// �쳣�������
				AfxMessageBox(CString(err.what()));
				strcpy(result, "{state:0}{message:���ݿ��������}");
				// �رն���
				dao0.Close();
				return 0;
			}
			// �رն���
			dao0.Close();
		}
		else	
		{
			strcpy(result, string("{state:1}{message:�����ɹ���}{itemCode:"+itemCode+"}").c_str());
			return 0;
		}
	}
	else
	{
		// -------------------------------------------��֢���-----------------------------------------------------
		// =================ͨ�����ݿ��ȡ��ѯ���=======================
		// ��ʼ�������б�
		map<string,string> param1;
		param1.insert(pair<string, string>("PATIENT_ID", string(patient_id))); 
		param1.insert(pair<string, string>("ITEM_CODE", itemCode));
		// ������ѯ����
		CSqlDao dao1;
		// ��������
		string limitDays = "";
		try
		{
			// ��ʼ����ѯ����
			dao1.Init("MIES_db_config.conf","MIES_sql_config.xml");
			// ִ�в�ѯ
		
			// ���Ҳ�����
			dbParameter = dao1.Query("MZYYParameter", param1);
			// ���������������ö�Ӧ��׼������ò���ֵ
			RESULT_MAP::iterator iter = dbParameter.find("parameter");
			while(iter != dbParameter.end())
			{
				RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
				for(;vecVecIter != iter->second.end(); ++vecVecIter)
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						// ��ȡ������֢��ҩ�����Ĳ���ֵ��15��
						if(vecIter->first == "NAME" && vecIter->second == "MZZZDAYS")
						{
							vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
							for(;vecIter!=vecVecIter->end(); ++vecIter)
							{
								if(vecIter->first == "VALUE")
									limitDays = vecIter->second;
							}
						}
					}
				}
				++iter;
			}

			if(!limitDays.empty())
			{
				char limitDateStr[256];
				CTime limitDate = CTime::GetCurrentTime() - CTimeSpan(atoi(limitDays.c_str()), 0, 0, 0);
				::wsprintfA(limitDateStr, "%ls", (LPCTSTR)limitDate.Format("%Y-%m-%d"));
				param1.insert(pair<string, string>("LIMIT_DATE", string(limitDateStr)));
				param1.insert(pair<string, string>("ITEM_CODE1", itemCode));
				dbResult = dao1.Query("MZYYTS", param1);
			}
		
		}
		catch( runtime_error err )
		{
			// �쳣�������
			AfxMessageBox(CString(err.what()));
			strcpy(result, "{state:0}{message:���ݿ��������}");
			// �رն���
			dao1.Close();
			return 0;
		}
		// �رն���
		dao1.Close();

		// =================ͨ���ж��߼������ѯ����ó���ʾ����=================
		LogicHandler logicHandler;
		displayInfo = logicHandler.HandleOutpatientServiceData(result, string(zz_code), limitDays, dbResult);
		// ���ҵ����滻��Ŀ
		if(itemCode != string(item_code))
		{
			string resultStr(result);
			resultStr += "{itemCode:"+itemCode+"}";
			strcpy(result, resultStr.c_str());
		}
	}
	// =================��ʾ��ѯ������ڶ���=================
	if(displayInfo != "")
	{
		//Line 1  , load the skin
		//SkinStart("skins\\basic_urf\\office2007_af.urf",WINDOW_TYPE_VC,"",GTP_LOAD_FILE,NULL,NULL);
		// ��ȡƤ��
		ifstream readSkin("skinConfig.ini", ios::in);
		string skin;
		readSkin>>skin;
		readSkin.close();
		skinppLoadSkin(const_cast<char*>(skin.c_str()));//bOzen Christmas DameK UltraBlue

		string resultStr = string(result);
		if(resultStr.find("state:3") != string::npos)
		{
			int a = AfxMessageBox(CString(displayInfo.c_str()), MB_YESNO|MB_ICONQUESTION);
			// �񣺲�����������������������
			if(a == IDNO) 
			{
				replaceStr(result, "{state:3", "}"); // ȥ������3��־����Ŀ
				string tmpStr(result);
				tmpStr = "{state:2}" + tmpStr;
				strcpy(result, tmpStr.c_str());
			}
			// �ǣ�����
			else
			{
				replaceStr(result, "{state:3", "}"); // ȥ������3��־����Ŀ
				string tmpStr(result);
				tmpStr = "{state:1}" + tmpStr;
				strcpy(result, tmpStr.c_str());
			}
		}
		else
		{
			AfxMessageBox(CString(displayInfo.c_str()), MB_OK|MB_ICONINFORMATION);
		}
		//Line 2  , cleanup
		//SkinRemove() ;
		// �ͷ�Ƥ��
		skinppExitSkin();
	}
	
	return 0;
}

//��  �ܣ���ӡ�Է�֪��ͬ����ӿں���
//��  ��������id
//����ֵ����
extern "C" __declspec(dllexport) void __cdecl PrintSelfChargeConsentLetter(char* result, char* patient_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//Line 1  , load the skin
	//SkinStartW(_T("skins\\basic_urf\\bluepinna.urf"),WINDOW_TYPE_VC,_T(""),GTP_LOAD_FILE,NULL,NULL);
	//SkinWindowSetEx(NULL,SKIN_SET_UNICODE_URF_FONT,1,0,NULL,0,NULL);

	// �򿪴�ӡ����
	// ��ȡƤ��
	ifstream readSkin("skinConfig.ini", ios::in);
	string skin;
	readSkin>>skin;
	readSkin.close();
	skinppLoadSkin(const_cast<char*>(skin.c_str()));
	
	// ������ʾ��Ϣ
	CPrintSelfConsentDlg dlg; 
	dlg.m_patientId = CString(patient_id);
	if(dlg.Init())
	{
		dlg.DoModal();
	}
	// �ͷ�Ƥ��
	skinppExitSkin();
	
}

//��  �ܣ���ӡÿ���嵥�ӿں���
//��  ��������id
//����ֵ����
extern "C" __declspec(dllexport) void __cdecl PrintDailyList(char* patient_id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDailyListDlg dlg;
	dlg.m_patientId = CString(patient_id);
	// ������ʾ��Ϣ
	dlg.DoModal();
}


