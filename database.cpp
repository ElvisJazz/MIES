/////////////////////////////////////////////////////////////////////////////////////////////////
// 名称：database.cpp
// 功能：实现CDB类
// 开发者：归耀城
// 时间：20140410
// 修改：20140502
// 修改：20140525
// 修改：20140531，GetConnectionParams()
// 长度：239行
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"							// MFC标准头文件
#include "database.h"						// 访问数据库头文件
//#include "Helper.h"						// 字符串帮助函数


// 函数名：InitLog()
// 功能：初始化日志文件
// 参数：无
// 返回值：无
void CDB::InitLog()
{
	////////////////////////////
	// 第一步：创建数据库日志文件对象
	m_log = new ofstream();					// 数据库日志文件，当相对路径下log目录不存在时，不会创建日志
	m_log->open("log\\CDB.log", std::ofstream::out|std::ofstream::trunc);
	////////////////////////////
	// 第二步：验证文件创建是否成功
	if(!m_log->is_open())
		m_log=NULL;							// 当相对路径下log目录不存在时，不会创建日志
}

// 函数名：GetConnectionParams()
// 功能：获取数据库连接参数
// 参数：文件路径
// 返回值：无
// 异常：runtime_error
string CDB::GetConnectionParams(string strFilePath)
{	
	///////////////////////////
	// 第一步：打开数据库配置文件
//	wstring wsFilePath = StoWs(strFilePath);// string转换为wstring
	// 打开数据库配置文件
	ifstream infile;
	infile.open(strFilePath.c_str(), ifstream::in);
	if(!infile.is_open())
	{
		if (m_log!=NULL)
			*m_log<<"数据库配置文件打开失败.文件不存在或文件已损坏.\n"+strFilePath;
		
		throw runtime_error("数据库配置文件打开失败.文件不存在或文件已损坏.\n"+strFilePath);
	}
	
	///////////////////////////
	// 第二步：解析数据库配置文件
	// 读取文件
	string strLine;
	string strConnection;
	while(!infile.eof())
	{
		// 读取一行参数
		std::getline(infile,strLine);
		// 解析一行参数
		int nPosition = strLine.find('#');	// 以#号开始的参数忽略，这样判断要求#号不出现在正常的配置中
		// 判断是否满足要求
		if (nPosition==string::npos && !strLine.empty())
											// 取不包含#号时的非空的第一行为配置参数
		{
			strConnection = strLine;
			break;
		}

	}
	
	///////////////////////////
	// 第三步：返回数据库连接配置
	// 验证解析结果的正确性
	if (strConnection.empty()) throw runtime_error("数据库配置文件参数为空.");
	// 关闭文件
	infile.close();
	// 返回值
	return strConnection;
}

////////////////////////////////////////////////////////////////
// 函数名：Init()	
// 功能：初始化
// 参数：配置文件路径
// 返回值：无
// 异常：runtime_error
void CDB::Init(string strFilePath)
{
	////////////////////////////
	// 第一步：获取数据库连接配置
	// 获得连接字符串
	string sConnection = GetConnectionParams(strFilePath);
	// string转换为CString
	CString strConnection(sConnection.c_str());
	
	////////////////////////////
	// 第二步：打开数据库连接
	TRY
	{
		// 打开数据库
		m_database.Open(NULL, FALSE, FALSE, strConnection);
	}
	CATCH_ALL(e)
	{
		// 数据库初始化失败
		if(m_log!=NULL)	{
			*m_log<<"数据库连接初始化失败"<<endl;
			*m_log<<(((CDBException*)e)->m_strError+_T("\n"));
			*m_log<<strFilePath<<endl;
			m_log->flush();
		}
		throw runtime_error("数据库连接创建异常.网络已断开或数据库配置文件参数不正确.");
	}
	END_CATCH_ALL;

	////////////////////////////
	// 第三步：输出数据库连接创建结果
	if ( !m_database.IsOpen() )
		throw runtime_error("数据库连接打开失败.网络已断开或数据库配置文件参数不正确.");

	if(m_log!=NULL) {
		*m_log<<"数据库连接初始化成功.\n";
		m_log->flush();
	}

	#ifdef	 BATCH_INSERT
	// 开始一个批量
	m_database.BeginTrans();
	// 初始化
	m_nInsertCounter = 0;
	#endif
}


/////////////////////////////////////////////////////////////////
// 功能：关闭数据库
void CDB::Close()
{
	#ifdef BATCH_INSERT
	// 提交最后一批数据
	m_database.CommitTrans();
	#endif

	// 关闭数据库
	m_database.Close();
	// 关闭日志文件
	if(m_log!=NULL)
		m_log->close();
}

////////////////////////////////////////////////////////////////
// 名称：Submit()
// 功能：提交一条插入语句或更新语句。在定义BATCH_INSERT的情况下为批量提交
// 参数1：查询语句
// 异常：runtime_error
void CDB::Submit(string ssql)
{
	// 将string字符串转换为CString
	CString strSql(ssql.c_str());

	TRY
	{
		#ifdef BATCH_INSERT
		// 计数
		m_nInsertCounter ++ ;
		#endif

		// 执行查询
		m_database.ExecuteSQL(strSql);

		#ifdef BATCH_INSERT
		if(m_nInsertCounter >= BATCH_SIZE)
		{
			// 提交一批
			m_database.CommitTrans();
			// 开启新的批
			m_database.BeginTrans();
			m_nInsertCounter = 0;
		}
		#endif
	}
	// 处理异常
	CATCH_ALL(e)
	{

		// 日志信息方式选择				
		if(m_log!=NULL) {
			*m_log<<"数据库语句执行失败"<<endl;
			*m_log<<ssql<<std::endl;
			m_log->flush();
		}
		throw runtime_error("数据库插入或更新语句执行失败.详细信息请查看数据库日志文件log\\CDB.log.\n"+ssql);
	}
	END_CATCH_ALL;
}


// 名称：Execute()
// 功能：单独执行一条语句，可以是任何数据库语句
// 输入：语句字符串
// 输出：无
// 异常：runtime_error
void CDB::Execute(string ssql)
{
	/*
		1、执行查询语句，需要立刻得到结果；
		2、如果开启了批量，需要立刻提交批量。
	*/
	CString strSql(ssql.c_str());

	TRY
	{
		// 执行查询
		m_database.ExecuteSQL(strSql);

		// 如果打开了批量，立刻提交批量
		#ifdef BATCH_INSERT
		// 提交一批
		m_database.CommitTrans();
		// 开启新的批
		m_database.BeginTrans();
		#endif
	}
	CATCH_ALL(e)
	{
		// 日志信息方式选择			
		if(m_log!=NULL)	{
			*m_log<<"数据库语句执行失败"<<endl;
			*m_log<<ssql<<std::endl;
			m_log->flush();
		}
		throw runtime_error("数据库语句执行失败.详细信息请查看数据库日志文件log\\CDB.log.\n"+ssql);
	}
	END_CATCH_ALL;
}