/*
所有编辑器的基类
*/
#pragma once
#include <string>
#include "../EditorManager/EventsDefinitions.h"

//为了简化设计，鼠标键盘消息都会发送到当前Editor，如果该编辑器不处理该消息则不重载该函数即可
class BaseEditor : public MouseListener , public KeyboardListener
{
public:
	//每一帧触发(如果注册了的话)
	virtual bool update(float timePassed)
	{
		return false;
	}

	virtual void OnObjectSelect(Node* pNode)
	{

	}
};
