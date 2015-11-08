///////////////////////////////////////////////////////////////////////
//文件名： LogicHandler.cpp
//功  能： 医保审查系统数据库获取数据逻辑处理类实现文件
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//功  能：处理预结算查询的数据库数据
//参  数：处理后的结果字符串指针，数据库查询数据，预计算查询参数
//返回值：无
void LogicHandler::HandlePatientPreSettlementInfoData(char* result, RESULT_MAP dbParameter, RESULT_MAP dbResult, RESULT_MAP dbResult2)
{
	string dataResult = "";
	string title = "医保";
	string grzf_yjj_cs = ""; // 个人自付/预交金比例参数
	string tczfhj_ybzfyjz_cs = ""; // 统筹支付合计/医保支付预警值比例参数
	string sbpf_cs = ""; // 商保赔付
	string ndylfy_cs = ""; // 年度累计医保费用参数
	string zfybl_cs = ""; // 自费用比例参数
	string zfyzb_cs = ""; // 自费药品比例参数

	double grzf = 0.0; // 个人自付
	double yjj = 0.0; // 预交金
	double tczfhj = 0.0; // 统筹支付合计
	double ybzfyjz = 0.0; // 医保支付预警值
	double sbpf = 0.0; // 商保赔付
	double ndylfy = 0.0; // 年度累计医保费用
	double zffybl = 0.0; // 自付费用比例

	// 颜色映射
	string whiteStr = "FFFFFF";
	string greenStr = "008000";

	// 读取参数
	RESULT_MAP::iterator iter = dbParameter.find("parameter");
	if(iter != dbParameter.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// 获取对应的参数值
				// 个人自付/预交金比例参数
				if(vecIter->first == "NAME" && vecIter->second == "GRZF_YJJ_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							grzf_yjj_cs = vecIter->second;
					}
				}
				// 统筹支付合计/医保支付预警值比例参数
				else if(vecIter->first == "NAME" && vecIter->second == "TCZFHJ_YBZFYJZ_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							tczfhj_ybzfyjz_cs = vecIter->second;
					}
				}
				// 商保赔付
				else if(vecIter->first == "NAME" && vecIter->second == "SBPF_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							sbpf_cs = vecIter->second;
					}
				}
				// 年度累计医保费用参数
				else if(vecIter->first == "NAME" && vecIter->second == "NDYLFY_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							ndylfy_cs = vecIter->second;
					}
				}
				// 自付费用比例
				else if(vecIter->first == "NAME" && vecIter->second == "ZFYBL_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							zfybl_cs = vecIter->second;
					}
				}
				// 自付药占比例
				else if(vecIter->first == "NAME" && vecIter->second == "ZFYZB_CS")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							zfyzb_cs = vecIter->second;
					}
				}
				// 获取显示标题
				else if(vecIter->first == "NAME" && vecIter->second == "TITLE")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							title = vecIter->second;
					}
				}
				// 获取基本颜色1
				else if(vecIter->first == "NAME" && vecIter->second == "BASIC_COLOR1")
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					for(;vecIter!=vecVecIter->end(); ++vecIter)
					{
						if(vecIter->first == "VALUE")
							whiteStr = vecIter->second;
					}
				}
				// 获取基本颜色2
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
	
	// 设置title
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
	// --------------------------------根据是否有对对应参数进行阈值审查---------------------------------------
	// 个人自付/预交金比例审查
	if(!grzf_yjj_cs.empty())
	{
		if(yjj != 0)
			ReplaceColorStr(dataResult, "PREPAYMENTS", grzf_yjj_cs, grzf/yjj);
	}

	// 统筹支付合计/医保支付预警值比例审查
	if(!tczfhj_ybzfyjz_cs.empty())
	{
		if(ybzfyjz != 0)
			ReplaceColorStr(dataResult, "TCZFHJ", tczfhj_ybzfyjz_cs, tczfhj/ybzfyjz);
	}

	// 商保赔付审查
	if(!sbpf_cs.empty())
	{
		ReplaceColorStr(dataResult, "SBPFHJ", sbpf_cs, sbpf);
	}

	// 年度累计医保费用审查
	if(!ndylfy_cs.empty())
	{
		ReplaceColorStr(dataResult, "NDYLFY", ndylfy_cs, ndylfy);
	}

	// 自费用比例审查
	if(!zfybl_cs.empty())
	{
		ReplaceColorStr(dataResult, "GRFDL", zfybl_cs, zffybl);
	}

	// --------------------------------若自费药占比项目占比参数不存在则不计算以下审查(只对新农合)---------------------------------------
	if(zfyzb_cs.empty())
	{
		dataResult = "{state:1}{message:操作成功}"+dataResult;
		strcpy(result, dataResult.c_str());
		return;
	}
	// ===================自费药占比审查===================
	string publicDrugChargeStr = "", allDrugChargeStr = "";
	// 获取公费药品费用
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
	// 获取所有药品费用
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
	// 计算自费药品费用比例
	if(!allDrugChargeStr.empty() || !publicDrugChargeStr.empty())
	{
		double allDrugCharge = atof(allDrugChargeStr.c_str());
		if(allDrugCharge != 0)
		{
			double selfDrugChargeRate = (atof(allDrugChargeStr.c_str())-atof(publicDrugChargeStr.c_str())) / allDrugCharge;
			char* selfDrugChargeRateStr = new char[16];
			//char* self_drug_charge_rate_str = new char[16];
			sprintf(selfDrugChargeRateStr, "%.2f%", selfDrugChargeRate*100);
			dataResult += "{自费药占比:"+string(selfDrugChargeRateStr)+"%";
			// 获取限额
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
					dataResult.append("【限额("+string(limitValueCStr)+"),超限额比例为("+string(offsetStr)+")】");
				}
			}
			// 获取颜色
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
			dataResult += "}";// (报销药品费用"+publicDrugChargeStr+"元，总药品费用"+allDrugChargeStr+"元)
		}
		else
		{
			dataResult += "{自费药占比:查询出该患者本次所有药品费用总额为0元，请核实是否有误！}";
		}
	}

	// ===================自费项目占比审查===================
	string publicItemChargeStr = "", allItemChargeStr = "";
	// 获取公费项目费用
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
	// 获取所有项目费用
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
	// 计算自费项目费用比例
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
			dataResult += "{自费项目占比:"+string(selfItemChargeRateStr)+"%";
			/*if(self_item_charge_rate!=-1 && selfItemChargeRate>self_item_charge_rate)
				dataResult += "【已超限("+string(self_item_charge_rate_str)+"%)】";*/

			// 获取颜色
			if(isWhite)
				dataResult.append("<<"+whiteStr+">>");
			else
				dataResult.append("<<"+greenStr+">>");
			isWhite = !isWhite;

			dataResult += "}";//(报销项目费用"+publicItemChargeStr+"元，总项目费用"+allItemChargeStr+"元)
		}
		else
		{
			dataResult += "{自费项目占比:查询出该患者本次所有项目费用总额为0元，请核实是否有误！}";
		}
	}
	// 返回结果数据和显示数据
	dataResult = "{state:1}{message:操作成功}"+dataResult;
	strcpy(result, dataResult.c_str());
}
 