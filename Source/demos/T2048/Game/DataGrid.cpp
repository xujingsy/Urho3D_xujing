#include "DataGrid.h"
#include <string.h>
#include <vector>

DataGrid::DataGrid()
{
	list<CHANGE_ORDER> outChange;
	OnNewGame(outChange);
}

DataGrid::~DataGrid()
{

}

void DataGrid::OnNewGame(list<CHANGE_ORDER>& outChange)
{
	memset(data,0,sizeof(data));
	IsChanged = false;
	Score = 0;
	GenNext(outChange);
}

bool DataGrid::GenNext(list<CHANGE_ORDER>& outChange)
{
	vector<Pos> vBlankCells;
	for(int x = 0;x < 4;x ++)
	{
		for(int y = 0;y < 4;y ++)
		{
			if(data[y][x] == 0)
			{
				Pos pos;
				pos.X = x;
				pos.Y = y;

				vBlankCells.push_back(pos);
			}
		}
	}

	//新产生的值，一定几率为4
	int newValue = 2;
	if(rand() % 100 < 20)
	{
		newValue = 4;
	}

	if(vBlankCells.size() > 0)
	{
		int index = rand() % vBlankCells.size();
		const Pos& pos = vBlankCells[index];

		data[pos.Y][pos.X] = newValue;

		//记录变化
		ChangeData change;
		change.cType = enChangeType_AddNew;
		change.Param1 = pos.Y;
		change.Param2 = pos.X;
		CHANGE_ORDER order;
		order.push_back(change);

		outChange.push_back(order);

		if(vBlankCells.size() == 1)
		{
			if(CheckEnd() == true)
			{
				//游戏结束
				return false;
			}
		}

		return true;
	}
	else
	{
		//没有空格了
		return false;
	}
}

bool DataGrid::OnLeft(list<CHANGE_ORDER>& outChange)
{
	IsChanged = false;

	for(int y = 0;y < 4;y ++)
	{
		MergeCol(y,0,3);
	}

	PushToHSide(0,3);

	if(IsChanged == true)
	{
		GenNext(outChange);
		IsChanged = false;
	}

	return true;
}

bool DataGrid::OnRight(list<CHANGE_ORDER>& outChange)
{
	IsChanged = false;

	for(int y = 0;y < 4;y ++)
	{
		MergeCol(y,3,0);
	}

	PushToHSide(3,0);

	if(IsChanged == true)
	{
		GenNext(outChange);
		IsChanged = false;
	}

	return true;
}

bool DataGrid::OnUp(list<CHANGE_ORDER>& outChange)
{
	IsChanged = false;

	for(int x = 0;x < 4;x ++)
	{
		MergeRow(x,0,3);
	}

	PushToVSide(0,3);

	if(IsChanged == true)
	{
		GenNext(outChange);
		IsChanged = false;
	}

	return true;
}

bool DataGrid::OnDown(list<CHANGE_ORDER>& outChange)
{
	IsChanged = false;

	for(int x = 0;x < 4;x ++)
	{
		MergeRow(x,3,0);
	}

	PushToVSide(3,0);

	if(IsChanged == true)
	{
		GenNext(outChange);
		IsChanged = false;
	}

	return true;
}

bool DataGrid::CheckEnd()
{
	for(int x = 0;x < 4;x ++)
	{
		for(int y = 0;y < 4;y ++)
		{
			if(x != 3)
			{
				//右边相等
				if(data[y][x] == data[y][x + 1])
				{
					return false;
				}
			}

			if(y != 3)
			{
				//下边相等
				if(data[y][x] == data[y + 1][x])
				{
					return false;
				}
			}
		}
	}

	//都没找到相邻相同的
	return true;
}

