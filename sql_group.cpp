/////////////////////////////////////////////////////////////////////
// 文件名：sql_group.cpp
// 功能：1）按分组解析查询语句；2）按分组执行查询语句
// 开发者：归耀城
// 时间：2014年7月9日
// 长度：81行
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// MFC头文件
#include "sql_group.h"			// CSqlGroup头文件

// 名称：ParseGroup()
// 功能：解析GROUP结点
// 参数：XmlElement*
// 返回：空
void CSqlGroup::ParseGroup(XMLElement * pGroup)
{
	// 参数合法性检查
	if (pGroup==NULL) return;
	// 获取GROUP结点的类型名称
	const char * sTypeName = pGroup->Attribute("type");
	if ( sTypeName ) m_sTypeName = sTypeName;
	else throw runtime_error("HIS查询配置文件解析失败.GROUP结点的type属性缺失.");
	// 遍历GROUP结点所有的子结点
	XMLElement * pQuery = pGroup->FirstChildElement();
	for(;pQuery!=NULL;pQuery=pQuery->NextSiblingElement())
	{
		// 获取结点激活属性
		string sactivate;
		const char * s = pQuery->Attribute("activate");
		if ( s ) sactivate = s;
		else throw runtime_error("HIS查询配置文件解析失败.GROUP结点的activate属性缺失.");
		// 判断当前有效的配置项
		if (sactivate.find("yes")==string::npos) continue;

		// 创建SQL查询对象，解析QUERY结点
		CSqlProcessor * pSqlPro = new CSqlProcessor();
		pSqlPro->ParseQuery(pQuery);
		// 解析结果添加到SQL查询数组
		m_processors.push_back(pSqlPro);
	}
}

// 名称：Query()
// 功能：执行小组查询
// 参数：map<string,string>, CDB
// 返回：RESULT_MAP
RESULT_MAP CSqlGroup::Query(map<string,string> & parammap, CDB * pDB)
{
	// 创建返回结果
	RESULT_MAP resultMap;
	// 参数合法性检查
	if ( pDB==NULL ) return resultMap;
	// 执行查询
	for (unsigned int i=0; i<m_processors.size(); i++)
	{
		CSqlProcessor * pPro = m_processors[i];
		std::string sTypeName = pPro->GetType();
		RECORD_VECTOR records = pPro->Query(parammap,pDB);
		resultMap.insert(std::pair<string,RECORD_VECTOR>(sTypeName,records));
	}
	// 返回查询结果
	return resultMap;
}

// 名称：Close()
// 功能：关闭对象
// 参数：无
// 返回：无
void CSqlGroup::Close()
{
	// 清理内存
	m_sTypeName.clear();

	// 清理指针数组对象
	for (unsigned int i=0; i<m_processors.size(); i++)
	{
		m_processors[i]->Close();
		m_processors[i]=NULL;
	}
	m_processors.clear();
}