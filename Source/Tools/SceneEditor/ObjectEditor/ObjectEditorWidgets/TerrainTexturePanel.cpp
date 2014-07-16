#include "stdafx.h"
#include "TerrainTexturePanel.h"

TerrainTexturePanel::TerrainTexturePanel(QWidget* parent)
{

}

void TerrainTexturePanel::BeginEditParams(Node* pNode)
{
	EditorsRoot::Instance()->ActiveTool = TOOL_PAINT;
}

void TerrainTexturePanel::EndEditParams()
{
	EditorsRoot::Instance()->ActiveTool = TOOL_SELECT;
}
