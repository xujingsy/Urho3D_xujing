#pragma once
#include "Object.h"
#include "GraphicsDefs.h"
using namespace Urho3D;

class EditorGlobalInfo : public Object
{
	OBJECT(EditorGlobalInfo)
public:
	
	EditorGlobalInfo(Context* context);
	~EditorGlobalInfo();

	void SetFillMode(FillMode mode);
private:
	FillMode mFillMode;
};

extern EditorGlobalInfo* gEditorGlobalInfo;
