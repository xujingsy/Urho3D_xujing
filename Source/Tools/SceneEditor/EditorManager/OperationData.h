#pragma once
#include "XMLFile.h"

enum enOpType
{
	enOpType_Add,
	enOpType_Delete,
	enOpType_Edit,
};

struct NodeData 
{
	int ParentNodeID;
	int NodeID;
	XMLFile* xmlData;

	NodeData()
	{
		ParentNodeID = 0;
		NodeID = 0;
		xmlData = NULL;
	}

	~NodeData()
	{
		if(xmlData)
		{
			//delete xmlData; todo:释放问题
			xmlData = NULL;
		}
	}
};

//记录编辑器中的编辑操作
class OperationData
{
public:
	OperationData()
	{
	}

	enOpType opType;

	//如果有其它操作可能是其它数据
	list<NodeData> nodesData;
};
