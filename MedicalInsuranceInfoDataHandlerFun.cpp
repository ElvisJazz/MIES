///////////////////////////////////////////////////////////////////////
//�ļ����� LogicHandler.cpp
//��  �ܣ� ҽ�����ϵͳ���ݿ��ȡ�����߼���������ҩ��ʾ����ʵ���ļ�
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//��  �ܣ�������ҩ��ʾ��ҽ��Ŀ¼����Ӧ�����ݿ�����
//��  ���������Ľ���ַ���ָ�룬���ݿ��ѯ����
//����ֵ��������ʾ��Ϣ
string LogicHandler::HandleMedicalInsuranceInfoData(char* result, string patientId, string visitId, string item_code, RESULT_MAP dbParameter, RESULT_MAP dbResult, string chargeType, string classCode)
{
	string displayInfo = "";
	string item_name = ""; // item_code��Ӧ����
	
	// ===================��ȡ���������ݿ���ֵ���ò���===================
	// ���������������ö�Ӧ��׼������ò���ֵ
	string ybjzk = ""; // ҽ����׼��
	string jzkfycs = ""; // ��׼����ò���
	string fylbz = ""; // ��Ŀ�������ֵ�����ࡢ���ࡢ�Էѵȶ�Ӧ��ֵ��
	string limitInfo = ""; // ����������
	string tipFlag = ""; // ҽ����׼�������ʾ
	string jxsgfy_cs = ""; // �����й����ò���
	string jctcts_cs = ""; // ����ײ���ʾ����

	// ��ȡ����
	RESULT_MAP::iterator iter = dbParameter.find("parameter");
	while(iter != dbParameter.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// ��ȡ��Ӧ�Ĳ���ֵ
				if(vecIter->first == "NAME" && vecIter->second == "YBJZK")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							ybjzk = vecIter->second;
					}
				}
				else if(vecIter->first == "NAME" && vecIter->second == "JZKFYCS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							jzkfycs = vecIter->second;
					}
				}
				else if(vecIter->first == "NAME" && vecIter->second == "JXSGFY_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							jxsgfy_cs = vecIter->second;
					}
				}
				else if(vecIter->first == "NAME" && vecIter->second == "JCTCTS") // ����ײ���ʾ
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							jctcts_cs = vecIter->second;
					}
				}
			}
		}
		++iter;
	}
	
	// ===================�����Ŀ��ѯ===================
	//AfxMessageBox(_T("����ģ��")+CString(jctcts_cs.c_str()));
	if(classCode=="C" && jctcts_cs!="")
	{
		string dateTime = "";
		// ���ò���
		map<string,string> tcParam;
		tcParam.insert(pair<string, string>("PATIENT_ID", patientId)); 
		tcParam.insert(pair<string, string>("VISIT_ID", visitId)); 
		tcParam.insert(pair<string, string>("ITEM_CODE", item_code));
		// ������ѯ����
		CSqlDao tcDao;
		RESULT_MAP tcDBResult;
		try
		{
			// ��ʼ����ѯ����
			tcDao.Init("MIES_db_config.conf","MIES_sql_config.xml");
			// ִ�в�ѯ
			tcDBResult = tcDao.Query("JCTC_YYTS", tcParam);
		
			// ��ȡ�����ײ͵�����
			RESULT_MAP::iterator iter = tcDBResult.find("YYTS");
			if(iter != tcDBResult.end())
			{
				RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
				if(vecVecIter != iter->second.end())
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					if(vecIter!=vecVecIter->end())
					{
						// ��ȡ����
						if(vecIter->first == "DATE_TIME")
						{
							dateTime = vecIter->second;
						}
					}
				}
			}
			
		}
		catch( runtime_error err )
		{
			// �쳣�������
			AfxMessageBox(CString(err.what()));
			sprintf(result, "{state:0}{message:���ݿ��������}");
			// �رն���
			tcDao.Close();
			return displayInfo;
		}
		// �رն���
		tcDao.Close();
		// ��ѯ��֮ǰ�������Ƽ����Ŀ
		//AfxMessageBox(_T("����")+CString(dateTime.c_str()));
		if(dateTime != "")
		{
			int offsetDays = CalculateTimeDifference(dateTime) / 24;
			string tmpStr = jctcts_cs;
			string::size_type index1 = tmpStr.find("<time>");
			tmpStr = tmpStr.replace(index1, 6, dateTime);
			index1 = tmpStr.find("<days>");
			char days[8];
			sprintf(days, "%d", offsetDays);
			tmpStr = tmpStr.replace(index1, 6, string(days));
			displayInfo = tmpStr+"\n";
		}
	}

	// ===================���ݲ�ͬ�Ļ�׼�������Ӧ�Ĳ���==================
	iter = dbResult.find("YYTS");
	int itemState = 1; // �����Ŀ״̬��־λ��1�޺�����Ϊ��2���ڻ�׼�⣬������3���人ҽ����׼�⣬������Ҫ�滻
	
	// ===================�人ҽ����׼��===================
	if(ybjzk == "YB_JZK_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "ZFBL") // �Էѱ���
						fylbz = vecIter->second;
					else if(vecIter->first == "SM" ) // ����������
					{
						if(vecIter->second.length()!=0 && vecIter->second!="��")
						{
							limitInfo = vecIter->second;
							displayInfo = "��ʾ��Ϣ��" + limitInfo + "\n";
						}
					}
					else if(vecIter->first == "TIP_FLAG") // ҽ����׼�������ʾ
					{
						tipFlag = vecIter->second;
						if(classCode=="C" || classCode=="D")
						{
							if(tipFlag=="1")
								displayInfo += "��ܿ۷ѷ��գ�����Բ�������ʹ�ã�";
							sprintf(result, "{state:1}{message:�����ɹ�}");
							return displayInfo;
						}
					}
					else if(vecIter->first == "ITEM_NAME") // ��Ŀ����
					{
						item_name = vecIter->second;
					}
				}
			}
			else // ���ڻ�׼��,������
			{
				if(classCode=="C" || classCode=="D")
				{
					//displayInfo = "";
					sprintf(result, "{state:1}{message:�����ɹ�}");
					return displayInfo;
				}
				else
					itemState = 2;
			}
		}
		else // ���ڻ�׼��,������
		{
			if(classCode=="C" || classCode=="D")
			{
				//displayInfo = "";
				sprintf(result, "{state:1}{message:�����ɹ�}");
				return displayInfo;
			}
			else
				itemState = 2;
		}
	}
	else if(classCode != "A" && classCode != "I")  // ����ҽ����׼����classCode��ΪA
	{
		sprintf(result, "{state:1}{message:�����ɹ�}");
		return "";
	}
	// ===================��·ҽ����׼��===================
	else if(ybjzk == "TB_JZK_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BXBZ") // �Էѱ���
						fylbz = vecIter->second;
					else if(vecIter->first == "SM" ) // ����������
					{
						if(vecIter->second.length()!=0 && vecIter->second!="��")
						{
							limitInfo = vecIter->second;
							displayInfo = "��ʾ��Ϣ��" + limitInfo + "\n";
						}
					}
					else if(vecIter->first == "ITEM_NAME") // ��Ŀ����
					{
						item_name = vecIter->second;
					}
				}
			}
			else // ���ڻ�׼��,������
			{
				itemState = 2;
			}
		}
		else // ���ڻ�׼��,������
		{
			itemState = 2;
		}

	}
	// ===================ʡҽ����׼��===================
	else if(ybjzk == "SYB_JZK_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BX") // �Էѱ���
						fylbz = vecIter->second;
					else if(vecIter->first == "SM" ) // ����������
					{
						if(vecIter->second.length()!=0 && vecIter->second!="��")
						{
							limitInfo = vecIter->second;
							displayInfo = "��ʾ��Ϣ��" + limitInfo + "\n";
						}
					}
					else if(vecIter->first == "ITEM_NAME") // ��Ŀ����
					{
						item_name = vecIter->second;
					}
				}
			}
			else // ���ڻ�׼��,������
			{
				itemState = 2;
			}			
		}
		else // ���ڻ�׼��,������
		{
			itemState = 2;
		}

	}
	// ===================��ũ��ҽ����׼��===================
	else if(ybjzk == "XNH_JZK_VS_PRICE_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "WIPEOUT") // �Էѱ���
						fylbz = vecIter->second;
				}
			}
			else // ���ڻ�׼��,������
			{
				itemState = 2;
			}
		}
		else // ���ڻ�׼��,������
		{
			itemState = 2;
		}

	}
	//===================ʡ�й��ѡ����ݻ�׼��===================
	else if(ybjzk == "CHARGE_SPECIAL_ITEM_DICT")
	{
		// ����ڻ�׼��
		string fenzi = "", fenmu = ""; // �������ӡ���ĸ
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "PROPORTION_NUMERATOR") // �Ը���������
						fenzi = vecIter->second;
					else if(vecIter->first == "PROPORTION_DENOMINATOR") // �Ը�������ĸ
						fenmu = vecIter->second;
				}
			}

			if(!fenzi.empty() && !fenmu.empty() && !jxsgfy_cs.empty() )
			{
				char infoStr[256] = "";
				if(atof(fenmu.c_str()) != 0)
				{
					double blz = atof(fenzi.c_str())/atof(fenmu.c_str());

					if(blz == 1.0)
						sprintf(infoStr, ("����Ŀ���Ϊ��"+chargeType+"���Էѡ�").c_str());
					else if(blz > atof(jxsgfy_cs.c_str()))
						sprintf(infoStr, ("����Ŀ���Ϊ��"+chargeType+"���Ը�����Ϊ��%.2f%��").c_str(), blz*100);
				}
				else
					sprintf(infoStr, ("����Ŀ���Ϊ��"+chargeType+"�����Ը�������ĸΪ0�����ʵ�Ƿ�����").c_str());
				displayInfo = string(infoStr);
			}
			
		}
	}

	// ===================����Ƿ����Է�ҩƷ������ڻ�׼���ҷ�ʡ�й��ѡ����ݻ�׼������===================
	int currentType = 0; // ��ǰ��Ŀ���ͣ�Ĭ�Ͽɱ��� 
	string tmpDisplayInfo;
	if(itemState!=2 && !ybjzk.empty() && ybjzk!="CHARGE_SPECIAL_ITEM_DICT")
	{
		jzkfycs.append(";");
		string::size_type index = jzkfycs.find(fylbz+":");
		if(fylbz!="" && index!=string::npos)
		{
			string::size_type index2 = jzkfycs.find(";", index);
			// ��ȡ�����жϵĲ�����,ʾ������ .10:����,0;0.00:����,0;-1:�Է�,1; �н�ȡ .10:����,0
			string checkStr = jzkfycs.substr(index, index2-index);
			// ��ȡ��Ӧ�������Ϣ
			index = checkStr.find(":", 0);
			index2 = checkStr.find(",", index);
			if(index!=string::npos && index2!=string::npos)
			{
				if((checkStr.substr(index+1, index2-index-1)=="�Է�" && classCode=="I") || classCode != "I")
					tmpDisplayInfo = "����Ŀ���Ϊ��" + checkStr.substr(index+1, index2-index-1);
				// �ж��Ƿ����Է���Ŀ��Ϊ�人\����\ʡҽ����׼����Ŀ
				if(ybjzk == "YB_JZK_DICT" || ybjzk == "TB_JZK_DICT" || ybjzk == "SYB_JZK_DICT") //atoi(checkStr.substr(index2+1).c_str())==1 && 
					itemState = 3;
			}
			else
			{
				displayInfo = "��ҩ��ʾ���ò�������";
			}
		}
		else // ������Ŀ������������������Ӧ����
		{
			index = jzkfycs.find("-1");
			string::size_type index2 = jzkfycs.find(";", index);
			// ��ȡ�����жϵĲ�����,ʾ������ .10:����,0;0.00:����,0;-1:�Է�,1; �н�ȡ .10:����,0			
			if(index!=string::npos && index2!=string::npos)
			{
				string checkStr = jzkfycs.substr(index, index2-index);
				// ��ȡ��Ӧ�������Ϣ
				index = checkStr.find(":", 0);
				index2 = checkStr.find(",", index);
				if(index!=string::npos && index2!=string::npos)
				{
					if((checkStr.substr(index+1, index2-index-1)=="�Է�" && classCode=="I") || classCode != "I")
						tmpDisplayInfo = "����Ŀ���Ϊ��" + checkStr.substr(index+1, index2-index-1);
					//displayInfo = "����Ŀ���Ϊ��" + checkStr.substr(index+1, index2-index-1) + "\n" + displayInfo;
					// �ж��Ƿ����Է���Ŀ��Ϊ�人\����\ʡҽ����׼����Ŀ
					if(ybjzk == "YB_JZK_DICT" || ybjzk == "TB_JZK_DICT" || ybjzk == "SYB_JZK_DICT") //atoi(checkStr.substr(index2+1).c_str())==1 && 
						itemState = 3;
					if(atoi(checkStr.substr(index2+1).c_str()) == 1 )
						currentType = 1; // �Է�
				}
				else
				{
					displayInfo = "��ҩ��ʾ���ò�������";
				}
			}
			else
			{
				displayInfo = "��ҩ��ʾ���ò�������";
			}
		}
	}

	// ����ǲ���,���ϲ�������
	if(classCode == "I")
		itemState = 1;
	//������Է�ҩƷ��ȥyb_special_control_dict����Ѱ�ҹ���ҩƷ,ֻ�������������Ҫ���صĴ��滻����Ŀ����	
	string resultStr = "";
	switch(itemState)
	{
	case 1: 
			resultStr = "{state:1}{message:�����ɹ�}";
			if(tmpDisplayInfo.length() > 0)
				displayInfo = tmpDisplayInfo + "\n" + displayInfo;
			sprintf(result, resultStr.c_str());
			break;
	case 2: resultStr = "{state:2}{message:�����ɹ�}";
			sprintf(result, resultStr.c_str());
			displayInfo = "����Ŀ���ڻ�׼���У�����ϵ��ؿ��ң�";
			break;
	case 3: // Ѱ�ҿ��滻��Ŀ
		if(limitInfo != "")
		{
			resultStr = "{state:3}{message:�����ɹ�}";
			displayInfo += "������Ϣ������Ŀ�Ƿ����������������";

			// ������ѯ����
			if(currentType == 1)
			{
				RESULT_MAP itemDbResult;
				string itemCode = "", itemName = "", type="";
				CSqlDao dao;
				try
				{
					// ��ʼ����ѯ����
					dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
					map<string,string> param;
					param.insert(pair<string, string>("ITEM_CODE", item_code));
					// ִ�в�ѯ
					itemDbResult = dao.Query("REPLACE_ITEM", param);
				}
				catch( runtime_error err )
				{
					// �쳣�������
					AfxMessageBox(CString(err.what()));
					sprintf(result, "{state:0}{message:���ݿ��������}");
					// �رն���
					dao.Close();
					return "";
				}
				// �رն���
				dao.Close();

				// ��ȡ��ѯ����item_name��item_code
				iter = itemDbResult.find("replace_item");
				if(iter != itemDbResult.end())
				{
					for(;iter!=itemDbResult.end(); ++iter)
					{
						// �ҵ���Ŀ����ȡ��Ӧ����
						RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
						if(iter->second.size() ==2 && ybjzk == "YB_JZK_DICT")
						{
							//resultStr = "{state:3}{message:�����ɹ�}";
							//displayInfo += "������Ϣ������Ŀ�Ƿ����������������";

							for(;vecVecIter != iter->second.end(); ++vecVecIter)
							{
								vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
								for(;vecIter!=vecVecIter->end(); ++vecIter)
								{
									if(vecIter->first == "ITEM_CODE") // ��Ŀ����
									{
										itemCode = vecIter->second;
										if(item_code != itemCode)
										{
											type = "0";
											displayInfo = "����Ŀ���Ϊ��" + GetFeeType(ybjzk, jzkfycs, classCode, itemCode) + "\n" + displayInfo;
										}
										else
											type = "1";
									}
									else if(vecIter->first == "ITEM_NAME") // ��Ŀ����
										itemName = vecIter->second;
									//else if(vecIter->first == "TYPE") // ��Ŀ����
									//	type = vecIter->second;
								}
								if(type == "0")
									resultStr += ("{item[code:"+itemCode+"##name:"+itemName+"]}");
							}
						}
						else // δ�ҵ�������Ŀ��¼,����ʾѯ����Ϣ
						{
							//resultStr += ("{itemCode:"+item_code+"}");
							displayInfo = tmpDisplayInfo+ "\n" + displayInfo;
							resultStr = "{state:3}{message:�����ɹ�}"+("{item[code:"+item_code+"##name:"+item_name+"]}");
						}
					}
				}
				// δ�ҵ���Ŀ��¼,����ʾѯ����Ϣ
				else
				{
					displayInfo = tmpDisplayInfo + "\n" + displayInfo;
					resultStr = "{state:3}{message:�����ɹ�}"+("{item[code:"+item_code+"##name:"+item_name+"]}");
				}
			}
			else
			{
				displayInfo = tmpDisplayInfo + "\n" + displayInfo;
				resultStr = "{state:3}{message:�����ɹ�}"+("{item[code:"+item_code+"##name:"+item_name+"]}");
			}
		}
		else
		{
			displayInfo = tmpDisplayInfo+ + "\n" + displayInfo;
			resultStr = "{state:1}{message:�����ɹ�}";
		}

		strcpy(result, resultStr.c_str());
		break;

	default: break;
	}
	
	return displayInfo;

}
	
