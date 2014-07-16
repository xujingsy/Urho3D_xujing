#pragma once
#include "BorderImage.h"
using namespace Urho3D;

class RectSelectionFrame
{
public:
	RectSelectionFrame();

	void UpdateShow(int x,int y);
	void Hide();

	void Begin(int x,int y);
	bool isBegin;
	Vector2 vecStart;
	Vector2 vecEnd;
private:
	BorderImage* border_;
};
