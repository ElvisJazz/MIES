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


//��  �ܣ�����Ԥ�����ѯ�����ݿ�����
//��  ���������Ľ���ַ���ָ�룬���ݿ��ѯ���ݣ�Ԥ�����ѯ����
//����ֵ����
void LogicHandler::HandlePatientPreSettlementInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult, RESULT_MAP dbResult2)
{
	string dataResult = "";
	string title = "ҽ��";
	string grzf_yjj_cs = ""; // �����Ը�/Ԥ�����������
	string tczfhj_ybzfyjz_cs = ""; // ͳ��֧���ϼ�/ҽ��֧��Ԥ��ֵ��������
	string sbpf_cs = ""; // �̱��⸶
	string ndylfy_cs = ""; // ����ۼ�ҽ�����ò���
	string zfybl_cs = ""; // �Է��ñ�������
	string zfyzb_cs = ""; // �Է�ҩƷ��������

	double grzf = 0.0; // �����Ը�
	double yjj = 0.0; // Ԥ����
	double tczfhj = 0.0; // ͳ��֧���ϼ�
	double ybzfyjz = 0.0; // ҽ��֧��Ԥ��ֵ
	double sbpf = 0.0; // �̱��⸶
	double ndylfy = 0.0; // ����ۼ�ҽ������
	double zffybl = 0.0; // �Ը����ñ���

	// ��ɫӳ��
	string whiteStr = "FFFFFF";
	string greenStr = "008000";

	// ��ȡ����
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
				// �����Ը�/Ԥ�����������
				if(vecIter->first == "NAME" && vecIter->second == "GRZF_YJJ_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							grzf_yjj_cs = vecIter->second;
					}
				}
				// ͳ��֧���ϼ�/ҽ��֧��Ԥ��ֵ��������
				else if(vecIter->first == "NAME" && vecIter->second == "TCZFHJ_YBZFYJZ_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							tczfhj_ybzfyjz_cs = vecIter->second;
					}
				}
				// �̱��⸶
				else if(vecIter->first == "NAME" && vecIter->second == "SBPF_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							sbpf_cs = vecIter->second;
					}
				}
				// ����ۼ�ҽ�����ò���
				else if(vecIter->first == "NAME" && vecIter->second == "NDYLFY_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							ndylfy_cs = vecIter->second;
					}
				}
				// �Ը����ñ���
				else if(vecIter->first == "NAME" && vecIter->second == "ZFYBL_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							zfybl_cs = vecIter->second;
					}
				}
				// �Ը�ҩռ����
				else if(vecIter->first == "NAME" && vecIter->second == "ZFYZB_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							zfyzb_cs = vecIter->second;
					}
				}
				// ��ȡ��ʾ����
				else if(vecIter->first == "NAME" && vecIter->second == "TITLE")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							title = vecIter->second;
					}
				}
				// ��ȡ������ɫ1
				else if(vecIter->first == "NAME" && vecIter->second == "BASIC_COLOR1")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							whiteStr = vecIter->second;
					}
				}
				// ��ȡ������ɫ2
				else if(vecIter->first == "NAME" && vecIter->second == "BASIC_COLOR2")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							greenStr = vecIter->second;
					}
				}
			}
		}
	}
	
	// ����title
	dataResult += "{title:";
	dataResult += title;
	dataResult += "}";
	iter = dbResult.find("PreSettlementInfo");

	bool isWhite = true;

	if(iter != dbResult.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				dataResult += "{";
				dataResult.append(GetChineseName(vecIter->first));
				dataResult += ":";
				dataResult.append(vecIter->second);
				
				if(vecIter->first == "PREPAYMENTS")
					yjj = atof(vecIter->second.c_str());
				else if(vecIter->first == "GRZF")
					grzf = atof(vecIter->second.c_str());
				else if(vecIter->first == "TCZFHJ")
					tczfhj = atof(vecIter->second.c_str());
				else if(vecIter->first == "YBZFYJZ")
					ybzfyjz = atof(vecIter->second.c_str());
				else if(vecIter->first == "SBPFHJ")
					sbpf = atof(vecIter->second.c_str());
				else if(vecIter->first == "NDYLFY")
					ndylfy = atof(vecIter->second.c_str());
				else if(vecIter->first == "GRFDL")
					zffybl = atof(vecIter->second.c_str());

				if(isWhite)
					dataResult.append("<<"+whiteStr+">>");
				else
					dataResult.append("<<"+greenStr+">>");
				isWhite = !isWhite;

				dataResult += "}";
			}
		}
	}
	// --------------------------------�����Ƿ��жԶ�Ӧ����������ֵ���---------------------------------------
	// �����Ը�/Ԥ����������
	if(!grzf_yjj_cs.empty())
	{
		if(yjj != 0)
			ReplaceColorStr(dataResult, "PREPAYMENTS", grzf_yjj_cs, grzf/yjj);
	}

	// ͳ��֧���ϼ�/ҽ��֧��Ԥ��ֵ�������
	if(!tczfhj_ybzfyjz_cs.empty())
	{
		if(ybzfyjz != 0)
			ReplaceColorStr(dataResult, "TCZFHJ", tczfhj_ybzfyjz_cs, tczfhj/ybzfyjz);
	}

	// �̱��⸶���
	if(!sbpf_cs.empty())
	{
		ReplaceColorStr(dataResult, "SBPFHJ", sbpf_cs, sbpf);
	}

	// ����ۼ�ҽ���������
	if(!ndylfy_cs.empty())
	{
		ReplaceColorStr(dataResult, "NDYLFY", ndylfy_cs, ndylfy);
	}

	// �Է��ñ������
	if(!zfybl_cs.empty())
	{
		ReplaceColorStr(dataResult, "GRFDL", zfybl_cs, zffybl);
	}

	// --------------------------------���Է�ҩռ����Ŀռ�Ȳ����������򲻼����������(ֻ����ũ��)---------------------------------------
	if(zfyzb_cs.empty())
	{
		dataResult = "{state:1}{message:�����ɹ�}"+dataResult;
		strcpy(result, dataResult.c_str());
		return;
	}
	// ===================�Է�ҩռ�����===================
	string publicDrugChargeStr = "", allDrugChargeStr = "";
	// ��ȡ����ҩƷ����
	iter = dbResult2.find("public_drug_charge");
	if(iter != dbResult2.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "PUBLIC_DRUG_CHARGE")
					publicDrugChargeStr = vecIter->second;
			}
		}
	}
	// ��ȡ����ҩƷ����
	iter = dbResult2.find("all_drug_charge");
	if(iter != dbResult2.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "ALL_DRUG_CHARGE")
					allDrugChargeStr = vecIter->second;
			}
		}
	}
	// �����Է�ҩƷ���ñ���
	if(!allDrugChargeStr.empty() || !publicDrugChargeStr.empty())
	{
		double allDrugCharge = atof(allDrugChargeStr.c_str());
		if(allDrugCharge != 0)
		{
			double selfDrugChargeRate = (atof(allDrugChargeStr.c_str())-atof(publicDrugChargeStr.c_str())) / allDrugCharge;
			char* selfDrugChargeRateStr = new char[16];
			//char* self_drug_charge_rate_str = new char[16];
			sprintf(selfDrugChargeRateStr, "%.2f%", selfDrugChargeRate*100);
			dataResult += "{�Է�ҩռ��:"+string(selfDrugChargeRateStr)+"%";
			// ��ȡ�޶�
			string zfyzbCs = zfyzb_cs.substr(0, zfyzb_cs.length()-1);
			string::size_type index00 = zfyzbCs.find_last_of(";");
			string::size_type index11 = zfyzbCs.find(":", index00);
			if(index00!=string::npos && index11!=string::npos)
			{
				string limitValueStr = zfyzb_cs.substr(index00+1, index11-index00-1);
				double limitValue = atof(limitValueStr.c_str());
				
				if(selfDrugChargeRate > limitValue)
				{
					char* offsetStr = new char[16];
					char* limitValueCStr = new char[16];
					sprintf(limitValueCStr, "%.2f%", limitValue*100);
					sprintf(offsetStr, "%.2f%", (selfDrugChargeRate-limitValue)*100);
					dataResult.append("���޶�("+string(limitValueCStr)+"),���޶����Ϊ("+string(offsetStr)+")��");
				}
			}
			// ��ȡ��ɫ
			string colorStr = GetColorStr(zfyzb_cs, selfDrugChargeRate);
			if(colorStr != "")
				dataResult += "<<"+colorStr+">>";
			else if(isWhite)
			{
				dataResult.append("<<"+whiteStr+">>");
				isWhite = !isWhite;
			}
			else
			{
				dataResult.append("<<"+greenStr+">>");
				isWhite = !isWhite;
			}
			dataResult += "}";// (����ҩƷ����"+publicDrugChargeStr+"Ԫ����ҩƷ����"+allDrugChargeStr+"Ԫ)
		}
		else
		{
			dataResult += "{�Է�ҩռ��:��ѯ���û��߱�������ҩƷ�����ܶ�Ϊ0Ԫ�����ʵ�Ƿ�����}";
		}
	}

	// ===================�Է���Ŀռ�����===================
	string publicItemChargeStr = "", allItemChargeStr = "";
	// ��ȡ������Ŀ����
	iter = dbResult2.find("public_item_charge");
	if(iter != dbResult2.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "PUBLIC_ITEM_CHARGE")
					publicItemChargeStr = vecIter->second;
			}
		}
	}
	// ��ȡ������Ŀ����
	iter = dbResult2.find("all_item_charge");
	if(iter != dbResult2.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		if(vecVecIter != iter->second.end())
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				if(vecIter->first == "ALL_ITEM_CHARGE")
					allItemChargeStr = vecIter->second;
			}
		}
	}
	// �����Է���Ŀ���ñ���
	if(!allItemChargeStr.empty() || !publicItemChargeStr.empty())
	{
		double allItemCharge = atof(allItemChargeStr.c_str());
		if(allItemCharge != 0)
		{
			double selfItemChargeRate = (atof(allItemChargeStr.c_str())-atof(publicItemChargeStr.c_str())) / allItemCharge;
			char* selfItemChargeRateStr = new char[16];
			char* self_item_charge_rate_str = new char[16];
			sprintf(selfItemChargeRateStr, "%.2f%", selfItemChargeRate*100);
			//sprintf(self_item_charge_rate_str, "%.2f%", self_item_charge_rate*100);
			dataResult += "{�Է���Ŀռ��:"+string(selfItemChargeRateStr)+"%";
			/*if(self_item_charge_rate!=-1 && selfItemChargeRate>self_item_charge_rate)
				dataResult += "���ѳ���("+string(self_item_charge_rate_str)+"%)��";*/

			// ��ȡ��ɫ
			if(isWhite)
				dataResult.append("<<"+whiteStr+">>");
			else
				dataResult.append("<<"+greenStr+">>");
			isWhite = !isWhite;

			dataResult += "}";//(������Ŀ����"+publicItemChargeStr+"Ԫ������Ŀ����"+allItemChargeStr+"Ԫ)
		}
		else
		{
			dataResult += "{�Է���Ŀռ��:��ѯ���û��߱���������Ŀ�����ܶ�Ϊ0Ԫ�����ʵ�Ƿ�����}";
		}
	}
	// ���ؽ�����ݺ���ʾ����
	dataResult = "{state:1}{message:�����ɹ�}"+dataResult;
	strcpy(result, dataResult.c_str());
}
 