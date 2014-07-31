#include "stdafx.h"
#include "TerrainTexturePanel.h"

TerrainTexturePanel::TerrainTexturePanel(QWidget* parent)
{

}

void TerrainTexturePanel::BeginEditParams(Node* pNode)
{
	EditorRoot::Instance()->ActiveTool = TOOL_PAINT;
}

void TerrainTexturePanel::EndEditParams()
{
	EditorRoot::Instance()->ActiveTool = TOOL_SELECT;
}
