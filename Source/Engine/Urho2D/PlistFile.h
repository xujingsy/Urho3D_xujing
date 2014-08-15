#pragma once
#include <map>
#include <string>
#include <list>
#include <vector>
using namespace std;

void SplitString(const char* cs_src,int srcLen, const char* cs_splitMark,vector<string>& vStringResult);
void Replace(std::string& s,std::string s1,std::string s2);

struct SizeElement
{
	int X;
	int Y;
};

struct PlistElement 
{
	PlistElement()
	{
		parent = NULL;
	}

	~PlistElement()
	{
		for each(PlistElement* ele in dict)
		{
			delete ele;
		}
	}

	PlistElement* GetChild(const char* name)
	{
		string strKey = name;
		for each(PlistElement* ele in dict)
		{
			if(ele->key == strKey)
			{
				return ele;
			}
		}

		return NULL;
	}

	vector<SizeElement> ValueToSizeArray()
	{
		vector<SizeElement> result;

		if(strValue == "")
			return result;

		for(int i = 0;i < strValue.size();i ++)
		{
			if(strValue[i] == '{' || strValue[i] == '}' || strValue[i] == ',')
				strValue[i] = ' ';
		}

		vector<string> vStrings;
		SplitString(strValue.c_str(), strValue.length() + 1, " ", vStrings);

		for (int i = 0;i < vStrings.size();i ++)
		{
			int nValue = atoi(vStrings[i].c_str());
			if(i % 2 == 0)
			{
				SizeElement ele;
				ele.X = nValue;
				ele.Y = 0;
				result.push_back(ele);
			}
			else
			{
				result[result.size() - 1].Y = nValue;
			}
		}

		return result;
	}

	string key;
	//value or childnodes
	string strValue;
	list<PlistElement*> dict;

	PlistElement* parent;
};

class PlistFile
{
public:

	bool Init(const unsigned char* szContent,int nLen);

	PlistElement data;
};
