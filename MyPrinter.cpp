///////////////////////////////////////////////////////////////////////
//�ļ����� MyPrinter.cpp
//��  �ܣ� ��ӡ��ʵ���ļ�
//�����ߣ� ������
//��  ����2014/7/27
//��  �ģ�2014/7/27
//��  �ȣ�
//��  ע������
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>
#include <fstream>
#include "MIES.h"
#include "MyPrinter.h"

using namespace std;

CMyPrinter::CMyPrinter()
{

}

CMyPrinter::~CMyPrinter()
{
}
 
//��  �ܣ���ȡhtmlģ��
//��  ������ȡ�ļ����ͣ�1�Է�֪��ͬ����ģ��2ÿ���嵥ģ��
//����ֵ����
void CMyPrinter::ReadHtmlTemplate(int type)
{
	// ���ö�ȡ�ļ���ַ
	string path1="", path2="";
	if(type == PRINT_SELF_CHARGE_CONSENT_LETTER)
	{
		path1 = "MIESPrintTemplate\\selfChargeConsent.html";
		path2 = "MIESPrintTemplate\\itemForSelfChargeConsent.html";
	}
	else if(type == PRINT_DAILY_LIST)
	{
		path1 = "MIESPrintTemplate\\dailyList.html";
		path2 = "MIESPrintTemplate\\itemForDailyList.html";
	}
	else
		throw runtime_error("δ���������");

	// ===============��ȡ�������html����===============
	locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���     
	ifstream in(path1, ios::binary);	
	locale::global(loc);//�ָ�ȫ��locale
	if (!in) {
        throw runtime_error("�޷���htmlģ���ļ���");
    }

	string fileContent = "";
	in.seekg(0, ios::end);
	long length = in.tellg(); // �ļ��ܴ�С

	in.seekg(0, ios::beg); // ת���ϴζ�����λ��
	
	long len = length - in.tellg(); // ��ȡ���ζ�ȡ�����ݶγ���
	in.seekg(0);

	fileContent.resize(len); // ���û�������С
	in.read((char*)&fileContent[0], len); // ��ȡָ��λ�úͶγ�����
	in.close(); // �ر��ļ�������
	m_html = fileContent;

	// ===============��ȡ��Ŀ���������Ŀģ��===============
	loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���     
	ifstream in2(path2, ios::binary);	
	locale::global(loc);//�ָ�ȫ��locale
	if (!in2) {
        throw runtime_error("�޷���htmlģ���ļ���");
    }

	fileContent = "";
	in2.seekg(0, ios::end);
	length = in2.tellg(); // �ļ��ܴ�С

	in2.seekg(0, ios::beg); // ת���ϴζ�����λ��
	
	len = length - in2.tellg(); // ��ȡ���ζ�ȡ�����ݶγ���
	in2.seekg(0);

	fileContent.resize(len); // ���û�������С
	in2.read((char*)&fileContent[0], len); // ��ȡָ��λ�úͶγ�����
	in2.close(); // �ر��ļ�������
	// ��������Ŀ�����html����
	string::size_type index1 = fileContent.find("<PART1>");
	string::size_type index2 = fileContent.find("</PART1>");
	if(index1!=string::npos && index2!=string::npos && index1<index2)
	{
		m_itemTypeHtmlTemplate = fileContent.substr(index1+7, index2-index1-7);
	}
	// ��������Ŀhtml����
	index1 = fileContent.find("<PART2>", index2);
	index2 = fileContent.find("</PART2>", index2+1);
	if(index1!=string::npos && index2!=string::npos && index1<index2)
	{
		m_itemHtmlTemplate = fileContent.substr(index1+7, index2-index1-7);
	}
}
	
