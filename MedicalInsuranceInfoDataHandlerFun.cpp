///////////////////////////////////////////////////////////////////////
//文件名： LogicHandler.cpp
//功  能： 医保审查系统数据库获取数据逻辑处理类用药提示函数实现文件
//开发者： 贾治中
//创  建：2014/7/8
//修  改：2014/7/8
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"


//功  能：处理用药提示（医保目录）对应的数据库数据
//参  数：处理后的结果字符串指针，数据库查询数据
//返回值：窗口显示信息
string LogicHandler::HandleMedicalInsuranceInfoData(char* result, string patientId, string visitId, string item_code, RESULT_MAP dbParameter, RESULT_MAP dbResult, string chargeType, string classCode)
{
	string displayInfo = "";
	string item_name = ""; // item_code对应名称
	
	// ===================获取并设置数据库阈值配置参数===================
	// 解析参数结果，获得对应基准库的配置参数值
	string ybjzk = ""; // 医保基准库
	string jzkfycs = ""; // 基准库费用参数
	string fylbz = ""; // 项目费用类别值（甲类、乙类、自费等对应数值）
	string limitInfo = ""; // 限制性条件
	string tipFlag = ""; // 医保基准库风险提示
	string jxsgfy_cs = ""; // 军休市公费用参数
	string jctcts_cs = ""; // 检查套餐提示参数

	// 获取参数
	RESULT_MAP::iterator iter = dbParameter.find("parameter");
	while(iter != dbParameter.end())
	{
		RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
		for(;vecVecIter != iter->second.end(); ++vecVecIter)
		{
			vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
			for(;vecIter!=vecVecIter->end(); ++vecIter)
			{
				// 获取对应的参数值
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
				else if(vecIter->first == "NAME" && vecIter->second == "JCTCTS") // 检查套餐提示
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
	
	// ===================检查项目查询===================
	//AfxMessageBox(_T("参数模板")+CString(jctcts_cs.c_str()));
	if(classCode=="C" && jctcts_cs!="")
	{
		string dateTime = "";
		// 设置参数
		map<string,string> tcParam;
		tcParam.insert(pair<string, string>("PATIENT_ID", patientId)); 
		tcParam.insert(pair<string, string>("VISIT_ID", visitId)); 
		tcParam.insert(pair<string, string>("ITEM_CODE", item_code));
		// 创建查询对象
		CSqlDao tcDao;
		RESULT_MAP tcDBResult;
		try
		{
			// 初始化查询对象
			tcDao.Init("MIES_db_config.conf","MIES_sql_config.xml");
			// 执行查询
			tcDBResult = tcDao.Query("JCTC_YYTS", tcParam);
		
			// 获取开具套餐的日期
			RESULT_MAP::iterator iter = tcDBResult.find("YYTS");
			if(iter != tcDBResult.end())
			{
				RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
				if(vecVecIter != iter->second.end())
				{
					vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
					if(vecIter!=vecVecIter->end())
					{
						// 获取日期
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
			// 异常处理代码
			AfxMessageBox(CString(err.what()));
			sprintf(result, "{state:0}{message:数据库操作出错！}");
			// 关闭对象
			tcDao.Close();
			return displayInfo;
		}
		// 关闭对象
		tcDao.Close();
		// 查询到之前看过类似检查项目
		//AfxMessageBox(_T("日期")+CString(dateTime.c_str()));
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

	// ===================根据不同的基准库进行相应的操作==================
	iter = dbResult.find("YYTS");
	int itemState = 1; // 审查项目状态标志位：1无后续行为；2不在基准库，不许开；3在武汉医保基准库，可能需要替换
	
	// ===================武汉医保基准库===================
	if(ybjzk == "YB_JZK_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "ZFBL") // 自费比例
						fylbz = vecIter->second;
					else if(vecIter->first == "SM" ) // 限制性条件
					{
						if(vecIter->second.length()!=0 && vecIter->second!="△")
						{
							limitInfo = vecIter->second;
							displayInfo = "提示信息：" + limitInfo + "\n";
						}
					}
					else if(vecIter->first == "TIP_FLAG") // 医保基准库风险提示
					{
						tipFlag = vecIter->second;
						if(classCode=="C" || classCode=="D")
						{
							if(tipFlag=="1")
								displayInfo += "规避扣费风险，请针对病情慎重使用！";
							sprintf(result, "{state:1}{message:操作成功}");
							return displayInfo;
						}
					}
					else if(vecIter->first == "ITEM_NAME") // 项目名称
					{
						item_name = vecIter->second;
					}
				}
			}
			else // 不在基准库,不允许开
			{
				if(classCode=="C" || classCode=="D")
				{
					//displayInfo = "";
					sprintf(result, "{state:1}{message:操作成功}");
					return displayInfo;
				}
				else
					itemState = 2;
			}
		}
		else // 不在基准库,不允许开
		{
			if(classCode=="C" || classCode=="D")
			{
				//displayInfo = "";
				sprintf(result, "{state:1}{message:操作成功}");
				return displayInfo;
			}
			else
				itemState = 2;
		}
	}
	else if(classCode != "A" && classCode != "I")  // 不是医保基准库且classCode不为A
	{
		sprintf(result, "{state:1}{message:操作成功}");
		return "";
	}
	// ===================铁路医保基准库===================
	else if(ybjzk == "TB_JZK_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BXBZ") // 自费比例
						fylbz = vecIter->second;
					else if(vecIter->first == "SM" ) // 限制性条件
					{
						if(vecIter->second.length()!=0 && vecIter->second!="△")
						{
							limitInfo = vecIter->second;
							displayInfo = "提示信息：" + limitInfo + "\n";
						}
					}
					else if(vecIter->first == "ITEM_NAME") // 项目名称
					{
						item_name = vecIter->second;
					}
				}
			}
			else // 不在基准库,不允许开
			{
				itemState = 2;
			}
		}
		else // 不在基准库,不允许开
		{
			itemState = 2;
		}

	}
	// ===================省医保基准库===================
	else if(ybjzk == "SYB_JZK_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BX") // 自费比例
						fylbz = vecIter->second;
					else if(vecIter->first == "SM" ) // 限制性条件
					{
						if(vecIter->second.length()!=0 && vecIter->second!="△")
						{
							limitInfo = vecIter->second;
							displayInfo = "提示信息：" + limitInfo + "\n";
						}
					}
					else if(vecIter->first == "ITEM_NAME") // 项目名称
					{
						item_name = vecIter->second;
					}
				}
			}
			else // 不在基准库,不允许开
			{
				itemState = 2;
			}			
		}
		else // 不在基准库,不允许开
		{
			itemState = 2;
		}

	}
	// ===================新农合医保基准库===================
	else if(ybjzk == "XNH_JZK_VS_PRICE_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "WIPEOUT") // 自费比例
						fylbz = vecIter->second;
				}
			}
			else // 不在基准库,不允许开
			{
				itemState = 2;
			}
		}
		else // 不在基准库,不允许开
		{
			itemState = 2;
		}

	}
	//===================省市公费、军休基准库===================
	else if(ybjzk == "CHARGE_SPECIAL_ITEM_DICT")
	{
		// 如果在基准库
		string fenzi = "", fenmu = ""; // 比例分子、分母
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "PROPORTION_NUMERATOR") // 自付比例分子
						fenzi = vecIter->second;
					else if(vecIter->first == "PROPORTION_DENOMINATOR") // 自付比例分母
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
						sprintf(infoStr, ("该项目类别为："+chargeType+"，自费。").c_str());
					else if(blz > atof(jxsgfy_cs.c_str()))
						sprintf(infoStr, ("该项目类别为："+chargeType+"，自付比例为：%.2f%。").c_str(), blz*100);
				}
				else
					sprintf(infoStr, ("该项目类别为："+chargeType+"，其自付比例分母为0，请核实是否有误！").c_str());
				displayInfo = string(infoStr);
			}
			
		}
	}

	// ===================检查是否是自费药品，针对在基准库且非省市公费、军休基准库的情况===================
	int currentType = 0; // 当前项目类型，默认可报销 
	string tmpDisplayInfo;
	if(itemState!=2 && !ybjzk.empty() && ybjzk!="CHARGE_SPECIAL_ITEM_DICT")
	{
		jzkfycs.append(";");
		string::size_type index = jzkfycs.find(fylbz+":");
		if(fylbz!="" && index!=string::npos)
		{
			string::size_type index2 = jzkfycs.find(";", index);
			// 获取用于判断的参数段,示例：从 .10:甲类,0;0.00:乙类,0;-1:自费,1; 中截取 .10:甲类,0
			string checkStr = jzkfycs.substr(index, index2-index);
			// 获取对应的类别信息
			index = checkStr.find(":", 0);
			index2 = checkStr.find(",", index);
			if(index!=string::npos && index2!=string::npos)
			{
				if((checkStr.substr(index+1, index2-index-1)=="自费" && classCode=="I") || classCode != "I")
					tmpDisplayInfo = "该项目类别为：" + checkStr.substr(index+1, index2-index-1);
				// 判断是否是自费项目且为武汉\铁保\省医保基准库项目
				if(ybjzk == "YB_JZK_DICT" || ybjzk == "TB_JZK_DICT" || ybjzk == "SYB_JZK_DICT") //atoi(checkStr.substr(index2+1).c_str())==1 && 
					itemState = 3;
			}
			else
			{
				displayInfo = "用药提示配置参数出错！";
			}
		}
		else // 搜索项目费用类别其他的情况对应参数
		{
			index = jzkfycs.find("-1");
			string::size_type index2 = jzkfycs.find(";", index);
			// 获取用于判断的参数段,示例：从 .10:甲类,0;0.00:乙类,0;-1:自费,1; 中截取 .10:甲类,0			
			if(index!=string::npos && index2!=string::npos)
			{
				string checkStr = jzkfycs.substr(index, index2-index);
				// 获取对应的类别信息
				index = checkStr.find(":", 0);
				index2 = checkStr.find(",", index);
				if(index!=string::npos && index2!=string::npos)
				{
					if((checkStr.substr(index+1, index2-index-1)=="自费" && classCode=="I") || classCode != "I")
						tmpDisplayInfo = "该项目类别为：" + checkStr.substr(index+1, index2-index-1);
					//displayInfo = "该项目类别为：" + checkStr.substr(index+1, index2-index-1) + "\n" + displayInfo;
					// 判断是否是自费项目且为武汉\铁保\省医保基准库项目
					if(ybjzk == "YB_JZK_DICT" || ybjzk == "TB_JZK_DICT" || ybjzk == "SYB_JZK_DICT") //atoi(checkStr.substr(index2+1).c_str())==1 && 
						itemState = 3;
					if(atoi(checkStr.substr(index2+1).c_str()) == 1 )
						currentType = 1; // 自费
				}
				else
				{
					displayInfo = "用药提示配置参数出错！";
				}
			}
			else
			{
				displayInfo = "用药提示配置参数出错！";
			}
		}
	}

	// 如果是材料,材料不做限制
	if(classCode == "I")
		itemState = 1;
	//如果是自费药品，去yb_special_control_dict表中寻找公费药品,只有这种情况有需要返回的待替换的项目内容	
	string resultStr = "";
	switch(itemState)
	{
	case 1: 
			resultStr = "{state:1}{message:操作成功}";
			if(tmpDisplayInfo.length() > 0)
				displayInfo = tmpDisplayInfo + "\n" + displayInfo;
			sprintf(result, resultStr.c_str());
			break;
	case 2: resultStr = "{state:2}{message:操作成功}";
			sprintf(result, resultStr.c_str());
			displayInfo = "该项目不在基准库中，请联系相关科室！";
			break;
	case 3: // 寻找可替换项目
		if(limitInfo != "")
		{
			resultStr = "{state:3}{message:操作成功}";
			displayInfo += "操作消息：该项目是否符合限制性条件？";

			// 创建查询对象
			if(currentType == 1)
			{
				RESULT_MAP itemDbResult;
				string itemCode = "", itemName = "", type="";
				CSqlDao dao;
				try
				{
					// 初始化查询对象
					dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
					map<string,string> param;
					param.insert(pair<string, string>("ITEM_CODE", item_code));
					// 执行查询
					itemDbResult = dao.Query("REPLACE_ITEM", param);
				}
				catch( runtime_error err )
				{
					// 异常处理代码
					AfxMessageBox(CString(err.what()));
					sprintf(result, "{state:0}{message:数据库操作出错！}");
					// 关闭对象
					dao.Close();
					return "";
				}
				// 关闭对象
				dao.Close();

				// 获取查询到的item_name和item_code
				iter = itemDbResult.find("replace_item");
				if(iter != itemDbResult.end())
				{
					for(;iter!=itemDbResult.end(); ++iter)
					{
						// 找到项目，获取对应变量
						RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
						if(iter->second.size() ==2 && ybjzk == "YB_JZK_DICT")
						{
							//resultStr = "{state:3}{message:操作成功}";
							//displayInfo += "操作消息：该项目是否符合限制性条件？";

							for(;vecVecIter != iter->second.end(); ++vecVecIter)
							{
								vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
								for(;vecIter!=vecVecIter->end(); ++vecIter)
								{
									if(vecIter->first == "ITEM_CODE") // 项目编码
									{
										itemCode = vecIter->second;
										if(item_code != itemCode)
										{
											type = "0";
											displayInfo = "该项目类别为：" + GetFeeType(ybjzk, jzkfycs, classCode, itemCode) + "\n" + displayInfo;
										}
										else
											type = "1";
									}
									else if(vecIter->first == "ITEM_NAME") // 项目名称
										itemName = vecIter->second;
									//else if(vecIter->first == "TYPE") // 项目名称
									//	type = vecIter->second;
								}
								if(type == "0")
									resultStr += ("{item[code:"+itemCode+"##name:"+itemName+"]}");
							}
						}
						else // 未找到两条项目记录,但显示询问信息
						{
							//resultStr += ("{itemCode:"+item_code+"}");
							displayInfo = tmpDisplayInfo+ "\n" + displayInfo;
							resultStr = "{state:3}{message:操作成功}"+("{item[code:"+item_code+"##name:"+item_name+"]}");
						}
					}
				}
				// 未找到项目记录,但显示询问信息
				else
				{
					displayInfo = tmpDisplayInfo + "\n" + displayInfo;
					resultStr = "{state:3}{message:操作成功}"+("{item[code:"+item_code+"##name:"+item_name+"]}");
				}
			}
			else
			{
				displayInfo = tmpDisplayInfo + "\n" + displayInfo;
				resultStr = "{state:3}{message:操作成功}"+("{item[code:"+item_code+"##name:"+item_name+"]}");
			}
		}
		else
		{
			displayInfo = tmpDisplayInfo+ + "\n" + displayInfo;
			resultStr = "{state:1}{message:操作成功}";
		}

		strcpy(result, resultStr.c_str());
		break;

	default: break;
	}
	
	return displayInfo;

}
	
