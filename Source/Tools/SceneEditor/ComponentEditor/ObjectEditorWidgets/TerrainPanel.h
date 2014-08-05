#pragma once
#include <qpushbutton.h>
#include "../../Controls/EditTool.h"
#include <qlabel.h>
#include <qslider.h>
#include <qboxlayout.h>
#include <qlistwidget.h>

class TerrainPanel : public EditTool
{
	Q_OBJECT
public:
	TerrainPanel(QWidget* parent = 0);

	virtual void BeginEditParams(Node* pNode);
	virtual void EndEditParams();

private Q_SLOTS:
	void brushSizeValueChanged ( int value );
	void brushIntensityValueChanged ( int value );
	void brushIndexChanged();
private:
	QBoxLayout* layout;
	QBoxLayout* layoutBrush;
	QListWidget* brushWidget;

	void add_brush(const char* pBrushTex);
	QSlider* sldBrushSize;
	QLabel* lblBrushSize;

	QSlider* sldIntensity;
	QLabel* lblIntensityValue;
};