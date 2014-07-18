#include "stdafx.h"
#include "EditorApplication.h"

EditorApplication::EditorApplication(int argc, char** argv, Context* context) :
	QApplication(argc, argv),
	Object(context)
{
	engine_ = new Engine(context);
	scene_ = new Scene(context);

	SubscribeToEvent(E_UPDATE, HANDLER(EditorApplication, HandleUpdate));
}

EditorApplication::~EditorApplication()
{

}

//init engine --> run
int EditorApplication::Run()
{
	VariantMap engineParams;
	engineParams["FrameLimiter"] = false;
	engineParams["LogName"] = "ParticleEditor3D.log";
	//engineParams["ExternalWindow"] = ;
	//engineParams[] = ;

	return QApplication::exec();
}

void EditorApplication::OnTimer()
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
