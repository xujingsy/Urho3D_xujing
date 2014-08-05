#include "stdafx.h"
#include "TerrainWidget.h"

TerrainWidget::TerrainWidget()
{
	terrainTool = new TerrainEditorTool(this);

	frameLayout_->addWidget(terrainTool);
}

TerrainWidget::~TerrainWidget()
{

}

void TerrainWidget::InitComponent(Component* pComponent)
{

}
