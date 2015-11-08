/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Helper.cpp
//功  能：		实现辅助函数
//开发者：		高志强
//日  期：		2014年04月12日
//长  度：		100行
/////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"				//MFC头文件
#include "Helper.h"				//辅助函数头文件


//名  称：		MapDoctorPosIDtoPosName()
//功  能：		映射医生职称ID到名称
//参  数：		无
//返回值：		无
string  MapDoctorPosIDtoPosName(int nDoctorPosID)
{
	//定义返回值
	string sDoctorPosName="";

	//获取返回值
	switch(nDoctorPosID)
	{
		case 0:
			sDoctorPosName="教授";
			break;
		case 1:
			sDoctorPosName="副教授";
			break;
		case 2:
			sDoctorPosName="主任医师";
			break;
		case 3:
			sDoctorPosName="副主任医师";
			break;
		case 4:
			sDoctorPosName="专家";
			break;
		case 5:
			sDoctorPosName="主治医师";
			break;
		case 6:
			sDoctorPosName="讲师";
			break;
		default:
			sDoctorPosName="其他";
			break;

	}

	//返回结果
	return sDoctorPosName;
}

//名  称：		MapDoctorPosNametoPosID()
//功  能：		映射医生职称名称到职称ID
//参  数：		无
//返回值：		无
int MapDoctorPosNametoPosID(string sDoctorPosName)
{
	//获取返回值
	if(sDoctorPosName=="教授")
	{
		return 0;
	}

	if(sDoctorPosName=="副教授")
	{
		return 1;
	}
			
	if(sDoctorPosName=="主任医师")
	{
		return 2;
	}
	
	if(sDoctorPosName=="副主任医师")
	{
		return 3;
	}
	
	if(sDoctorPosName=="专家")
	{
		return 4;
	}

	if(sDoctorPosName=="主治医师")
	{
		return 5;
	}

	if(sDoctorPosName=="讲师")
	{
		return 6;
	}

	//返回结果
	return 7;
}


//名  称：		MapCStringToString()
//功  能：		将CString映射到string
//参  数：		CString&
//返回值：		string
string  MapCStringToString(CString& sGivenCString)
{
    CStringA sTemp(sGivenCString.GetBuffer(0));
    sGivenCString.ReleaseBuffer();
 
    string sReturnString=sTemp.GetBuffer(0);
    sTemp.ReleaseBuffer();

	//返回结果
	return sReturnString;
}

//名  称：		StoWs()
//功  能：		将string映射到wstring
//参  数：		string&
//返回值：		wstring
std::wstring StoWs(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//名  称：		StoWs()
//功  能：		将string映射到wstring
//参  数：		string&
//返回值：		wstring
int GetPositionIDFromName(string& sPositionName)
{
	//根据医生职称返回结果
	if(sPositionName=="教授")		return 0;
	if(sPositionName=="副教授")		return 1;
	if(sPositionName=="主任医师")	return 2;
	if(sPositionName=="副主任医师")	return 3;
	if(sPositionName=="专家")		return 4;
	if(sPositionName=="主治医师")	return 5;
	if(sPositionName=="讲师")		
	{
		return 6;
	}
	else	//其他
	{
		return 7;
	}
}

//名  称：		GetMainFileName()
//功  能：		根据文件名称获取主文件名
//参  数：		string&
//返回值：		string
string GetMainFileName(string& sFileName)
{
	//定义主文件名
	string sMainFileName=sFileName.substr(0,sFileName.size()-4);

	//返回主文件名
	return sMainFileName;
}