//��  �ܣ�����ѡ�еĴ���ӡ��Ŀ,����д�ļ���html,���ɴ�ӡ����
//��  �����������ͣ�1�Է�֪��ͬ����2ÿ���嵥;������Ϣ����;������Ŀ����
//����ֵ����
void CMyPrinter::ProducePrintContent(int type, map<string, string>& infoMap, map<string, vector<string> >& itemMap, map<string, double>& itemAmountMap)
{
	// �����벿��html
	string subHtml = "";
	string tmpHtml = "";
	string typeStr = "";
	string::size_type index = 0;
	string::size_type lastIndex = 0;

	if(type == PRINT_SELF_CHARGE_CONSENT_LETTER)
	{		
		string itemName = "";
		string date = "";
		string itemCode = "";
		string units = "";
		string itemType = "";
		double cost = 0.00;
		double amount = 0;
		double sum = 0.00;
		// ��¼�Ѵ�ӡ����
		map<string, double> hasPrintMap;
		// ���ɴ����벿��html
		map<string, vector<string> >::iterator iter = itemMap.begin();
		for(; iter!=itemMap.end(); ++iter)
		{
			// ����������html,�磺ҩƷ��
			typeStr = iter->first.substr(1);
			tmpHtml = m_itemTypeHtmlTemplate;
			index = tmpHtml.find("{$type}");
			if(index == string::npos)
				throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");
			tmpHtml.replace(index, 7, typeStr);
			// ���뵽�����벿��html
			subHtml += (tmpHtml+'\n');

			// ���ɶ�Ӧ�������¸�����Ŀ
			vector<string>::iterator iterVec = iter->second.begin();
			for(; iterVec!=iter->second.end(); ++iterVec)
			{
				// ��������Ӧ�����ڣ���Ŀ����
				index = iterVec->find("#1#");
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				date = iterVec->substr(0, index);

				lastIndex = index + 3;
				index = iterVec->find("#2#", lastIndex);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				itemName = iterVec->substr(lastIndex, index-lastIndex);

				lastIndex = index + 3;
				// ��ȡitemCode
				index = iterVec->find("#3#", lastIndex);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				itemCode = iterVec->substr(lastIndex, index-lastIndex);
				lastIndex = index + 3;

				// ��ȡ�������жϸ���Ŀ�Ƿ��ӡ��
				map<string, double>::iterator hasPrintMapIter = hasPrintMap.find(itemCode);
				map<string, double>::iterator amountMapIter = itemAmountMap.find(itemCode);
				if(hasPrintMapIter != hasPrintMap.end())
				{
					if(hasPrintMapIter->second == 1) // ����
					{ 
						continue;
					}
				}
				else if(amountMapIter != itemAmountMap.end())
				{
					amount = amountMapIter->second;
					hasPrintMap.insert(pair<string, double>(itemCode, 1));
				}

				// ����������������
				// ��λ
				lastIndex = iterVec->find("#5#", lastIndex) + 3;
				if(lastIndex == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				index = iterVec->find("#6#", lastIndex);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				units = iterVec->substr(lastIndex, index-lastIndex);
				lastIndex = index + 3;
				// ����
				index = iterVec->find("#7#", lastIndex);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				cost = atof(iterVec->substr(lastIndex, index-lastIndex).c_str());
				lastIndex = index + 3;
				// �������
				index = iterVec->find("#8#", lastIndex);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ�����ӡ���������⣡");
				itemType = iterVec->substr(lastIndex, index-lastIndex);
				// �ܼ�
				sum = cost*amount;

				// �滻��Ӧ��ģ�����ֶ�
				tmpHtml = m_itemHtmlTemplate;
				
				// �滻����
			/*	index = tmpHtml.find("{$date}");
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");				
				tmpHtml.replace(index, 7, date);*/

				// �滻��Ŀ����
				index = tmpHtml.find("{$itemName}", index);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");			
				tmpHtml.replace(index, 11, itemName);

				// �滻��Ŀ�������
				index = tmpHtml.find("{$itemType}", index);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");			
				tmpHtml.replace(index, 11, itemType);

				// �滻����
				index = tmpHtml.find("{$amount}", index);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");
				char amountStr[16];
				sprintf(amountStr, "%.2f", amount);
				tmpHtml.replace(index, 9, string(amountStr));

				// �滻��λ
				index = tmpHtml.find("{$units}", index);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");			
				tmpHtml.replace(index, 8, units);

				// �滻����
				index = tmpHtml.find("{$price}", index);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");
				char costStr[16];
				sprintf(costStr, "%.2f", cost);
				tmpHtml.replace(index, 8, string(costStr));

				// �滻���
				index = tmpHtml.find("{$sum}", index);
				if(index == string::npos)
					throw runtime_error("�Է�֪��ͬ������벿��ģ���ļ������⣡");
				char sumStr[16];
				sprintf(sumStr, "%.2f", sum);
				tmpHtml.replace(index, 6, string(sumStr));

				// ���뵽�����벿��html
				subHtml += (tmpHtml+'\n');
			}
		}
		// ���뵽�������html����
		index = m_html.find("{$itemHtml}");
		m_html.replace(index, 11, subHtml);
		
		// ���������Ϣ
		string title = "";
		string content = "";
		map<string, string>::iterator infoMapIter = infoMap.begin();
		for(; infoMapIter!=infoMap.end(); ++infoMapIter)
		{
			title = "{$"+infoMapIter->first+"}";
			content = infoMapIter->second;
			index = m_html.find(title);
			if(index == string::npos)
				continue;//throw runtime_error("�Է�֪��ͬ��������ģ���ļ������⣡");
			
			m_html.replace(index, title.length(), content);
		}
	}
	else if(type == PRINT_DAILY_LIST)
	{		
		string itemContent = "";
		// ���ɴ����벿��html
		map<string, vector<string> >::iterator iter = itemMap.begin();
		for(; iter!=itemMap.end(); ++iter)
		{
			// ����������html,�磺ҩƷ
			typeStr = iter->first;
			index = typeStr.find(":");
			// ��𣬷���С��
			string type = typeStr.substr(0, index);
			string charge = typeStr.substr(index+1);

			tmpHtml = m_itemTypeHtmlTemplate;
			index = tmpHtml.find("{$type}");
			if(index == string::npos)
				throw runtime_error("ÿ���嵥���벿��ģ���ļ������⣡");
			tmpHtml.replace(index, 7, type);
			index = tmpHtml.find("{$charge}", index);
			if(index == string::npos)
				throw runtime_error("ÿ���嵥���벿��ģ���ļ������⣡");
			tmpHtml.replace(index, 9, charge);
			// ���뵽�����벿��html
			subHtml += (tmpHtml+'\n');	

			// ���ɶ�Ӧ�������¸�����Ŀ					
			tmpHtml = m_itemHtmlTemplate;
			vector<string>::iterator iterVec = iter->second.begin();
			for(; iterVec!=iter->second.end(); ++iterVec)
			{
				// ��������Ӧ�����ڣ���Ŀ����
				lastIndex = 0;
				string title[7] = {"feeType", "itemCode", "itemName", "spec", "num", "price", "cost"};
				int i = 0;
				do
				{
					index = iterVec->find("##", lastIndex);
					if(index == string::npos)
						break;
					itemContent = iterVec->substr(lastIndex, index-lastIndex);
					lastIndex = index+2;

					index = tmpHtml.find("{$"+title[i]+"}");
					if(index == string::npos)
						throw runtime_error("ÿ���嵥���벿��ģ���ļ������⣡");	
					// �滻��Ӧ��ģ�����ֶ�		 
					tmpHtml.replace(index, title[i].length()+3, itemContent);
					i++;
				}while(true);

				// ���뵽�����벿��html
				index = subHtml.find("{$item}");
				subHtml.replace(index, 7, tmpHtml);
			}
		}
		// ���뵽�������html����
		index = m_html.find("{$itemHtml}");
		m_html.replace(index, 11, subHtml);
		
		// ���������Ϣ
		string title = "";
		string content = "";
		map<string, string>::iterator infoMapIter = infoMap.begin();
		for(; infoMapIter!=infoMap.end(); ++infoMapIter)
		{
			title = "{$"+infoMapIter->first+"}";
			content = infoMapIter->second;
			index = m_html.find(title);
			if(index == string::npos)
				throw runtime_error("�Է�֪��ͬ��������ģ���ļ������⣡");
			
			m_html.replace(index, title.length(), content);
		}
	}
	else
		throw runtime_error("δ���������");


}
	
//��  �ܣ�����html�ļ�
//��  ������
//����ֵ�����ɵ�html�ļ���·��
void CMyPrinter::ProduceHtmlFile()
{
	ofstream output("MIESTmpPrintFile\\tmpPrintFile.html", ios::out);
	output<<m_html;
	output.close();

	CString strPath;  
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH+1), MAX_PATH);  
	strPath.ReleaseBuffer();
	int index = strPath.ReverseFind('\\');
	CString desStr = strPath.Left(index);
	desStr += _T("\\MIESTmpPrintFile\\tmpPrintFile.html");
	
	ShellExecute(NULL, _T("open"), _T("WINWORD.EXE"), desStr, NULL, SW_SHOW);  
}