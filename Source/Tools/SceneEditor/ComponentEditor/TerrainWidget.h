#pragma once
#include "BaseComponentEditor.h"
#include "../ComponentTool/TerrainEditorTool.h"

//Terrain�༭���
class TerrainWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	TerrainWidget();
	~TerrainWidget();

	void InitComponent(Component* pComponent);

private:
	TerrainEditorTool* terrainTool;
};
