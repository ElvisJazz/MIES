///////////////////////////////////////////////////////////////////////
//文件名： MyPrinter.cpp
//功  能： 打印类实现文件
//开发者： 贾治中
//创  建：2014/7/27
//修  改：2014/7/27
//长  度：
//备  注：暂无
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
 
//功  能：读取html模板
//参  数：读取文件类型：1自费知情同意书模板2每日清单模板
//返回值：无
void CMyPrinter::ReadHtmlTemplate(int type)
{
	// 设置读取文件地址
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
		throw runtime_error("未定义操作！");

	// ===============读取待插入的html主体===============
	locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境     
	ifstream in(path1, ios::binary);	
	locale::global(loc);//恢复全局locale
	if (!in) {
        throw runtime_error("无法打开html模板文件！");
    }

	string fileContent = "";
	in.seekg(0, ios::end);
	long length = in.tellg(); // 文件总大小

	in.seekg(0, ios::beg); // 转到上次读到的位置
	
	long len = length - in.tellg(); // 获取本次读取的数据段长度
	in.seekg(0);

	fileContent.resize(len); // 设置缓冲区大小
	in.read((char*)&fileContent[0], len); // 读取指定位置和段长数据
	in.close(); // 关闭文件输入流
	m_html = fileContent;

	// ===============读取项目类别名和项目模板===============
	loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境     
	ifstream in2(path2, ios::binary);	
	locale::global(loc);//恢复全局locale
	if (!in2) {
        throw runtime_error("无法打开html模板文件！");
    }

	fileContent = "";
	in2.seekg(0, ios::end);
	length = in2.tellg(); // 文件总大小

	in2.seekg(0, ios::beg); // 转到上次读到的位置
	
	len = length - in2.tellg(); // 获取本次读取的数据段长度
	in2.seekg(0);

	fileContent.resize(len); // 设置缓冲区大小
	in2.read((char*)&fileContent[0], len); // 读取指定位置和段长数据
	in2.close(); // 关闭文件输入流
	// 解析出项目类别名html部分
	string::size_type index1 = fileContent.find("<PART1>");
	string::size_type index2 = fileContent.find("</PART1>");
	if(index1!=string::npos && index2!=string::npos && index1<index2)
	{
		m_itemTypeHtmlTemplate = fileContent.substr(index1+7, index2-index1-7);
	}
	// 解析出项目html部分
	index1 = fileContent.find("<PART2>", index2);
	index2 = fileContent.find("</PART2>", index2+1);
	if(index1!=string::npos && index2!=string::npos && index1<index2)
	{
		m_itemHtmlTemplate = fileContent.substr(index1+7, index2-index1-7);
	}
}
	
