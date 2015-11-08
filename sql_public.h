/////////////////////////////////////////////////////////////////////
// �ļ�����sql_public.h
// ���ܣ����ݿ��ѯ������ͷ�ļ�����������ֵ�ṹ�嶨��
// �����ߣ���ҫ��
// ʱ�䣺2014��7��9��
// ���ȣ�26��
/////////////////////////////////////////////////////////////////////

#ifndef SQL_PUBLIC
#define SQL_PUBLIC

#include "tinyxml2.h"		// tinyxmlͷ�ļ�
#include "database.h"		// ���ݿ���ʶ���ͷ�ļ�
#include <vector>			// STL vectorͷ�ļ�
#include <map>				// STL mapͷ�ļ�
#include <string>			// STL stringͷ�ļ�
#include "Helper.h"			// ����ʦ�ṩ�ĸ�������ͷ�ļ�

using namespace std;		// STL �����ռ�
using namespace tinyxml2;	// TinyXML2 �����ռ�

// ��������
typedef vector<vector<pair<string,string>>> RECORD_VECTOR;
typedef std::map<string,RECORD_VECTOR> RESULT_MAP;

#endif