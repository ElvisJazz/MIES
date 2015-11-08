/////////////////////////////////////////////////////////////////////
// 文件名：sql_dao.h
// 功能：1）提供数据库和查询初始化函数；2）按分组调用查询的函数
// 开发者：归耀城
// 时间：2014年7月9日
// 长度：41行
// 使用说明：
// 1）Init()是初始化函数，创建查询对象时只需初始化一次，
//    参数是数据库连接配置文件和查询语句配置文件；
// 2）Query()是查询函数，参数是查询组的名称，以及查询使用的参数。
/////////////////////////////////////////////////////////////////////

#ifndef SQL_DAO
#define SQL_DAO

#include "sql_group.h"					// SQL查询语句组头文件

// 类定义
class CSqlDao
{
///////////////////////////////
	// 私有成员变量
private:
	map<string,CSqlGroup *> m_groups;	// 查询组集合
	CDB  m_db;							// 数据库对象

///////////////////////////////
	// 公共成员函数
public:
	void Init(string sDBConfigFile, string sSqlFile);					// 初始化对象
	void Close();														// 关闭对象
	~CSqlDao(){Close();};												// 析构函数
	RESULT_MAP Query(string sGroupName, map<string,string> & parammap);	// 执行特定的查询

///////////////////////////////
	// 私有成员函数
private:
	void ParseDoc(string sSqlFile);										// 解析查询配置文件
};

#endif