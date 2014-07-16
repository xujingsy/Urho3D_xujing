#pragma once
#include "Node.h"
#include <list>
using namespace std;
using namespace Urho3D;

//内部数据变动应该产生变化列表发出来，外部根据

enum enChangeType
{
	enChangeType_MoveTo,	//一个元素移动到另一个

	enChangeType_Dispear,	//消失
	enChangeType_AddNew,	//产生新的
};

struct ChangeData
{
	ChangeData()
	{
		memset(this,0,sizeof(*this));
	}

	enChangeType cType;
	Node* pNode;
	int Frames;	//多少帧完成

	//Move 表示从当前移动到x,y
	int Param1;
	int Param2;
};

struct Pos
{
	int X;
	int Y;
};

//变化的动作序列,队列中的效果是按顺序执行的
typedef list<ChangeData> CHANGE_ORDER;

#define out

class DataGrid
{
public:
	DataGrid();
	~DataGrid();

	//产生下一个数，如果产生失败，游戏结束
	bool GenNext(list<CHANGE_ORDER>& outChange);

	//向上向下操作,返回是否发生变化
	bool OnLeft(list<CHANGE_ORDER>& outChange);
	bool OnRight(list<CHANGE_ORDER>& outChange);
	bool OnUp(list<CHANGE_ORDER>& outChange);
	bool OnDown(list<CHANGE_ORDER>& outChange);

	void OnNewGame(list<CHANGE_ORDER>& outChange);

	bool CheckEnd();

	//初值为0
	int Score;	//当前分数
	int data[4][4];	//y,x
private:
	bool IsChanged;
	void MergeRow(int ColIndex,int fromRow,int toRow);
	void MergeCol(int RowIndex,int fromCol,int toCol);

	void PushToVSide(int yFrom,int yTo);
	void PushToHSide(int xFrom,int xTo);
};
