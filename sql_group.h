/////////////////////////////////////////////////////////////////////
// �ļ�����sql_group.h
// ���ܣ�1�������������ѯ��䣻2��������ִ�в�ѯ���
// �����ߣ���ҫ��
// ʱ�䣺2014��7��8��
// ���ȣ�34��
/////////////////////////////////////////////////////////////////////

#ifndef SQL_GROUP
#define SQL_GROUP

#include "sql_processor.h"	// SQL��ѯ���ִ�ж���ͷ�ļ�

// ������
class CSqlGroup
// �ඨ��
{
/////////////////////////////////
	// ˽�г�Ա����
private:
	vector<CSqlProcessor *>  m_processors;	// SQL����������
	string m_sTypeName;						// ��ѯ������

/////////////////////////////////
	// ������Ա����
public:
	void Close();							// �رն���
	~CSqlGroup(){Close();};					// ��������
	void ParseGroup(XMLElement * pGroup);	// ����GROUP���
	string GetType(){return m_sTypeName;};	// ��ȡ��ѯ������
	RESULT_MAP Query(map<string,string> & parammap, CDB * pDB);	// ִ�в�ѯ
};

#endif