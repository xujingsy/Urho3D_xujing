#include "stdafx.h"
#include "TerrainEditorTool.h"
#include "../ObjectEditor/ObjectEditorWidgets/TerrainPanel.h"
#include "../ObjectEditor/ObjectEditorWidgets/TerrainTexturePanel.h"
#include "../ObjectEditor/ObjectEditorWidgets/TerrainGrassPanel.h"

TerrainEditorTool::TerrainEditorTool(QWidget* parent)
{
	this->AddButton("TerrainHeight",new TerrainPanel());
	this->AddButton("PaintTexture",new TerrainTexturePanel());
	this->AddButton("PaintGrass",new TerrainGrassPanel());

	this->OnInitDialog();
}
