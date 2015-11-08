///////////////////////////////////////////////////////////////////////
//�ļ����� MyPrinter.h
//��  �ܣ� ��ӡ��ͷ�ļ�
//�����ߣ� ������
//��  ����2014/7/27
//��  �ģ�2014/7/27
//��  �ȣ�
//��  ע������
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

	// ��ȡhtmlģ��
	void ReadHtmlTemplate(int type);
	// ����ѡ�еĴ���ӡ��Ŀ,����д�ļ���html,���ɴ�ӡ����
	void ProducePrintContent(int type, map<string, string>& infoMap, map<string, vector<string> >& itemMap, map<string, double>& itemAmountMap);
	// ����html�ļ�
	void ProduceHtmlFile();

private:
	// ��Ŀ�����htmlģ��
	string m_itemTypeHtmlTemplate;
	// ��Ŀhtmlģ��
	string m_itemHtmlTemplate;
	// �������html����
	string m_html;

};


