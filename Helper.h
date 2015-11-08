/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Helper.h
//功  能：		定义辅助函数
//开发者：		高志强
//日  期：		2014年04月12日
//长  度：		30行
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>								//map头文件
#include <set>								//set头文件
#include <string>							//string头文件
#include <list>								//list头文件
#include <vector>							//vector头文件
#include <algorithm>						//algorithm类头文件
#include <numeric>							//numeric类头文件
#include <sstream>							//sstream类头文件
#include <fstream>							//fstream类头文件
#include <iostream>							//iostream类头文件
#include <time.h>							//时间函数类头文件
#include "math.h"							//数学函数头文件

//声明命名空间
using namespace std;						//使用标准命名空间

string MapDoctorPosIDtoPosName(int);		//映射医生职称ID到职称
int MapDoctorPosNametoPosID(string&);		//映射医生职称到职称ID
string MapCStringToString(CString&);		//将CString映射到string

std::wstring StoWs(const std::string& s);	//将string映射到wstring

int GetPositionIDFromName(string&);			//根据医生职称名称获取其ID

//转换文件名称
string GetMainFileName(string&);			//根据文件名称获取主文件名