#pragma once
#include "MainWindow.h"

#include "Object.h"
#include "Engine.h"
#include "Scene.h"
#include "Node.h"
using namespace Urho3D;

#include <QApplication>
#include <QObject>

class EditorApplication : public QApplication , public Object
{
	Q_OBJECT
	OBJECT(EditorApplication)
public:
	EditorApplication(int argc, char** argv, Context* context);
	~EditorApplication();

	void SetStyleSheet(const char* qssPath);

	int Run();
private slots:
	void OnTimer();

private:
	void CreateScene();

	void HandleUpdate(StringHash eventType, VariantMap& eventData);
	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleMouseWheel(StringHash eventType, VariantMap& eventData);
	void HandleRenderUpdate(StringHash eventType, VariantMap& eventData);

	MainWindow* mainWindow_;
	Engine* engine_;
	Scene* scene_;
	Node* particleNode_;
};
