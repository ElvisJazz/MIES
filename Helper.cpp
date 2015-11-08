/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		Helper.cpp
//��  �ܣ�		ʵ�ָ�������
//�����ߣ�		��־ǿ
//��  �ڣ�		2014��04��12��
//��  �ȣ�		100��
/////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"				//MFCͷ�ļ�
#include "Helper.h"				//��������ͷ�ļ�


//��  �ƣ�		MapDoctorPosIDtoPosName()
//��  �ܣ�		ӳ��ҽ��ְ��ID������
//��  ����		��
//����ֵ��		��
string  MapDoctorPosIDtoPosName(int nDoctorPosID)
{
	//���巵��ֵ
	string sDoctorPosName="";

	//��ȡ����ֵ
	switch(nDoctorPosID)
	{
		case 0:
			sDoctorPosName="����";
			break;
		case 1:
			sDoctorPosName="������";
			break;
		case 2:
			sDoctorPosName="����ҽʦ";
			break;
		case 3:
			sDoctorPosName="������ҽʦ";
			break;
		case 4:
			sDoctorPosName="ר��";
			break;
		case 5:
			sDoctorPosName="����ҽʦ";
			break;
		case 6:
			sDoctorPosName="��ʦ";
			break;
		default:
			sDoctorPosName="����";
			break;

	}

	//���ؽ��
	return sDoctorPosName;
}

//��  �ƣ�		MapDoctorPosNametoPosID()
//��  �ܣ�		ӳ��ҽ��ְ�����Ƶ�ְ��ID
//��  ����		��
//����ֵ��		��
int MapDoctorPosNametoPosID(string sDoctorPosName)
{
	//��ȡ����ֵ
	if(sDoctorPosName=="����")
	{
		return 0;
	}

	if(sDoctorPosName=="������")
	{
		return 1;
	}
			
	if(sDoctorPosName=="����ҽʦ")
	{
		return 2;
	}
	
	if(sDoctorPosName=="������ҽʦ")
	{
		return 3;
	}
	
	if(sDoctorPosName=="ר��")
	{
		return 4;
	}

	if(sDoctorPosName=="����ҽʦ")
	{
		return 5;
	}

	if(sDoctorPosName=="��ʦ")
	{
		return 6;
	}

	//���ؽ��
	return 7;
}


//��  �ƣ�		MapCStringToString()
//��  �ܣ�		��CStringӳ�䵽string
//��  ����		CString&
//����ֵ��		string
string  MapCStringToString(CString& sGivenCString)
{
    CStringA sTemp(sGivenCString.GetBuffer(0));
    sGivenCString.ReleaseBuffer();
 
    string sReturnString=sTemp.GetBuffer(0);
    sTemp.ReleaseBuffer();

	//���ؽ��
	return sReturnString;
}

//��  �ƣ�		StoWs()
//��  �ܣ�		��stringӳ�䵽wstring
//��  ����		string&
//����ֵ��		wstring
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

//��  �ƣ�		StoWs()
//��  �ܣ�		��stringӳ�䵽wstring
//��  ����		string&
//����ֵ��		wstring
int GetPositionIDFromName(string& sPositionName)
{
	//����ҽ��ְ�Ʒ��ؽ��
	if(sPositionName=="����")		return 0;
	if(sPositionName=="������")		return 1;
	if(sPositionName=="����ҽʦ")	return 2;
	if(sPositionName=="������ҽʦ")	return 3;
	if(sPositionName=="ר��")		return 4;
	if(sPositionName=="����ҽʦ")	return 5;
	if(sPositionName=="��ʦ")		
	{
		return 6;
	}
	else	//����
	{
		return 7;
	}
}

//��  �ƣ�		GetMainFileName()
//��  �ܣ�		�����ļ����ƻ�ȡ���ļ���
//��  ����		string&
//����ֵ��		string
string GetMainFileName(string& sFileName)
{
	//�������ļ���
	string sMainFileName=sFileName.substr(0,sFileName.size()-4);

	//�������ļ���
	return sMainFileName;
}