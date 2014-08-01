#pragma once
#include <QWidget>
#include <QEvent>
#include <Windows.h>

//引擎渲染窗口
class EditorSceneWidget : public QWidget
{
   Q_OBJECT

public:
   EditorSceneWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

   void showObjectMenu();
private:
	bool m_Captured;
	POINT m_savedpos;	//CatureMouse时保存的鼠标位置
	int m_oldx;	//MouseMove时旧的鼠标坐标
	int m_oldy;

	bool MouseMovedSincePress;	//鼠标按下后是否移动过
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent *event);

	void mouseMoveEvent(QMouseEvent *evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void leaveEvent(QEvent *evt);
	void wheelEvent(QWheelEvent *evt);

	void keyPressEvent(QKeyEvent *evt);

	//Qt的鼠标到Urho的转换
	unsigned int getMouseButton(Qt::MouseButtons buttons,Qt::MouseButton button);
};
