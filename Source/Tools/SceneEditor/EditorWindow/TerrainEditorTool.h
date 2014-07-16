#pragma once
#include <qobject.h>
#include <qpushbutton.h>
#include "../Controls/ButtonsPanel.h"

//地形编辑面板 应该从BaseEditor继承？
class TerrainEditorTool : public ButtonsPanel
{
	Q_OBJECT
public:
	TerrainEditorTool(QWidget* parent = 0);

};