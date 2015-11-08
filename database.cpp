/////////////////////////////////////////////////////////////////////////////////////////////////
// ���ƣ�database.cpp
// ���ܣ�ʵ��CDB��
// �����ߣ���ҫ��
// ʱ�䣺20140410
// �޸ģ�20140502
// �޸ģ�20140525
// �޸ģ�20140531��GetConnectionParams()
// ���ȣ�239��
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"							// MFC��׼ͷ�ļ�
#include "database.h"						// �������ݿ�ͷ�ļ�
//#include "Helper.h"						// �ַ�����������


// ��������InitLog()
// ���ܣ���ʼ����־�ļ�
// ��������
// ����ֵ����
void CDB::InitLog()
{
	////////////////////////////
	// ��һ�����������ݿ���־�ļ�����
	m_log = new ofstream();					// ���ݿ���־�ļ��������·����logĿ¼������ʱ�����ᴴ����־
	m_log->open("log\\CDB.log", std::ofstream::out|std::ofstream::trunc);
	////////////////////////////
	// �ڶ�������֤�ļ������Ƿ�ɹ�
	if(!m_log->is_open())
		m_log=NULL;							// �����·����logĿ¼������ʱ�����ᴴ����־
}

// ��������GetConnectionParams()
// ���ܣ���ȡ���ݿ����Ӳ���
// �������ļ�·��
// ����ֵ����
// �쳣��runtime_error
string CDB::GetConnectionParams(string strFilePath)
{	
	///////////////////////////
	// ��һ���������ݿ������ļ�
//	wstring wsFilePath = StoWs(strFilePath);// stringת��Ϊwstring
	// �����ݿ������ļ�
	ifstream infile;
	infile.open(strFilePath.c_str(), ifstream::in);
	if(!infile.is_open())
	{
		if (m_log!=NULL)
			*m_log<<"���ݿ������ļ���ʧ��.�ļ������ڻ��ļ�����.\n"+strFilePath;
		
		throw runtime_error("���ݿ������ļ���ʧ��.�ļ������ڻ��ļ�����.\n"+strFilePath);
	}
	
	///////////////////////////
	// �ڶ������������ݿ������ļ�
	// ��ȡ�ļ�
	string strLine;
	string strConnection;
	while(!infile.eof())
	{
		// ��ȡһ�в���
		std::getline(infile,strLine);
		// ����һ�в���
		int nPosition = strLine.find('#');	// ��#�ſ�ʼ�Ĳ������ԣ������ж�Ҫ��#�Ų�������������������
		// �ж��Ƿ�����Ҫ��
		if (nPosition==string::npos && !strLine.empty())
											// ȡ������#��ʱ�ķǿյĵ�һ��Ϊ���ò���
		{
			strConnection = strLine;
			break;
		}

	}
	
	///////////////////////////
	// ���������������ݿ���������
	// ��֤�����������ȷ��
	if (strConnection.empty()) throw runtime_error("���ݿ������ļ�����Ϊ��.");
	// �ر��ļ�
	infile.close();
	// ����ֵ
	return strConnection;
}

