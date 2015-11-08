/////////////////////////////////////////////////////////////////////
// �ļ�����sql_processor.cpp
// ���ܣ�����QUERY�ڵ㣬ִ�в�ѯ
// �����ߣ���ҫ��
// ʱ�䣺2014��4��13��
// �޸ģ�2014��7��9��
// ���ȣ�219��
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// MFCͷ�ļ�
#include "sql_processor.h"		// Sqlִ�ж���ͷ�ļ�

// ���ƣ�ParseQuery()
// ���ܣ�����XML�ĵ���SQL���
// ������TiXmlElement *
// ���أ���
// �쳣��runtime_error
void CSqlProcessor::ParseQuery(XMLElement * pQuery)
{
	// �ж�����Ϸ���
	if (pQuery==NULL) return;
	// ������ݽṹ
	m_vfieldNames.clear();
	m_vparamNames.clear();
	m_ssql.clear();

	// ��ȡ����
	const char * s = pQuery->Attribute("type");
	if ( s ) m_sTypeName = s;
	else throw runtime_error("HIS��ѯ�����ļ�����ʧ��.QUERY���type����ȱʧ."); // �׳��쳣
	// ����MAPPING���
	_ParseQuery(pQuery);
}

// ���ƣ�Query()
// ���ܣ���ѯ���ݿ�
// ������map<string,string>&, CDB*
// ���أ���
RECORD_VECTOR CSqlProcessor::Query(map<string,string> & parammap, CDB * pDB)
{
	// �����Ϸ��Լ��
	if ( pDB==NULL ) return RECORD_VECTOR();
	// �ع���ѯ��䣬��д����
	string ssql = ReBuildQuery(parammap);
	// ִ�в�ѯ
	return QueryDB(pDB,ssql);
}


// ���ƣ�QueryDB()
// ���ܣ�ִ�����ݿ��ѯ
// ������CDB*, string
// ���أ���
RECORD_VECTOR CSqlProcessor::QueryDB(CDB *pDB, string ssql)
{
	// �������ؽ��
	RECORD_VECTOR records;
	// �ַ�������ת��
	CString strSql(ssql.c_str());
	strSql.Trim();

	TRY
	{
		// ������¼��
		CRecordset recordset(&(pDB->m_database));
		// ִ�в�ѯ
		recordset.Open(CRecordset::forwardOnly, strSql);

		// ������¼��
		for(; !recordset.IsEOF(); recordset.MoveNext())
		{
			// ��������
			std::vector<pair<string,string>> record;
			// ��ȡ��Ϣ
			// ����SQL�����ֶ����ݻ�ȡ����
			unsigned int fieldSize = m_vfieldNames.size();
			for(unsigned int i=0; i<fieldSize; i++)
			{
				// �õ�һ���ֶ���
				CString strFieldName(m_vfieldNames[i].c_str()); // �ֶ�����string����ת��ΪCString����
				CStringW var;
				// ���ʼ�¼�����õ��ֶ�ֵ
				recordset.GetFieldValue(strFieldName, var);
				string sfieldValue = MapCStringToString(var);
				// ��ӵ��ֶ�ֵ����
				record.push_back(std::pair<string,string>(m_vfieldNames[i],sfieldValue));
			}
			// ��ӵ���ѯ�������
			records.push_back(record);
		}
		// �رռ�¼��
		recordset.Close();
	}
	CATCH_ALL(e)
	{
		// ���ݿ��ѯ�쳣��ʾ
		throw runtime_error("���ݿ��ѯ�쳣.����Ͽ���HIS��ѯ�����ļ�����.�ֶ�������SQL��䲻ƥ��.\n"+ssql);
	}
	END_CATCH_ALL;
	// ���ز�ѯ���
	return records;
}

