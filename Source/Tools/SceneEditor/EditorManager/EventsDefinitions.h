/*
说明：
	所有的编辑器都从这些类继承
	当主编辑器相应事件发生时，会主动通知所有注册了该事件的编辑器
*/
#pragma once

class MouseListener
{
public:
	MouseListener() {};
	virtual ~MouseListener() {};

	virtual void OnMouseMove(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseLeave(float x,float y,unsigned int buttons)
	{

	}

	//鼠标按下时只会触发一次
	virtual void OnMouseLeftDown(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseLeftUp(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseRightDown(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseRightUp(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseMiddleDown(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseMiddleUp(float x,float y,unsigned int buttons)
	{

	}

	virtual void OnMouseWheel(float x,float y,unsigned int buttons)
	{

	}
};

class KeyboardListener
{
public:
	KeyboardListener() {};
	virtual ~KeyboardListener() {};

	virtual bool OnKeyDown(unsigned short key)
	{

		return true;
	}

	virtual bool OnKeyUp(unsigned short key)
	{
		return true;
	}
};

class DragDropHandler
{
public:
	DragDropHandler() {};
	virtual ~DragDropHandler() {};

	virtual bool OnDragEnter () = 0;
	virtual void OnDragLeave () = 0;
	virtual bool OnDragMove (float x,float y,unsigned int buttons) = 0;
	virtual void OnDragWheel (float x,float y,unsigned int buttons, float delta) = 0;
	virtual void OnDragDropped (float x,float y,unsigned int buttons) = 0;
};
