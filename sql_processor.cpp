/////////////////////////////////////////////////////////////////////
// 文件名：sql_processor.cpp
// 功能：解析QUERY节点，执行查询
// 开发者：归耀城
// 时间：2014年4月13日
// 修改：2014年7月9日
// 长度：219行
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// MFC头文件
#include "sql_processor.h"		// Sql执行对象头文件

// 名称：ParseQuery()
// 功能：解析XML文档的SQL结点
// 参数：TiXmlElement *
// 返回：空
// 异常：runtime_error
void CSqlProcessor::ParseQuery(XMLElement * pQuery)
{
	// 判断输入合法性
	if (pQuery==NULL) return;
	// 清空数据结构
	m_vfieldNames.clear();
	m_vparamNames.clear();
	m_ssql.clear();

	// 获取名称
	const char * s = pQuery->Attribute("type");
	if ( s ) m_sTypeName = s;
	else throw runtime_error("HIS查询配置文件解析失败.QUERY结点type属性缺失."); // 抛出异常
	// 解析MAPPING结点
	_ParseQuery(pQuery);
}

// 名称：Query()
// 功能：查询数据库
// 参数：map<string,string>&, CDB*
// 返回：空
RECORD_VECTOR CSqlProcessor::Query(map<string,string> & parammap, CDB * pDB)
{
	// 参数合法性检查
	if ( pDB==NULL ) return RECORD_VECTOR();
	// 重构查询语句，填写参数
	string ssql = ReBuildQuery(parammap);
	// 执行查询
	return QueryDB(pDB,ssql);
}


// 名称：QueryDB()
// 功能：执行数据库查询
// 参数：CDB*, string
// 返回：空
RECORD_VECTOR CSqlProcessor::QueryDB(CDB *pDB, string ssql)
{
	// 创建返回结果
	RECORD_VECTOR records;
	// 字符串类型转换
	CString strSql(ssql.c_str());
	strSql.Trim();

	TRY
	{
		// 创建记录集
		CRecordset recordset(&(pDB->m_database));
		// 执行查询
		recordset.Open(CRecordset::forwardOnly, strSql);

		// 遍历记录集
		for(; !recordset.IsEOF(); recordset.MoveNext())
		{
			// 创建数组
			std::vector<pair<string,string>> record;
			// 获取信息
			// 根据SQL语句的字段数据获取数据
			unsigned int fieldSize = m_vfieldNames.size();
			for(unsigned int i=0; i<fieldSize; i++)
			{
				// 得到一个字段名
				CString strFieldName(m_vfieldNames[i].c_str()); // 字段名由string类型转换为CString类型
				CStringW var;
				// 访问记录集，得到字段值
				recordset.GetFieldValue(strFieldName, var);
				string sfieldValue = MapCStringToString(var);
				// 添加到字段值数组
				record.push_back(std::pair<string,string>(m_vfieldNames[i],sfieldValue));
			}
			// 添加到查询结果数组
			records.push_back(record);
		}
		// 关闭记录集
		recordset.Close();
	}
	CATCH_ALL(e)
	{
		// 数据库查询异常提示
		throw runtime_error("数据库查询异常.网络断开或HIS查询配置文件错误.字段名称与SQL语句不匹配.\n"+ssql);
	}
	END_CATCH_ALL;
	// 返回查询结果
	return records;
}

// 名称：ReBuildQuery()
// 功能：查询语句重构
// 参数：map<string,string>&
// 返回：string
string CSqlProcessor::ReBuildQuery(map<string, string> & map)
{
	// 复制sql语句
	string ssql = m_ssql;
	// 替换sql语句中所有的参数
	unsigned int paramSize = m_vparamNames.size();
	for (unsigned int i=0; i<paramSize; i++)
	{
		// 获取参数名称
		string & param = m_vparamNames[i];
		// 根据参数名称，在参数map中查询
		std::map<string, string>::iterator iter = map.find(param);
		// 如果存在参数的值，则替换SQL语句中的第一个标记（如？）
		if (iter!=map.end())
		{
			string value = iter->second;
			// 参数中含有可不替换单引号标志
			if(iter->second.find("####")!=string::npos) 
				ReplaceFirst(value, string("####"), string(""));
			else if(iter->second.find("'")!=string::npos) // 参数中包含单引号
				value = "'@@@@'";
			else
				value = "'"+value+"'";
			ReplaceFirst(ssql, m_smark, value);
		}
		// 如果不存在参数的值，则出现异常，使用特殊的值替换第一个标记
		else
		{
			// 使用特殊标记替换参数，确保查询语句正常，且查询结果为空
			string tag="'@@@@'";
			ReplaceFirst(ssql, m_smark, tag);
		}
	}
	// 取出SELECT前的空格或空行
	if (ssql.find_first_not_of(" \n\r\t")!=string::npos)
		ssql = ssql.substr(ssql.find_first_not_of(" \n\r\t"));
	// 返回结果
	//AfxMessageBox(CString(ssql.c_str()));
	return ssql;
}

// 名称：ReplaceFirst()
// 功能：替换字符串中第一个子串
// 参数：string&,string&,string&
// 返回：空
void CSqlProcessor::ReplaceFirst(string & originStr, string & oldStr, string & newStr)
{
	// 第一个子串的位置
	unsigned int nFirstIndex = originStr.find(oldStr);
	if (nFirstIndex!=string::npos)
		originStr.replace(nFirstIndex,oldStr.length(),newStr);
}


// 名称：_ParseQuery()
// 功能：抽取MAPPING元素的具体内容
// 参数：XmlElement*
// 返回：空
// 异常：runtime_error
void CSqlProcessor::_ParseQuery(XMLElement *pQuery)
{
	// 获取子结点，进入循环
	XMLElement * pEle = pQuery->FirstChildElement();
	for (; pEle!=NULL; pEle=pEle->NextSiblingElement())
	{
		// 获取元素名称
		string eleName;
		const char * s = pEle->Value();
		// 处理异常
		if ( s ) eleName = s;
		else throw runtime_error("HIS查询配置文件解析失败.QUERY结点的子结点定义出错.");

		// 根据元素名称抽取元素的内容
		if (eleName=="SELECTS")
			GetValues(pEle, m_vfieldNames);
		else if (eleName=="SQL")
		{
			const char * s = pEle->GetText();
			if ( s ) m_ssql = s;
			else throw runtime_error("HIS查询配置文件解析失败.SQL结点没有内容.");
		}
		else if (eleName=="PARAMETERS")
			GetValues(pEle, m_vparamNames);
	}
}

// 名称：GetValues()
// 功能：获取列表元素的内容
// 参数：XmlElement*,vector<string>&
// 返回：空
// 异常：runtime_error
void CSqlProcessor::GetValues(XMLElement * pEle, vector<string> & values)
{
	// 遍历所有子结点
	XMLElement * pNext = pEle->FirstChildElement();
	for (; pNext!=NULL; pNext=pNext->NextSiblingElement())
	{
		// 获取元素的内容
		const char * value = pNext->GetText();
		// 添加到数组
		if (value) values.push_back(value);
		else throw runtime_error("HIS配置文件解析失败.QUERY结点列表元素解析失败.");
	}
}

// 名称：Close()
// 功能：关闭对象
// 参数：无
// 返回：无
void CSqlProcessor::Close()
{
	// 清理内存
	m_vfieldNames.clear();
	m_vparamNames.clear();
	m_ssql.clear();
	m_smark.clear();
	m_sTypeName.clear();
}