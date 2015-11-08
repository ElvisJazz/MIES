///////////////////////////////////////////////////////////////////////
//�ļ����� LogicHandler.cpp
//��  �ܣ� ҽ�����ϵͳ���ݿ��ȡ�����߼�������ʵ���ļ�
//�����ߣ� ������
//��  ����2014/7/8
//��  �ģ�2014/7/8
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogicHandler.h"
#include <time.h>


LogicHandler::LogicHandler(void)
{

}


LogicHandler::~LogicHandler(void)
{
}

//�� �ܣ������뵱ǰʱ����ʽ�磺2014-02-13 9:33:50�����������Сʱ����������ʾС�ڵ�ǰʱ�䣬-1��ʾ���ڵ�ǰʱ��
//�� �룺���Ƚ�ʱ��
//�� ����ʱ���
int LogicHandler::CalculateTimeDifference(string timeStr)
{
	timeStr += ' ';
	string::size_type index1 = 0, index2 = 0;
	string::size_type length = timeStr.length();
	
	struct tm* timePtr = new struct tm();

	// �������ܴ��ڵĿո�
	for(; index1<length; index1++)
	{
		if(timeStr[index1] != ' ')
			break;
	}
	// ������
	index2 = timeStr.find('-', index1);
	timePtr->tm_year = atoi((timeStr.substr(index1, index2-index1)).c_str())-1900;
	// ����·�
	index1 = index2 + 1;
	index2 = timeStr.find('-', index1);
	timePtr->tm_mon = atoi((timeStr.substr(index1, index2-index1)).c_str())-1;
	// �����
	index1 = index2 + 1;
	index2 = timeStr.find(' ', index1);
	timePtr->tm_mday = atoi((timeStr.substr(index1, index2-index1)).c_str());

	// �������ܴ��ڵĿո�
	index1 = index2;
	for(; index1<length; index1++)
	{
		if(timeStr[index1] != ' ')
			break;
	}
	// ���ʱ
	index2 = timeStr.find(':', index1);
	timePtr->tm_hour = atoi((timeStr.substr(index1, index2-index1)).c_str());
	// ��÷�
	index1 = index2 + 1;
	index2 = timeStr.find(':', index1);
	timePtr->tm_min = atoi((timeStr.substr(index1, index2-index1)).c_str());
	// �����
	index1 = index2 + 1;
	index2 = timeStr.find(' ', index1);
	timePtr->tm_sec = atoi((timeStr.substr(index1, index2-index1)).c_str());

	timePtr->tm_isdst = -1;
	// ת��������
	time_t otime = mktime(timePtr);
	time_t stime = time(NULL);
	struct tm* sblock = localtime(&stime);
	stime = mktime(sblock);

	double offsetSecond = difftime(stime, otime);

	// �ͷ�ָ���ڴ�
	delete timePtr;
	if(offsetSecond < 0)
		return -1;
	else
		return ((long)offsetSecond) / 3600;
}


//��  �ܣ������ֶ���������Ӧ����
//��  ������������ֶ�����Ԥ�����ѯ����
//����ֵ����Ӧ����������
string LogicHandler::GetChineseName(string field)
{
	if(field == "NAME")
		return "��������";
	else if(field == "JZLB")
		return "ҽ������";
	else if(field == "YLFYZE")
		return "����ҽ�Ʒ���";
	else if(field == "PREPAYMENTS")
		return "Ԥ����";
	else if(field == "GRZF")
		return "�����Ը�";
	else if(field == "GRFDL")
		return "�Ը����ñ���(%)";
	else if(field == "CZFJE")
		return "���Էѷ���";
	else if(field == "ELZFZF")
		return "���Էѷ���";
	else if(field == "JBYLFDFY")
		return "ͳ��ҽ�Ʒ���";
	else if(field == "TCZFHJ")
		return "ͳ��֧���ϼ�";
	else if(field == "YBZFYJZ")
		return "ҽ��֧�����Ԥ��ֵ";
	else if(field == "SBPFHJ")
		return "�̱��⸶����";
	else if(field == "JBYLTCBLF")
		return "ͳ��֧��1";
	else if(field == "JBYLBLZHF")
		return "ͳ��֧��2���û����ϣ�";
	else if(field == "TCZF")
		return "ͳ��֧��";
	else if(field == "JJZF")
		return "����֧��";
	else if(field == "GRBLZF")
		return "���˱���֧��";
	else if(field == "NDYLFY")
		return "����ۼ�ҽ������";


	return field;
}

//�� �ܣ�����ֵ����ɫӳ���ϵ��������ɫ�ַ���
//�� �룺��ɫ�����ַ���������ֵ
//�� ������ɫ�ַ���
string LogicHandler::GetColorStr(string numColorCs, double rate)
{
	string::size_type index1=0, index2=0;
	// ��һ����������ɫ����
	string lastColorCsStr = "";
	// ��������ֵ��������
	double rateCs = 0.0;

	index2 = numColorCs.find(":");
	while(index2 != string::npos)
	{
		rateCs = atof(numColorCs.substr(index1, index2-index1).c_str());
		if(rate > rateCs)
		{
			index1 = numColorCs.find(";", index2);
			if(index1 != string::npos)
				lastColorCsStr = numColorCs.substr(index2+1, index1-index2-1);
			else
			{
				AfxMessageBox(_T("�����������������⣡"));
				return "";
			}
			index2 = numColorCs.find(":", index1);
			index1++;
		}
		else
			return lastColorCsStr;
	}

	return lastColorCsStr;
}
 
//�� �ܣ��滻Ŀ�괮����ɫ����
//�� �룺Ŀ�괮����ɫ�����ַ���������ֵ
//�� ������ɫ�ַ���
void LogicHandler::ReplaceColorStr(string &desStr, string desField, string numColorCs, double rate)
{
	string colorStr = GetColorStr(numColorCs, rate);
	if(!colorStr.empty())
	{
		string chiName = GetChineseName(desField);
		string::size_type index1 = desStr.find(chiName);
		if(index1 != string::npos)
		{
			string::size_type index2 = desStr.find("<<", index1);
			index1 = desStr.find(">>", index2);
			desStr.replace(index2+2, index1-index2-2, colorStr);
		}
	}
}