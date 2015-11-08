/////////////////////////////////////////////////////////////////////
// 文件名：sql_public.h
// 功能：数据库查询公共的头文件，包括返回值结构体定义
// 开发者：归耀城
// 时间：2014年7月9日
// 长度：26行
/////////////////////////////////////////////////////////////////////

#ifndef SQL_PUBLIC
#define SQL_PUBLIC

#include "tinyxml2.h"		// tinyxml头文件
#include "database.h"		// 数据库访问对象头文件
#include <vector>			// STL vector头文件
#include <map>				// STL map头文件
#include <string>			// STL string头文件
#include "Helper.h"			// 高老师提供的辅助函数头文件

using namespace std;		// STL 命名空间
using namespace tinyxml2;	// TinyXML2 命名空间

// 类型声明
typedef vector<vector<pair<string,string>>> RECORD_VECTOR;
typedef std::map<string,RECORD_VECTOR> RESULT_MAP;

#endif