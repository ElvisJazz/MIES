/////////////////////////////////////////////////////////////////////
// 文件名：sql_dao.cpp
// 功能：1）提供数据库和查询初始化函数；2）按分组调用查询的函数
// 开发者：归耀城
// 时间：2014年7月9日
// 长度：96行
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// MFC头文件
#include "sql_dao.h"			// CSqlDao头文件

// 名称：Init()
// 功能：初始化数据库和查询配置
// 参数：string,string
// 返回值：无
// 异常：runtime_error
void CSqlDao::Init(string sDBConfigFile, string sSqlFile)
{
	// 初始化数据库
	m_db.Init(sDBConfigFile);
	// 解析配置文件
	ParseDoc(sSqlFile);
}

// 名称：Close()
// 功能：关闭数据库对象
// 参数：string,string
// 返回值：无
// 异常：runtime_error
void CSqlDao::Close()
{
	// 关闭数据库对象
	m_db.Close();
	// 清理容器
	map<string,CSqlGroup *>::iterator iter;
	for (iter=m_groups.begin();iter!=m_groups.end();iter++)
	{
		iter->second->Close();
		iter->second=NULL;
	}
	m_groups.clear();
}


// 名称：ParseDoc()
// 功能：解析配置文件
// 参数：string,配置文件完整路径
// 返回值：无
void CSqlDao::ParseDoc(string sSqlFile)
{
	// 创建文档对象
	tinyxml2::XMLDocument sqlDoc;
	// 加载文档
	sqlDoc.LoadFile(sSqlFile.c_str());
	
	// 解析文档
	XMLElement * pRoot = sqlDoc.RootElement();	// 获取根结点
	// 异常处理
	if ( !pRoot )
		throw runtime_error("HIS查询配置文件解析失败.配置文件不存在或XML文件缺少根结点.");
	// 遍历所有的GROUP
	XMLElement * pGroup = pRoot->FirstChildElement();  // 获取第一个GROUP
	for( ; pGroup != NULL; pGroup = pGroup->NextSiblingElement())
	{
		// 获取结点激活状态
		string sactivate;
		const char * s = pGroup->Attribute("activate");
		// 异常处理
		if ( s ) sactivate = s;
		else throw runtime_error("HIS查询配置文件解析失败.GROUP结点的activate属性缺失.");
		// 判断是否激活
		if ( sactivate.find("yes")==string::npos ) continue; // 结点没有激活，忽略，不做解析
		// 创建GROUP结点处理对象
		CSqlGroup * pSqlGroup = new CSqlGroup();
		// 解析查询组
		pSqlGroup->ParseGroup(pGroup);
		// 添加到查询组数组
		m_groups.insert(std::pair<string,CSqlGroup*>(pSqlGroup->GetType(), pSqlGroup));
	}
}

// 名称：Query()
// 功能：执行特定的查询组
// 参数：string,参数
// 返回值：RESULT_MAP
RESULT_MAP CSqlDao::Query(string sGroupName, map<string,string> & parammap)
{
	// 合法性检查
	if (sGroupName.empty()) return RESULT_MAP();
	// 通过名称获得查询组
	map<string,CSqlGroup *>::iterator iter;
	iter = m_groups.find(sGroupName);
	if (iter==m_groups.end()) return RESULT_MAP();
	CSqlGroup * pGroup = iter->second;
	// 执行查询，返回查询结果
	return pGroup->Query(parammap,&m_db);
}