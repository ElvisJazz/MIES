//////////////////////////////////////////////////////////////////////////////////////////////////
// ���ƣ�database.h
// ���ܣ�ʵ��CDB��
// �����ߣ���ҫ��
// ʱ�䣺20101018
// ���ȣ�50��
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef	ACCESS_DATABASE
#define	ACCESS_DATABASE

// ͷ�ļ�
#include <afxdb.h>									// MFC���ݿ�ͷ�ļ�
#include <odbcinst.h>								// ODBCͷ�ļ�
#include <fstream>									// fstreamͷ�ļ�
#include <string>									// stringͷ�ļ�

using namespace std;

/*
�����
	ͨ�������꿪���͹ر������ύ���ܣ�
	ͨ��MY_DEBUG�꿪�����Թ��ܣ����Թ��̲��ύ���ݡ�
*/
//#define BATCH_SIZE		200000					// 20��
//#define BATCH_INSERT
//#define MY_DEBUG

// �������Ͷ���
class CDB
{
public:
	// ��Ա����
	CDB(){InitLog();};								// ���캯��
	~CDB(){Close();};								// ��������
	void InitLog();									// ��ʼ����־�ļ�
	string GetConnectionParams(string);				// ��ȡ���Ӳ���
	void Init(string);								// ��ʼ������
	void Close();									// �ر����ݿ�
	void Submit(string);							// �����ݿ��ύһ����䣬����һ������ִ��			// ע�⣺���������Ըĳ�����
	void Execute(string);							// ����ִ��һ�����							// ���£�2011-12-03
	
	// ��Ա����
	CDatabase		m_database;						// ���ݿ����
	unsigned int	m_nInsertCounter;				// ���������
	ofstream	*	m_log;							// ������־
};

#endif
