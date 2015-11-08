//////////////////////////////////////////////////////////////////////////////////////////////////
// 名称：database.h
// 功能：实现CDB类
// 开发者：归耀城
// 时间：20101018
// 长度：50行
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef	ACCESS_DATABASE
#define	ACCESS_DATABASE

// 头文件
#include <afxdb.h>									// MFC数据库头文件
#include <odbcinst.h>								// ODBC头文件
#include <fstream>									// fstream头文件
#include <string>									// string头文件

using namespace std;

/*
宏命令：
	通过批量宏开启和关闭批量提交功能；
	通过MY_DEBUG宏开启调试功能，调试过程不提交数据。
*/
//#define BATCH_SIZE		200000					// 20万
//#define BATCH_INSERT
//#define MY_DEBUG

// 类声明和定义
class CDB
{
public:
	// 成员函数
	CDB(){InitLog();};								// 构造函数
	~CDB(){Close();};								// 析构函数
	void InitLog();									// 初始化日志文件
	string GetConnectionParams(string);				// 获取连接参数
	void Init(string);								// 初始化函数
	void Close();									// 关闭数据库
	void Submit(string);							// 向数据库提交一条语句，但不一定立刻执行			// 注意：参数不可以改成引用
	void Execute(string);							// 单独执行一条语句							// 更新：2011-12-03
	
	// 成员变量
	CDatabase		m_database;						// 数据库对象
	unsigned int	m_nInsertCounter;				// 插入计数器
	ofstream	*	m_log;							// 错误日志
};

#endif
