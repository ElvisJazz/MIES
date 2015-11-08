/////////////////////////////////////////////////////////////////////
// �ļ�����sql_group.cpp
// ���ܣ�1�������������ѯ��䣻2��������ִ�в�ѯ���
// �����ߣ���ҫ��
// ʱ�䣺2014��7��9��
// ���ȣ�81��
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// MFCͷ�ļ�
#include "sql_group.h"			// CSqlGroupͷ�ļ�

// ���ƣ�ParseGroup()
// ���ܣ�����GROUP���
// ������XmlElement*
// ���أ���
void CSqlGroup::ParseGroup(XMLElement * pGroup)
{
	// �����Ϸ��Լ��
	if (pGroup==NULL) return;
	// ��ȡGROUP������������
	const char * sTypeName = pGroup->Attribute("type");
	if ( sTypeName ) m_sTypeName = sTypeName;
	else throw runtime_error("HIS��ѯ�����ļ�����ʧ��.GROUP����type����ȱʧ.");
	// ����GROUP������е��ӽ��
	XMLElement * pQuery = pGroup->FirstChildElement();
	for(;pQuery!=NULL;pQuery=pQuery->NextSiblingElement())
	{
		// ��ȡ��㼤������
		string sactivate;
		const char * s = pQuery->Attribute("activate");
		if ( s ) sactivate = s;
		else throw runtime_error("HIS��ѯ�����ļ�����ʧ��.GROUP����activate����ȱʧ.");
		// �жϵ�ǰ��Ч��������
		if (sactivate.find("yes")==string::npos) continue;

		// ����SQL��ѯ���󣬽���QUERY���
		CSqlProcessor * pSqlPro = new CSqlProcessor();
		pSqlPro->ParseQuery(pQuery);
		// ���������ӵ�SQL��ѯ����
		m_processors.push_back(pSqlPro);
	}
}

// ���ƣ�Query()
// ���ܣ�ִ��С���ѯ
// ������map<string,string>, CDB
// ���أ�RESULT_MAP
RESULT_MAP CSqlGroup::Query(map<string,string> & parammap, CDB * pDB)
{
	// �������ؽ��
	RESULT_MAP resultMap;
	// �����Ϸ��Լ��
	if ( pDB==NULL ) return resultMap;
	// ִ�в�ѯ
	for (unsigned int i=0; i<m_processors.size(); i++)
	{
		CSqlProcessor * pPro = m_processors[i];
		std::string sTypeName = pPro->GetType();
		RECORD_VECTOR records = pPro->Query(parammap,pDB);
		resultMap.insert(std::pair<string,RECORD_VECTOR>(sTypeName,records));
	}
	// ���ز�ѯ���
	return resultMap;
}

// ���ƣ�Close()
// ���ܣ��رն���
// ��������
// ���أ���
void CSqlGroup::Close()
{
	// �����ڴ�
	m_sTypeName.clear();

	// ����ָ���������
	for (unsigned int i=0; i<m_processors.size(); i++)
	{
		m_processors[i]->Close();
		m_processors[i]=NULL;
	}
	m_processors.clear();
}