//将某一列合并
void DataGrid::MergeRow(int ColIndex,int fromRow,int toRow)
{
	//只记录当前行的值
	int lastRow = 0;
	int lastValue = 0;

	if(fromRow <= toRow)
	{
		for(int y = fromRow;y <= toRow;y ++)
		{
			int strCur = data[y][ColIndex];
			if( strCur == 0)
			{
				continue;
			}

			if(lastValue == 0)   //第一个元素
			{
				lastValue = strCur;
				lastRow = y; //当前行时只记录参数，在后面发现相同时回过来改
			}
			else    //之前有值,判断是否合并
			{
				if(strCur == lastValue)
				{
					//合并
					data[lastRow][ColIndex] = strCur + lastValue;
					data[y][ColIndex] = 0;
					Score += strCur + lastValue;

					IsChanged = true;
					//lastRow会在之后遇到新值时设置
					lastValue = 0;
				}
				else
				{
					lastValue = strCur;
					lastRow = y;
				}
			}
		}
	}
	else
	{
		for(int y = fromRow;y >= toRow;y --)
		{
			int strCur = data[y][ColIndex];
			if( strCur == 0)
			{
				continue;
			}

			if(lastValue == 0)   //第一个元素
			{
				lastValue = strCur;
				lastRow = y; //当前行时只记录参数，在后面发现相同时回过来改
			}
			else    //之前有值,判断是否合并
			{
				if(strCur == lastValue)
				{
					//合并
					data[lastRow][ColIndex] = strCur + lastValue;
					data[y][ColIndex] = 0;
					Score += strCur + lastValue;

					IsChanged = true;
					//lastRow会在之后遇到新值时设置
					lastValue = 0;
				}
				else
				{
					lastValue = strCur;
					lastRow = y;
				}
			}
		}
	}
}

void DataGrid::MergeCol(int RowIndex,int fromCol,int toCol)
{
	int lastCol = 0;
	int lastValue = 0;

	if(fromCol <= toCol)
	{
		for(int x = fromCol;x <= toCol;x ++)
		{
			int strCur = data[RowIndex][x];
			if(strCur == 0)
			{
				continue;
			}

			if(lastValue == 0)
			{
				lastValue = strCur;
				lastCol = x;
			}
			else
			{
				if(strCur == lastValue)
				{
					data[RowIndex][lastCol] = strCur + lastValue;
					data[RowIndex][x] = 0;

					Score += strCur + lastValue;
					IsChanged = true;
					lastValue = 0;
				}
				else
				{
					lastValue = strCur;
					lastCol = x;
				}

			}
		}
	}
	else
	{
		for(int x = fromCol;x >= toCol;x --)
		{
			int strCur = data[RowIndex][x];
			if(strCur == 0)
			{
				continue;
			}

			if(lastValue == 0)
			{
				lastValue = strCur;
				lastCol = x;
			}
			else
			{
				if(strCur == lastValue)
				{
					data[RowIndex][lastCol] = strCur + lastValue;
					data[RowIndex][x] = 0;

					Score += strCur + lastValue;
					IsChanged = true;
					lastValue = 0;
				}
				else
				{
					lastValue = strCur;
					lastCol = x;
				}

			}
		}
	}
}

//垂直方向
void DataGrid::PushToVSide(int yFrom,int yTo)
{
	for(int x = 0;x < 4;x ++)
	{
		//当前堆到第几行
		int nLine = yFrom;

		if(yFrom <= yTo)
		{
			for(int y = yFrom;y <= yTo;y ++)
			{
				if(data[y][x] != 0)
				{
					if(nLine != y)
					{
						//将格子挪过去
						data[nLine][x] = data[y][x];
						data[y][x] = 0;

						IsChanged = true;
					}

					nLine ++;
				}
			}
		}
		else
		{
			for(int y = yFrom;y >= yTo;y --)
			{
				if(data[y][x] != 0)
				{
					if(nLine != y)
					{
						//将格子挪过去
						data[nLine][x] = data[y][x];
						data[y][x] = 0;

						IsChanged = true;
					}

					nLine --;
				}
			}
		}
	}
}

//水平方向
void DataGrid::PushToHSide(int xFrom,int xTo)
{
	for(int y = 0;y < 4;y ++)
	{
		int nLine = xFrom;

		if(xFrom <= xTo)
		{
			for(int x = xFrom;x <= xTo;x ++)
			{
				if(data[y][x] != 0)
				{
					if(nLine != x)
					{
						data[y][nLine] = data[y][x];
						data[y][x] = 0;

						IsChanged = true;
					}

					nLine ++;
				}
			}
		}
		else
		{
			for(int x = xFrom;x >= xTo;x --)
			{
				if(data[y][x] != 0)
				{
					if(nLine != x)
					{
						data[y][nLine] = data[y][x];
						data[y][x] = 0;

						IsChanged = true;
					}

					nLine --;
				}
			}
		}
	}
}
