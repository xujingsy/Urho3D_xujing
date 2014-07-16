#include "stdafx.h"
#include "EditorGlobalInfo.h"

EditorGlobalInfo* gEditorGlobalInfo;

EditorGlobalInfo::EditorGlobalInfo(Context* context) : Object(context)
{
	mFillMode = FillMode::FILL_SOLID;
}

EditorGlobalInfo::~EditorGlobalInfo()
{

}

void EditorGlobalInfo::SetFillMode(FillMode mode)
{
	Renderer* renderer = GetSubsystem<Renderer>();

	renderer->GetViewport(0)->GetCamera()->SetFillMode(mode);
}
