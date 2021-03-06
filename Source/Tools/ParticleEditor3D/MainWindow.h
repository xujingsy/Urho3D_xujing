#pragma once
#include "Object.h"
#include "Context.h"
using namespace Urho3D;
#include "EmitterAttributeEditor.h"
#include "ParticleAttributeEditor.h"
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

	void UpdateWidgets();
private slots:
	void HandleNewAction();
	void HandleOpenAction();
	void HandleSaveAction();
	void HandleZoomAction();
	void HandleBackgroundAction();
private:
	void CreateActions();
	void CreateMenuBar();
	void CreateToolBar();
	void CreateDockWidgets();
private:
	QAction* newAction_;
	QAction* openAction_;
	QAction* saveAction_;
	QAction* exitAction_;

	QAction* zoomInAction_;
	QAction* zoomOutAction_;
	QAction* zoomResetAction_;

	QAction* backgroundAction_;

	QMenu* fileMenu_;
	QMenu* viewMenu_;

	QToolBar* toolBar_;

	EmitterAttributeEditor* emitterAttributeEditor_;
	ParticleAttributeEditor* particleAttributeEditor_;
};
