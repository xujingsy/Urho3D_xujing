#pragma once
#include <list>
#include <string>
using namespace std;
#include "OperationData.h"
#include "Misc/Single.h"

class UndoManager : public Singleton<UndoManager>
{
public:
	void AddHistory(const OperationData& data)
	{
		mHistory.push_back(data);
	}

	bool TryPopHistory(OperationData& data)
	{
		if(mHistory.empty())
		{
			return false;
		}
		else
		{
			data = mHistory.front();
			mHistory.pop_front();

			return true;
		}
	}

	void Undo();

	void Redo();

	bool CanUndo()
	{
		return mHistory.size() > 0;
	}

	bool CanRedo()
	{
		return mRedoHistory.size() > 0;
	}

	string GetUndoString();
	string GetRedoString();

	void Clear()
	{
		mHistory.clear();
		mRedoHistory.clear();
	}
private:
	list<OperationData> mHistory;
	list<OperationData> mRedoHistory;
};
