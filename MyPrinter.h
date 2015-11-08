///////////////////////////////////////////////////////////////////////
//文件名： MyPrinter.h
//功  能： 打印类头文件
//开发者： 贾治中
//创  建：2014/7/27
//修  改：2014/7/27
//长  度：
//备  注：暂无
///////////////////////////////////////////////////////////////////////
#pragma once

#include <map>
#include <vector>

#define PRINT_SELF_CHARGE_CONSENT_LETTER 1
#define	PRINT_DAILY_LIST 2

class CMyPrinter
{
public:
	CMyPrinter();
	virtual ~CMyPrinter();

	// 读取html模板
	void ReadHtmlTemplate(int type);
	// 根据选中的待打印项目,填充待写文件的html,生成打印内容
	void ProducePrintContent(int type, map<string, string>& infoMap, map<string, vector<string> >& itemMap, map<string, double>& itemAmountMap);
	// 生成html文件
	void ProduceHtmlFile();

private:
	// 项目类别名html模板
	string m_itemTypeHtmlTemplate;
	// 项目html模板
	string m_itemHtmlTemplate;
	// 待插入的html主体
	string m_html;

};


