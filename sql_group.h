/////////////////////////////////////////////////////////////////////
// 文件名：sql_group.h
// 功能：1）按分组解析查询语句；2）按分组执行查询语句
// 开发者：归耀城
// 时间：2014年7月8日
// 长度：34行
/////////////////////////////////////////////////////////////////////

#ifndef SQL_GROUP
#define SQL_GROUP

#include "sql_processor.h"	// SQL查询语句执行对象头文件

// 类声明
class CSqlGroup
// 类定义
{
/////////////////////////////////
	// 私有成员变量
private:
	vector<CSqlProcessor *>  m_processors;	// SQL语句对象数组
	string m_sTypeName;						// 查询组名称

/////////////////////////////////
	// 公共成员函数
public:
	void Close();							// 关闭对象
	~CSqlGroup(){Close();};					// 析构函数
	void ParseGroup(XMLElement * pGroup);	// 解析GROUP结点
	string GetType(){return m_sTypeName;};	// 获取查询组名称
	RESULT_MAP Query(map<string,string> & parammap, CDB * pDB);	// 执行查询
};

#endif