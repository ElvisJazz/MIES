/////////////////////////////////////////////////////////////////////
// 文件名：sql_processor.h
// 功能：1）解析查询语句，2）执行查询
// 开发者：归耀城
// 时间：2014年4月13日
// 修改：2014年7月9日
// 长度：53行
// 使用说明：
// 1）ParseQuery()表示解析过程，只需执行一遍
// 2）Query()表示查询过程，可重复调用
/////////////////////////////////////////////////////////////////////

#ifndef SQL_PARSER
#define SQL_PARSER

#include "sql_public.h"						// 数据库查询头文件

// 类声明
class CSqlProcessor
// 类定义
{
/////////////////////////////////
	// 私有成员变量
private:
	vector<string> m_vfieldNames;			// 字段名称数组
	vector<string> m_vparamNames;			// 参数名数组
	string m_ssql;							// SQL语句
	string m_smark;							// 特殊符号
	string m_sTypeName;						// 对象类型，用以区别当前查询对象

/////////////////////////////////
	// 公共函数
public:
	CSqlProcessor(){m_smark="?";};								// 构造函数
	void Close();												// 关闭对象
	~CSqlProcessor(){Close();};									// 析构函数
	string GetType(){return m_sTypeName;}						// 获得对象类型
	vector<string> GetFieldNames(){return m_vfieldNames;};		// 获得字段名数组
	void ParseQuery(XMLElement * pQuery);						// XML元素解析函数
	RECORD_VECTOR Query(map<string,string> & map, CDB * pDB);	// 数据库查询函数

/////////////////////////////////
	// 私有函数
private:
	void _ParseQuery(XMLElement * pQuery);										// 解析QUERY配置结点		
	void GetValues(XMLElement * pEle, vector<string> & values);					// 获得XML元素结点的内容
	void ParseSelects(XMLElement * pSelects, string sqlName);					// 解析SELECTS结点
	void ReplaceFirst(string & originStr, string & oldStr, string & newStr);	// 替换第一次出现的子串
	string ReBuildQuery(map<string,string> & map);								// 重构查询语句
	RECORD_VECTOR QueryDB(CDB * pDB, string sql);								// 查询数据库操作
};

#endif