////////////////////////////////////////////////////////////////
// ��������Init()	
// ���ܣ���ʼ��
// �����������ļ�·��
// ����ֵ����
// �쳣��runtime_error
void CDB::Init(string strFilePath)
{
	////////////////////////////
	// ��һ������ȡ���ݿ���������
	// ��������ַ���
	string sConnection = GetConnectionParams(strFilePath);
	// stringת��ΪCString
	CString strConnection(sConnection.c_str());
	
	////////////////////////////
	// �ڶ����������ݿ�����
	TRY
	{
		// �����ݿ�
		m_database.Open(NULL, FALSE, FALSE, strConnection);
	}
	CATCH_ALL(e)
	{
		// ���ݿ��ʼ��ʧ��
		if(m_log!=NULL)	{
			*m_log<<"���ݿ����ӳ�ʼ��ʧ��"<<endl;
			*m_log<<(((CDBException*)e)->m_strError+_T("\n"));
			*m_log<<strFilePath<<endl;
			m_log->flush();
		}
		throw runtime_error("���ݿ����Ӵ����쳣.�����ѶϿ������ݿ������ļ���������ȷ.");
	}
	END_CATCH_ALL;

	////////////////////////////
	// ��������������ݿ����Ӵ������
	if ( !m_database.IsOpen() )
		throw runtime_error("���ݿ����Ӵ�ʧ��.�����ѶϿ������ݿ������ļ���������ȷ.");

	if(m_log!=NULL) {
		*m_log<<"���ݿ����ӳ�ʼ���ɹ�.\n";
		m_log->flush();
	}

	#ifdef	 BATCH_INSERT
	// ��ʼһ������
	m_database.BeginTrans();
	// ��ʼ��
	m_nInsertCounter = 0;
	#endif
}


/////////////////////////////////////////////////////////////////
// ���ܣ��ر����ݿ�
void CDB::Close()
{
	#ifdef BATCH_INSERT
	// �ύ���һ������
	m_database.CommitTrans();
	#endif

	// �ر����ݿ�
	m_database.Close();
	// �ر���־�ļ�
	if(m_log!=NULL)
		m_log->close();
}

////////////////////////////////////////////////////////////////
// ���ƣ�Submit()
// ���ܣ��ύһ���������������䡣�ڶ���BATCH_INSERT�������Ϊ�����ύ
// ����1����ѯ���
// �쳣��runtime_error
void CDB::Submit(string ssql)
{
	// ��string�ַ���ת��ΪCString
	CString strSql(ssql.c_str());

	TRY
	{
		#ifdef BATCH_INSERT
		// ����
		m_nInsertCounter ++ ;
		#endif

		// ִ�в�ѯ
		m_database.ExecuteSQL(strSql);

		#ifdef BATCH_INSERT
		if(m_nInsertCounter >= BATCH_SIZE)
		{
			// �ύһ��
			m_database.CommitTrans();
			// �����µ���
			m_database.BeginTrans();
			m_nInsertCounter = 0;
		}
		#endif
	}
	// �����쳣
	CATCH_ALL(e)
	{

		// ��־��Ϣ��ʽѡ��				
		if(m_log!=NULL) {
			*m_log<<"���ݿ����ִ��ʧ��"<<endl;
			*m_log<<ssql<<std::endl;
			m_log->flush();
		}
		throw runtime_error("���ݿ�����������ִ��ʧ��.��ϸ��Ϣ��鿴���ݿ���־�ļ�log\\CDB.log.\n"+ssql);
	}
	END_CATCH_ALL;
}


// ���ƣ�Execute()
// ���ܣ�����ִ��һ����䣬�������κ����ݿ����
// ���룺����ַ���
// �������
// �쳣��runtime_error
void CDB::Execute(string ssql)
{
	/*
		1��ִ�в�ѯ��䣬��Ҫ���̵õ������
		2�������������������Ҫ�����ύ������
	*/
	CString strSql(ssql.c_str());

	TRY
	{
		// ִ�в�ѯ
		m_database.ExecuteSQL(strSql);

		// ������������������ύ����
		#ifdef BATCH_INSERT
		// �ύһ��
		m_database.CommitTrans();
		// �����µ���
		m_database.BeginTrans();
		#endif
	}
	CATCH_ALL(e)
	{
		// ��־��Ϣ��ʽѡ��			
		if(m_log!=NULL)	{
			*m_log<<"���ݿ����ִ��ʧ��"<<endl;
			*m_log<<ssql<<std::endl;
			m_log->flush();
		}
		throw runtime_error("���ݿ����ִ��ʧ��.��ϸ��Ϣ��鿴���ݿ���־�ļ�log\\CDB.log.\n"+ssql);
	}
	END_CATCH_ALL;
}