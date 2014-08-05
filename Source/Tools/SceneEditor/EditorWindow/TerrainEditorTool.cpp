#include "stdafx.h"
#include "TerrainEditorTool.h"
#include "../ComponentEditor/ObjectEditorWidgets/TerrainPanel.h"
#include "../ComponentEditor/ObjectEditorWidgets/TerrainTexturePanel.h"
#include "../ComponentEditor/ObjectEditorWidgets/TerrainGrassPanel.h"

TerrainEditorTool::TerrainEditorTool(QWidget* parent)
{
	this->AddButton("TerrainHeight",new TerrainPanel());
	this->AddButton("PaintTexture",new TerrainTexturePanel());
	this->AddButton("PaintGrass",new TerrainGrassPanel());

	this->OnInitDialog();
}
