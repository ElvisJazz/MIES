/////////////////////////////////////////////////////////////////////
// �ļ�����sql_processor.h
// ���ܣ�1��������ѯ��䣬2��ִ�в�ѯ
// �����ߣ���ҫ��
// ʱ�䣺2014��4��13��
// �޸ģ�2014��7��9��
// ���ȣ�53��
// ʹ��˵����
// 1��ParseQuery()��ʾ�������̣�ֻ��ִ��һ��
// 2��Query()��ʾ��ѯ���̣����ظ�����
/////////////////////////////////////////////////////////////////////

#ifndef SQL_PARSER
#define SQL_PARSER

#include "sql_public.h"						// ���ݿ��ѯͷ�ļ�

// ������
class CSqlProcessor
// �ඨ��
{
/////////////////////////////////
	// ˽�г�Ա����
private:
	vector<string> m_vfieldNames;			// �ֶ���������
	vector<string> m_vparamNames;			// ����������
	string m_ssql;							// SQL���
	string m_smark;							// �������
	string m_sTypeName;						// �������ͣ���������ǰ��ѯ����

/////////////////////////////////
	// ��������
public:
	CSqlProcessor(){m_smark="?";};								// ���캯��
	void Close();												// �رն���
	~CSqlProcessor(){Close();};									// ��������
	string GetType(){return m_sTypeName;}						// ��ö�������
	vector<string> GetFieldNames(){return m_vfieldNames;};		// ����ֶ�������
	void ParseQuery(XMLElement * pQuery);						// XMLԪ�ؽ�������
	RECORD_VECTOR Query(map<string,string> & map, CDB * pDB);	// ���ݿ��ѯ����

/////////////////////////////////
	// ˽�к���
private:
	void _ParseQuery(XMLElement * pQuery);										// ����QUERY���ý��		
	void GetValues(XMLElement * pEle, vector<string> & values);					// ���XMLԪ�ؽ�������
	void ParseSelects(XMLElement * pSelects, string sqlName);					// ����SELECTS���
	void ReplaceFirst(string & originStr, string & oldStr, string & newStr);	// �滻��һ�γ��ֵ��Ӵ�
	string ReBuildQuery(map<string,string> & map);								// �ع���ѯ���
	RECORD_VECTOR QueryDB(CDB * pDB, string sql);								// ��ѯ���ݿ����
};

#endif