//功  能：根据选中的待打印项目,填充待写文件的html,生成打印内容
//参  数：生成类型：1自费知情同意书2每日清单;基本信息容器;分组项目容器
//返回值：无
void CMyPrinter::ProducePrintContent(int type, map<string, string>& infoMap, map<string, vector<string> >& itemMap, map<string, double>& itemAmountMap)
{
	// 待插入部分html
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
		// 记录已打印部分
		map<string, double> hasPrintMap;
		// 生成待插入部分html
		map<string, vector<string> >::iterator iter = itemMap.begin();
		for(; iter!=itemMap.end(); ++iter)
		{
			// 生成类型名html,如：药品类
			typeStr = iter->first.substr(1);
			tmpHtml = m_itemTypeHtmlTemplate;
			index = tmpHtml.find("{$type}");
			if(index == string::npos)
				throw runtime_error("自费知情同意书插入部分模板文件有问题！");
			tmpHtml.replace(index, 7, typeStr);
			// 加入到待插入部分html
			subHtml += (tmpHtml+'\n');

			// 生成对应类型名下各个项目
			vector<string>::iterator iterVec = iter->second.begin();
			for(; iterVec!=iter->second.end(); ++iterVec)
			{
				// 解析出对应的日期，项目名称
				index = iterVec->find("#1#");
				if(index == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				date = iterVec->substr(0, index);

				lastIndex = index + 3;
				index = iterVec->find("#2#", lastIndex);
				if(index == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				itemName = iterVec->substr(lastIndex, index-lastIndex);

				lastIndex = index + 3;
				// 获取itemCode
				index = iterVec->find("#3#", lastIndex);
				if(index == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				itemCode = iterVec->substr(lastIndex, index-lastIndex);
				lastIndex = index + 3;

				// 获取数量并判断该项目是否打印过
				map<string, double>::iterator hasPrintMapIter = hasPrintMap.find(itemCode);
				map<string, double>::iterator amountMapIter = itemAmountMap.find(itemCode);
				if(hasPrintMapIter != hasPrintMap.end())
				{
					if(hasPrintMapIter->second == 1) // 跳过
					{ 
						continue;
					}
				}
				else if(amountMapIter != itemAmountMap.end())
				{
					amount = amountMapIter->second;
					hasPrintMap.insert(pair<string, double>(itemCode, 1));
				}

				// 继续解析其他部分
				// 单位
				lastIndex = iterVec->find("#5#", lastIndex) + 3;
				if(lastIndex == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				index = iterVec->find("#6#", lastIndex);
				if(index == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				units = iterVec->substr(lastIndex, index-lastIndex);
				lastIndex = index + 3;
				// 单价
				index = iterVec->find("#7#", lastIndex);
				if(index == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				cost = atof(iterVec->substr(lastIndex, index-lastIndex).c_str());
				lastIndex = index + 3;
				// 费用类别
				index = iterVec->find("#8#", lastIndex);
				if(index == string::npos)
					throw runtime_error("自费知情同意书打印解析出问题！");
				itemType = iterVec->substr(lastIndex, index-lastIndex);
				// 总价
				sum = cost*amount;

				// 替换对应的模板中字段
				tmpHtml = m_itemHtmlTemplate;
				
				// 替换日期
			/*	index = tmpHtml.find("{$date}");
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");				
				tmpHtml.replace(index, 7, date);*/

				// 替换项目名称
				index = tmpHtml.find("{$itemName}", index);
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");			
				tmpHtml.replace(index, 11, itemName);

				// 替换项目费用类别
				index = tmpHtml.find("{$itemType}", index);
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");			
				tmpHtml.replace(index, 11, itemType);

				// 替换数量
				index = tmpHtml.find("{$amount}", index);
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");
				char amountStr[16];
				sprintf(amountStr, "%.2f", amount);
				tmpHtml.replace(index, 9, string(amountStr));

				// 替换单位
				index = tmpHtml.find("{$units}", index);
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");			
				tmpHtml.replace(index, 8, units);

				// 替换单价
				index = tmpHtml.find("{$price}", index);
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");
				char costStr[16];
				sprintf(costStr, "%.2f", cost);
				tmpHtml.replace(index, 8, string(costStr));

				// 替换金额
				index = tmpHtml.find("{$sum}", index);
				if(index == string::npos)
					throw runtime_error("自费知情同意书插入部分模板文件有问题！");
				char sumStr[16];
				sprintf(sumStr, "%.2f", sum);
				tmpHtml.replace(index, 6, string(sumStr));

				// 加入到待插入部分html
				subHtml += (tmpHtml+'\n');
			}
		}
		// 加入到待插入的html主体
		index = m_html.find("{$itemHtml}");
		m_html.replace(index, 11, subHtml);
		
		// 插入基本信息
		string title = "";
		string content = "";
		map<string, string>::iterator infoMapIter = infoMap.begin();
		for(; infoMapIter!=infoMap.end(); ++infoMapIter)
		{
			title = "{$"+infoMapIter->first+"}";
			content = infoMapIter->second;
			index = m_html.find(title);
			if(index == string::npos)
				continue;//throw runtime_error("自费知情同意书主体模板文件有问题！");
			
			m_html.replace(index, title.length(), content);
		}
	}
	else if(type == PRINT_DAILY_LIST)
	{		
		string itemContent = "";
		// 生成待插入部分html
		map<string, vector<string> >::iterator iter = itemMap.begin();
		for(; iter!=itemMap.end(); ++iter)
		{
			// 生成类型名html,如：药品
			typeStr = iter->first;
			index = typeStr.find(":");
			// 类别，费用小结
			string type = typeStr.substr(0, index);
			string charge = typeStr.substr(index+1);

			tmpHtml = m_itemTypeHtmlTemplate;
			index = tmpHtml.find("{$type}");
			if(index == string::npos)
				throw runtime_error("每日清单插入部分模板文件有问题！");
			tmpHtml.replace(index, 7, type);
			index = tmpHtml.find("{$charge}", index);
			if(index == string::npos)
				throw runtime_error("每日清单插入部分模板文件有问题！");
			tmpHtml.replace(index, 9, charge);
			// 加入到待插入部分html
			subHtml += (tmpHtml+'\n');	

			// 生成对应类型名下各个项目					
			tmpHtml = m_itemHtmlTemplate;
			vector<string>::iterator iterVec = iter->second.begin();
			for(; iterVec!=iter->second.end(); ++iterVec)
			{
				// 解析出对应的日期，项目名称
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
						throw runtime_error("每日清单插入部分模板文件有问题！");	
					// 替换对应的模板中字段		 
					tmpHtml.replace(index, title[i].length()+3, itemContent);
					i++;
				}while(true);

				// 加入到待插入部分html
				index = subHtml.find("{$item}");
				subHtml.replace(index, 7, tmpHtml);
			}
		}
		// 加入到待插入的html主体
		index = m_html.find("{$itemHtml}");
		m_html.replace(index, 11, subHtml);
		
		// 插入基本信息
		string title = "";
		string content = "";
		map<string, string>::iterator infoMapIter = infoMap.begin();
		for(; infoMapIter!=infoMap.end(); ++infoMapIter)
		{
			title = "{$"+infoMapIter->first+"}";
			content = infoMapIter->second;
			index = m_html.find(title);
			if(index == string::npos)
				throw runtime_error("自费知情同意书主体模板文件有问题！");
			
			m_html.replace(index, title.length(), content);
		}
	}
	else
		throw runtime_error("未定义操作！");


}
	
//功  能：生成html文件
//参  数：无
//返回值：生成的html文件的路径
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