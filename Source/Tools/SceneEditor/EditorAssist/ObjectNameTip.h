#pragma once
#include "Text.h"
using namespace Urho3D;

//物件上显示的名称Tip，用UI实现
class ObjectNameTip
{
public:
	ObjectNameTip();

	//更新显示位置
	void UpdateShow(int x,int y);
	void Hide();

	void SetText(const char* pName);
private:
	Text* text_;
};
