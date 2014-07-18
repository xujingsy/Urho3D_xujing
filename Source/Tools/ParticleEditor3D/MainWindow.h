#pragma once
#include "Object.h"
#include "Context.h"
using namespace Urho3D;

#include <QMainWindow>
#include <QMap>

//Main Application Window
class MainWindow : public QMainWindow ,public Object
{
	Q_OBJECT
	OBJECT(MainWindow)
public:
	MainWindow(Context* context);
	void CreateWidgets();

private:
	void CreateActions();
	void CreateMenuBar();
	void CreateToolBar();
	void CreateDockWidgets();
};
