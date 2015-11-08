/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		Helper.h
//��  �ܣ�		���帨������
//�����ߣ�		��־ǿ
//��  �ڣ�		2014��04��12��
//��  �ȣ�		30��
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>								//mapͷ�ļ�
#include <set>								//setͷ�ļ�
#include <string>							//stringͷ�ļ�
#include <list>								//listͷ�ļ�
#include <vector>							//vectorͷ�ļ�
#include <algorithm>						//algorithm��ͷ�ļ�
#include <numeric>							//numeric��ͷ�ļ�
#include <sstream>							//sstream��ͷ�ļ�
#include <fstream>							//fstream��ͷ�ļ�
#include <iostream>							//iostream��ͷ�ļ�
#include <time.h>							//ʱ�亯����ͷ�ļ�
#include "math.h"							//��ѧ����ͷ�ļ�

//���������ռ�
using namespace std;						//ʹ�ñ�׼�����ռ�

string MapDoctorPosIDtoPosName(int);		//ӳ��ҽ��ְ��ID��ְ��
int MapDoctorPosNametoPosID(string&);		//ӳ��ҽ��ְ�Ƶ�ְ��ID
string MapCStringToString(CString&);		//��CStringӳ�䵽string

std::wstring StoWs(const std::string& s);	//��stringӳ�䵽wstring

int GetPositionIDFromName(string&);			//����ҽ��ְ�����ƻ�ȡ��ID

//ת���ļ�����
string GetMainFileName(string&);			//�����ļ����ƻ�ȡ���ļ���