// ���ƣ�ReBuildQuery()
// ���ܣ���ѯ����ع�
// ������map<string,string>&
// ���أ�string
string CSqlProcessor::ReBuildQuery(map<string, string> & map)
{
	// ����sql���
	string ssql = m_ssql;
	// �滻sql��������еĲ���
	unsigned int paramSize = m_vparamNames.size();
	for (unsigned int i=0; i<paramSize; i++)
	{
		// ��ȡ��������
		string & param = m_vparamNames[i];
		// ���ݲ������ƣ��ڲ���map�в�ѯ
		std::map<string, string>::iterator iter = map.find(param);
		// ������ڲ�����ֵ�����滻SQL����еĵ�һ����ǣ��磿��
		if (iter!=map.end())
		{
			string value = iter->second;
			// �����к��пɲ��滻�����ű�־
			if(iter->second.find("####")!=string::npos) 
				ReplaceFirst(value, string("####"), string(""));
			else if(iter->second.find("'")!=string::npos) // �����а���������
				value = "'@@@@'";
			else
				value = "'"+value+"'";
			ReplaceFirst(ssql, m_smark, value);
		}
		// ��������ڲ�����ֵ��������쳣��ʹ�������ֵ�滻��һ�����
		else
		{
			// ʹ���������滻������ȷ����ѯ����������Ҳ�ѯ���Ϊ��
			string tag="'@@@@'";
			ReplaceFirst(ssql, m_smark, tag);
		}
	}
	// ȡ��SELECTǰ�Ŀո�����
	if (ssql.find_first_not_of(" \n\r\t")!=string::npos)
		ssql = ssql.substr(ssql.find_first_not_of(" \n\r\t"));
	// ���ؽ��
	//AfxMessageBox(CString(ssql.c_str()));
	return ssql;
}

// ���ƣ�ReplaceFirst()
// ���ܣ��滻�ַ����е�һ���Ӵ�
// ������string&,string&,string&
// ���أ���
void CSqlProcessor::ReplaceFirst(string & originStr, string & oldStr, string & newStr)
{
	// ��һ���Ӵ���λ��
	unsigned int nFirstIndex = originStr.find(oldStr);
	if (nFirstIndex!=string::npos)
		originStr.replace(nFirstIndex,oldStr.length(),newStr);
}


// ���ƣ�_ParseQuery()
// ���ܣ���ȡMAPPINGԪ�صľ�������
// ������XmlElement*
// ���أ���
// �쳣��runtime_error
void CSqlProcessor::_ParseQuery(XMLElement *pQuery)
{
	// ��ȡ�ӽ�㣬����ѭ��
	XMLElement * pEle = pQuery->FirstChildElement();
	for (; pEle!=NULL; pEle=pEle->NextSiblingElement())
	{
		// ��ȡԪ������
		string eleName;
		const char * s = pEle->Value();
		// �����쳣
		if ( s ) eleName = s;
		else throw runtime_error("HIS��ѯ�����ļ�����ʧ��.QUERY�����ӽ�㶨�����.");

		// ����Ԫ�����Ƴ�ȡԪ�ص�����
		if (eleName=="SELECTS")
			GetValues(pEle, m_vfieldNames);
		else if (eleName=="SQL")
		{
			const char * s = pEle->GetText();
			if ( s ) m_ssql = s;
			else throw runtime_error("HIS��ѯ�����ļ�����ʧ��.SQL���û������.");
		}
		else if (eleName=="PARAMETERS")
			GetValues(pEle, m_vparamNames);
	}
}

// ���ƣ�GetValues()
// ���ܣ���ȡ�б�Ԫ�ص�����
// ������XmlElement*,vector<string>&
// ���أ���
// �쳣��runtime_error
void CSqlProcessor::GetValues(XMLElement * pEle, vector<string> & values)
{
	// ���������ӽ��
	XMLElement * pNext = pEle->FirstChildElement();
	for (; pNext!=NULL; pNext=pNext->NextSiblingElement())
	{
		// ��ȡԪ�ص�����
		const char * value = pNext->GetText();
		// ��ӵ�����
		if (value) values.push_back(value);
		else throw runtime_error("HIS�����ļ�����ʧ��.QUERY����б�Ԫ�ؽ���ʧ��.");
	}
}

// ���ƣ�Close()
// ���ܣ��رն���
// ��������
// ���أ���
void CSqlProcessor::Close()
{
	// �����ڴ�
	m_vfieldNames.clear();
	m_vparamNames.clear();
	m_ssql.clear();
	m_smark.clear();
	m_sTypeName.clear();
}