//��  �ܣ�����人ҽ����ʡҽ����������ȡ��Ŀ��������
//��  ������׼�⣬��׼����ò�������Ŀ�����룬��Ŀ����
//����ֵ��������Ŀ��������
string LogicHandler::GetFeeType(string ybjzk, string jzkfycs, string itemClass, string itemCode)
{
	if(ybjzk=="XNH_JZK_VS_PRICE_DICT" && ybjzk=="CHARGE_SPECIAL_ITEM_DICT")
		return "";

	map<string,string> param; 
	param.insert(pair<string, string>("ITEM_CLASS", itemClass)); 
	param.insert(pair<string, string>("ITEM_CODE", itemCode)); 
	// ������ѯ����
	CSqlDao dao;
	RESULT_MAP dbParameter, dbResult;
	try
	{
		// ��ʼ����ѯ����
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// ִ�в�ѯ
		
		
		// �ҵ���Ӧ��ҽ����׼�����ֵ����sql_config.xml�д��ڶ�Ӧ��ybjzkֵ��group��ѯ
		dbResult = dao.Query(ybjzk+"_YYTS", param);
	}
	catch( runtime_error err )
	{
		// �쳣�������
		AfxMessageBox(CString(err.what()));
		// �رն���
		dao.Close();
		return "";
	}
	dao.Close();

	string fylbz("");
	RESULT_MAP::iterator iter = dbResult.find("YYTS");
	// ===================�人ҽ����׼��===================
	if(ybjzk == "YB_JZK_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "ZFBL") // �Էѱ���
						fylbz = vecIter->second;
				}
			}
			else // ���ڻ�׼��,������
			{
				AfxMessageBox(CString((itemCode+"δ���人ҽ����׼�����ҵ��滻��Ķ�Ӧ��Ŀ��").c_str()));
				return "";
			}
		}
		else // ���ڻ�׼��,������
		{
			AfxMessageBox(CString((itemCode+"δ���人ҽ����׼�����ҵ��滻��Ķ�Ӧ��Ŀ��").c_str()));
			return "";
		}
	}
	// ===================��·ҽ����׼��===================
	else if(ybjzk == "TB_JZK_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BXBZ") // �Էѱ���
						fylbz = vecIter->second;
				}
			}
			else // ���ڻ�׼��,������
			{
				AfxMessageBox(CString((itemCode+"δ��������׼�����ҵ��滻��Ķ�Ӧ��Ŀ��").c_str()));
				return "";
			}
		}
		else // ���ڻ�׼��,������
		{
			AfxMessageBox(CString((itemCode+"δ��������׼�����ҵ��滻��Ķ�Ӧ��Ŀ��").c_str()));
			return "";
		}

	}
	// ===================ʡҽ����׼��===================
	else if(ybjzk == "SYB_JZK_DICT")
	{
		// ����ڻ�׼��
		if(iter != dbResult.end())
		{
			// ��ȡ��Ӧ����
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BX") // �Էѱ���
						fylbz = vecIter->second;
				}
			}
			else // ���ڻ�׼��,������
			{
				AfxMessageBox(CString((itemCode+"δ��ʡҽ����׼�����ҵ��滻��Ķ�Ӧ��Ŀ��").c_str()));
				return "";
			}			
		}
		else // ���ڻ�׼��,������
		{
			AfxMessageBox(CString((itemCode+"δ��ʡҽ����׼�����ҵ��滻��Ķ�Ӧ��Ŀ��").c_str()));
			return "";
		}

	}
	
	// ��ȡ��Ŀ�������
	jzkfycs.append(";");
	string::size_type index = jzkfycs.find(fylbz+":");
	if(fylbz!="" && index!=string::npos)
	{
		string::size_type index2 = jzkfycs.find(";", index);
		// ��ȡ�����жϵĲ�����,ʾ������ .10:����,0;0.00:����,0;-1:�Է�,1; �н�ȡ .10:����,0
		string checkStr = jzkfycs.substr(index, index2-index);
		// ��ȡ��Ӧ�������Ϣ
		index = checkStr.find(":", 0);
		index2 = checkStr.find(",", index);
		if(index!=string::npos && index2!=string::npos)
		{
			return checkStr.substr(index+1, index2-index-1);
		}
		else
		{
			AfxMessageBox(_T("��ҩ��ʾ���ò�������"));
			return "";
		}
	}
	else // ������Ŀ������������������Ӧ����
	{
		index = jzkfycs.find("-1");
		string::size_type index2 = jzkfycs.find(";", index);
		// ��ȡ�����жϵĲ�����,ʾ������ .10:����,0;0.00:����,0;-1:�Է�,1; �н�ȡ .10:����,0			
		if(index!=string::npos && index2!=string::npos)
		{
			string checkStr = jzkfycs.substr(index, index2-index);
			// ��ȡ��Ӧ�������Ϣ
			index = checkStr.find(":", 0);
			index2 = checkStr.find(",", index);
			if(index!=string::npos && index2!=string::npos)
			{
				return checkStr.substr(index+1, index2-index-1);
			}
			else
			{
				AfxMessageBox(_T("��ҩ��ʾ���ò�������"));
				return "";
			}
		}
		else
		{
			AfxMessageBox(_T("��ҩ��ʾ���ò�������"));
			return "";
		}
	}
	
}