//功  能：针对武汉医保、省医保、铁保获取项目费用类型
//参  数：基准库，基准库费用参数，项目类别编码，项目编码
//返回值：返回项目费用类型
string LogicHandler::GetFeeType(string ybjzk, string jzkfycs, string itemClass, string itemCode)
{
	if(ybjzk=="XNH_JZK_VS_PRICE_DICT" && ybjzk=="CHARGE_SPECIAL_ITEM_DICT")
		return "";

	map<string,string> param; 
	param.insert(pair<string, string>("ITEM_CLASS", itemClass)); 
	param.insert(pair<string, string>("ITEM_CODE", itemCode)); 
	// 创建查询对象
	CSqlDao dao;
	RESULT_MAP dbParameter, dbResult;
	try
	{
		// 初始化查询对象
		dao.Init("MIES_db_config.conf","MIES_sql_config.xml");
		// 执行查询
		
		
		// 找到对应的医保基准库参数值，在sql_config.xml中存在对应的ybjzk值得group查询
		dbResult = dao.Query(ybjzk+"_YYTS", param);
	}
	catch( runtime_error err )
	{
		// 异常处理代码
		AfxMessageBox(CString(err.what()));
		// 关闭对象
		dao.Close();
		return "";
	}
	dao.Close();

	string fylbz("");
	RESULT_MAP::iterator iter = dbResult.find("YYTS");
	// ===================武汉医保基准库===================
	if(ybjzk == "YB_JZK_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "ZFBL") // 自费比例
						fylbz = vecIter->second;
				}
			}
			else // 不在基准库,不允许开
			{
				AfxMessageBox(CString((itemCode+"未在武汉医保基准库中找到替换后的对应项目！").c_str()));
				return "";
			}
		}
		else // 不在基准库,不允许开
		{
			AfxMessageBox(CString((itemCode+"未在武汉医保基准库中找到替换后的对应项目！").c_str()));
			return "";
		}
	}
	// ===================铁路医保基准库===================
	else if(ybjzk == "TB_JZK_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BXBZ") // 自费比例
						fylbz = vecIter->second;
				}
			}
			else // 不在基准库,不允许开
			{
				AfxMessageBox(CString((itemCode+"未在铁保基准库中找到替换后的对应项目！").c_str()));
				return "";
			}
		}
		else // 不在基准库,不允许开
		{
			AfxMessageBox(CString((itemCode+"未在铁保基准库中找到替换后的对应项目！").c_str()));
			return "";
		}

	}
	// ===================省医保基准库===================
	else if(ybjzk == "SYB_JZK_DICT")
	{
		// 如果在基准库
		if(iter != dbResult.end())
		{
			// 获取对应变量
			RECORD_VECTOR::iterator vecVecIter = iter->second.begin();
			if(vecVecIter != iter->second.end())
			{
				vector<pair<string, string> >::iterator vecIter = vecVecIter->begin();
				for(;vecIter!=vecVecIter->end(); ++vecIter)
				{
					if(vecIter->first == "BX") // 自费比例
						fylbz = vecIter->second;
				}
			}
			else // 不在基准库,不允许开
			{
				AfxMessageBox(CString((itemCode+"未在省医保基准库中找到替换后的对应项目！").c_str()));
				return "";
			}			
		}
		else // 不在基准库,不允许开
		{
			AfxMessageBox(CString((itemCode+"未在省医保基准库中找到替换后的对应项目！").c_str()));
			return "";
		}

	}
	
	// 获取项目费用类别
	jzkfycs.append(";");
	string::size_type index = jzkfycs.find(fylbz+":");
	if(fylbz!="" && index!=string::npos)
	{
		string::size_type index2 = jzkfycs.find(";", index);
		// 获取用于判断的参数段,示例：从 .10:甲类,0;0.00:乙类,0;-1:自费,1; 中截取 .10:甲类,0
		string checkStr = jzkfycs.substr(index, index2-index);
		// 获取对应的类别信息
		index = checkStr.find(":", 0);
		index2 = checkStr.find(",", index);
		if(index!=string::npos && index2!=string::npos)
		{
			return checkStr.substr(index+1, index2-index-1);
		}
		else
		{
			AfxMessageBox(_T("用药提示配置参数出错！"));
			return "";
		}
	}
	else // 搜索项目费用类别其他的情况对应参数
	{
		index = jzkfycs.find("-1");
		string::size_type index2 = jzkfycs.find(";", index);
		// 获取用于判断的参数段,示例：从 .10:甲类,0;0.00:乙类,0;-1:自费,1; 中截取 .10:甲类,0			
		if(index!=string::npos && index2!=string::npos)
		{
			string checkStr = jzkfycs.substr(index, index2-index);
			// 获取对应的类别信息
			index = checkStr.find(":", 0);
			index2 = checkStr.find(",", index);
			if(index!=string::npos && index2!=string::npos)
			{
				return checkStr.substr(index+1, index2-index-1);
			}
			else
			{
				AfxMessageBox(_T("用药提示配置参数出错！"));
				return "";
			}
		}
		else
		{
			AfxMessageBox(_T("用药提示配置参数出错！"));
			return "";
		}
	}
	
}
