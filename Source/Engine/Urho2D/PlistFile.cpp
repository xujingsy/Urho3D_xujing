#include "Precompiled.h"
#include "PlistFile.h"

//helper function
void SplitString(const char* cs_src,int srcLen, const char* cs_splitMark,vector<string>& vStringResult)
{
	vStringResult.clear();

	char* str_tmp = new char[srcLen + 1];
	strcpy(str_tmp, cs_src);
	str_tmp[srcLen] = 0;

	char* token = strtok(str_tmp,cs_splitMark);

	while(token != NULL)
	{
		vStringResult.push_back(string(token));
		token = strtok(NULL,cs_splitMark);
	}

	delete[] str_tmp;
}

// replace first s1 with s2 in string s
void Replace(string& s,string s1,string s2)
{
	int pos = s.find(s1);

	if (pos > 0)
	{
		s.replace(pos, s1.size(), s2);
	}
}

void Trim(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) 
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos)
			str.erase(0, pos);
	}
	else
	{
		str.erase(str.begin(), str.end());
	}
}

bool IsStringExist(const string& text, const string& value)
{
	size_t res = text.find(value);
	return res != wstring::npos;
}

bool IsExistValue(const string& text)
{
	return IsStringExist(text, "<integer>") || IsStringExist(text, "<string>") || 
		IsStringExist(text, "<real>") || IsStringExist(text, "<integer>") ||
		IsStringExist(text, "<true/>") || IsStringExist(text, "<false/>");
}

string GetLineValue(const string& line)
{
	string strLine = line;
	Trim(strLine);

	int leftPos = line.find_first_of(">");
	int rightPos = line.find_last_of("<");

	return strLine.substr(leftPos + 1, rightPos - leftPos - 1);
}

string GetLineKey(const string& line)
{
	string strLine = line;
	Trim(strLine);

	int leftPos = strLine.find_first_of("<");
	int rightPos = strLine.find_first_of(">");

	return strLine.substr(leftPos + 1, rightPos - leftPos - 1);
}

//preprocess each line string
vector<string> PreProcessStrings(vector<string>& strLines)
{
	vector<string> vResult;

	for(int i = 0;i < strLines.size();i ++)
	{
		string& strLine = strLines[i];
		Trim(strLine);
		//only process blank string,todo:remark
		if(strLine != "")
		{
			vResult.push_back(strLine);
		}
	}

	return vResult;
}

bool PlistFile::Init(const unsigned char* szContent,int nLen)
{
	//new line flag format
	string strFlag = "\n";
	if(IsStringExist(string((const char*)szContent), "\r\n"))
		strFlag = "\r\n";

	//split to vector<string> then preprocess
	vector<string> vLines;
	SplitString((const char*)szContent,nLen, strFlag.c_str(), vLines);
	vLines = PreProcessStrings(vLines);

	// build plist value tree
	PlistElement* pCurParent = & data;
	PlistElement* pCurValue = pCurParent;

	//first 3 rows,last 1 row passed
	for(int i = 3;i < vLines.size() - 1;i ++)
	{
		const string& strLine = vLines[i];

		if(IsStringExist( strLine, "<key>"))	//key-begin-end
		{
			//create sibling node
			PlistElement* newChild = new PlistElement();
			newChild->key = GetLineValue(strLine);
			newChild->parent = pCurParent;
			
			pCurParent->dict.push_back(newChild);

			pCurValue = newChild;	//current node is current use node
		}
		else if(IsExistValue( strLine ))	// value begin
		{
			if(IsStringExist( strLine, "<true/>"))
			{
				pCurValue->strValue = "true";
			}
			else if(IsStringExist( strLine, "<false/>"))
			{
				pCurValue->strValue = "false";
			}
			else
			{
				pCurValue->strValue = GetLineValue(strLine);
			}
		}
		else if(IsStringExist(strLine, "<dict>"))	//dict begin
		{
			pCurParent = pCurValue;	//goto child node,current node is parentnode
		}
		else if(IsStringExist( strLine, "</dict>"))	//value end
		{
			pCurValue = pCurParent;
			pCurParent = pCurValue->parent;
		}
		else
		{
			continue;
			//other error?
		}
	}

	return true;
}
