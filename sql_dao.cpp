/////////////////////////////////////////////////////////////////////
// �ļ�����sql_dao.cpp
// ���ܣ�1���ṩ���ݿ�Ͳ�ѯ��ʼ��������2����������ò�ѯ�ĺ���
// �����ߣ���ҫ��
// ʱ�䣺2014��7��9��
// ���ȣ�96��
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// MFCͷ�ļ�
#include "sql_dao.h"			// CSqlDaoͷ�ļ�

// ���ƣ�Init()
// ���ܣ���ʼ�����ݿ�Ͳ�ѯ����
// ������string,string
// ����ֵ����
// �쳣��runtime_error
void CSqlDao::Init(string sDBConfigFile, string sSqlFile)
{
	// ��ʼ�����ݿ�
	m_db.Init(sDBConfigFile);
	// ���������ļ�
	ParseDoc(sSqlFile);
}

// ���ƣ�Close()
// ���ܣ��ر����ݿ����
// ������string,string
// ����ֵ����
// �쳣��runtime_error
void CSqlDao::Close()
{
	// �ر����ݿ����
	m_db.Close();
	// ��������
	map<string,CSqlGroup *>::iterator iter;
	for (iter=m_groups.begin();iter!=m_groups.end();iter++)
	{
		iter->second->Close();
		iter->second=NULL;
	}
	m_groups.clear();
}


// ���ƣ�ParseDoc()
// ���ܣ����������ļ�
// ������string,�����ļ�����·��
// ����ֵ����
void CSqlDao::ParseDoc(string sSqlFile)
{
	// �����ĵ�����
	tinyxml2::XMLDocument sqlDoc;
	// �����ĵ�
	sqlDoc.LoadFile(sSqlFile.c_str());
	
	// �����ĵ�
	XMLElement * pRoot = sqlDoc.RootElement();	// ��ȡ�����
	// �쳣����
	if ( !pRoot )
		throw runtime_error("HIS��ѯ�����ļ�����ʧ��.�����ļ������ڻ�XML�ļ�ȱ�ٸ����.");
	// �������е�GROUP
	XMLElement * pGroup = pRoot->FirstChildElement();  // ��ȡ��һ��GROUP
	for( ; pGroup != NULL; pGroup = pGroup->NextSiblingElement())
	{
		// ��ȡ��㼤��״̬
		string sactivate;
		const char * s = pGroup->Attribute("activate");
		// �쳣����
		if ( s ) sactivate = s;
		else throw runtime_error("HIS��ѯ�����ļ�����ʧ��.GROUP����activate����ȱʧ.");
		// �ж��Ƿ񼤻�
		if ( sactivate.find("yes")==string::npos ) continue; // ���û�м�����ԣ���������
		// ����GROUP��㴦�����
		CSqlGroup * pSqlGroup = new CSqlGroup();
		// ������ѯ��
		pSqlGroup->ParseGroup(pGroup);
		// ��ӵ���ѯ������
		m_groups.insert(std::pair<string,CSqlGroup*>(pSqlGroup->GetType(), pSqlGroup));
	}
}

// ���ƣ�Query()
// ���ܣ�ִ���ض��Ĳ�ѯ��
// ������string,����
// ����ֵ��RESULT_MAP
RESULT_MAP CSqlDao::Query(string sGroupName, map<string,string> & parammap)
{
	// �Ϸ��Լ��
	if (sGroupName.empty()) return RESULT_MAP();
	// ͨ�����ƻ�ò�ѯ��
	map<string,CSqlGroup *>::iterator iter;
	iter = m_groups.find(sGroupName);
	if (iter==m_groups.end()) return RESULT_MAP();
	CSqlGroup * pGroup = iter->second;
	// ִ�в�ѯ�����ز�ѯ���
	return pGroup->Query(parammap,&m_db);
}