#include "stdafx.h"
#include "EditorApplication.h"
#include <QTimer>
#include <QFile>

EditorApplication::EditorApplication(int argc, char** argv, Context* context) :
	QApplication(argc, argv),
	Object(context)
{
	engine_ = new Engine(context);
	scene_ = new Scene(context);

	SetStyleSheet(":/Images/stylesheets/dark.qss");
	mainWindow_ = new MainWindow(context);

	SubscribeToEvent(E_UPDATE, HANDLER(EditorApplication, HandleUpdate));
}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::SetStyleSheet(const char* qssPath)
{
	QFile file(qssPath);
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
	file.close();
}

//init engine --> run
int EditorApplication::Run()
{
	VariantMap engineParams;
	engineParams["FrameLimiter"] = false;
	engineParams["LogName"] = "ParticleEditor3D.log";
	engineParams["ExternalWindow"] = (void*)(mainWindow_->centralWidget()->winId());
	
	if( !engine_->Initialize(engineParams))
		return -1;

	CreateScene();
	mainWindow_->CreateWidgets();

	QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
	timer.start(16);

	return QApplication::exec();
}

void EditorApplication::OnTimer()
{

}

void EditorApplication::CreateScene()
{

}

void EditorApplication::HandleUpdate(StringHash eventType, VariantMap& eventData)
{

}

void EditorApplication::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{

}

void EditorApplication::HandleMouseWheel(StringHash eventType, VariantMap& eventData)
{

}

void EditorApplication::HandleRenderUpdate(StringHash eventType, VariantMap& eventData)
{

}
