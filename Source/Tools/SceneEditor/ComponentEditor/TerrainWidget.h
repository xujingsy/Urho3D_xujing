#pragma once
#include "BaseComponentEditor.h"
#include "../ComponentTool/TerrainEditorTool.h"

//Terrain±à¼­Ãæ°å
class TerrainWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	TerrainWidget();
	~TerrainWidget();

	void Init(Node* pNode);

private:
	TerrainEditorTool* terrainTool;
};
