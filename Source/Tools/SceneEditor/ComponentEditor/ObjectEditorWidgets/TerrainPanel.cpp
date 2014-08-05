#include "stdafx.h"
#include "TerrainPanel.h"

TerrainPanel::TerrainPanel(QWidget* parent)
{
	layout = new QBoxLayout(QBoxLayout::TopToBottom);
	this->setLayout(layout);

	//笔刷样式
	vector<string> vBrushImages;
	vBrushImages.push_back(":/Images/Brushes/Circular.png");
	vBrushImages.push_back(":/Images/Brushes/flat_circular.png");
	vBrushImages.push_back(":/Images/Brushes/noisy_circular1.png");
	vBrushImages.push_back(":/Images/Brushes/noisy_circular2.png");
	vBrushImages.push_back(":/Images/Brushes/sharp_circular.png");
	vBrushImages.push_back(":/Images/Brushes/Square.png");

	brushWidget = new QListWidget();
	brushWidget->setViewMode(QListView::IconMode);
	brushWidget->setGridSize(QSize(52, 52));
	brushWidget->setIconSize(QSize(48, 48));
	brushWidget->setFlow(QListView::LeftToRight);
	brushWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
	brushWidget->setFixedHeight(100);
	for(int i = 0;i < vBrushImages.size();i ++)
	{
		const string& strPath = vBrushImages[i];
		QImage pImage(strPath.c_str());
		pImage.invertPixels(QImage::InvertRgb);
		QPixmap pmap = QPixmap::fromImage(pImage);

		//string strName = 
		QListWidgetItem* witem = new QListWidgetItem(QIcon(pmap),strPath.c_str());
		witem->setWhatsThis(QString(strPath.c_str()));
		witem->setToolTip(QString(strPath.c_str()));
		brushWidget->addItem(witem);
	}

	brushWidget->setCurrentItem(brushWidget->item(0));
	connect(brushWidget,SIGNAL( itemSelectionChanged() ), this, SLOT( brushIndexChanged()));

	//QWidget* wBrushLine = new QWidget();
	//layoutBrush = new QBoxLayout(QBoxLayout::LeftToRight);
	//wBrushLine->setLayout(layoutBrush);

	layout->addWidget(brushWidget);

	//1
	QLabel* lblHeader = new QLabel("Settings");
	lblHeader->setFont(QFont("Times",13,QFont::Bold));
	layout->addWidget(lblHeader);

	//2 BrushSize行
	QWidget* wLineBrushSize = new QWidget();
	QBoxLayout* lineBrushSize = new QBoxLayout(QBoxLayout::LeftToRight);
	wLineBrushSize->setLayout(lineBrushSize);
	
	QLabel* lblBrushSizeHeader = new QLabel("Brush Size");
	lineBrushSize->addWidget(lblBrushSizeHeader);
	
	sldBrushSize = new QSlider(Qt::Orientation::Horizontal);
	sldBrushSize->setRange(1,100);
	sldBrushSize->setTickInterval(1);
	sldBrushSize->setTickPosition(QSlider::TicksBelow);
	lineBrushSize->addWidget(sldBrushSize);
	
	lblBrushSize = new QLabel();
	lblBrushSize->setFixedSize(60,20);
	lineBrushSize->addWidget(lblBrushSize);
	brushSizeValueChanged(0);
	layout->addWidget(wLineBrushSize);

	//3 强度行
	QWidget* wLineIntensity = new QWidget();
	QBoxLayout* lineIntensity = new QBoxLayout(QBoxLayout::LeftToRight);
	wLineIntensity->setLayout(lineIntensity);
	QLabel* lblIntensityHeader = new QLabel("Intensity");
	lineIntensity->addWidget(lblIntensityHeader);

	sldIntensity = new QSlider(Qt::Orientation::Horizontal);
	sldIntensity->setRange(1,100);
	sldIntensity->setTickInterval(1);
	sldIntensity->setTickPosition(QSlider::TicksBelow);
	lineIntensity->addWidget(sldIntensity);

	lblIntensityValue = new QLabel();
	lblIntensityValue->setFixedSize(60,20);
	lineIntensity->addWidget(lblIntensityValue);
	brushIntensityValueChanged(0);
	layout->addWidget(wLineIntensity);

	//
	connect(sldBrushSize,SIGNAL(valueChanged(int)),this,SLOT(brushSizeValueChanged(int)));
	connect(sldIntensity,SIGNAL(valueChanged(int)),this,SLOT(brushIntensityValueChanged(int)));
}

void TerrainPanel::brushIndexChanged()
{
	if(!brushWidget->currentItem())
		return;

	QString str = brushWidget->currentItem()->whatsThis();

	TerrainBrush* brush = EditorRoot::Instance()->GetTerrainEditor()->GetBrush();
	if(brush != NULL)
	{
		ResourceCache* cache = EditorRoot::Instance()->engine_->GetContext()->GetSubsystem<ResourceCache>();
		brush->SetBrushTexture(cache->GetResource<Texture2D>(str.toStdString().c_str()));
	}
}

void TerrainPanel::add_brush(const char* pBrushTex)
{
	QLabel* b1 = new QLabel();
	b1->setPixmap(QPixmap(pBrushTex));
	layoutBrush->addWidget(b1);
}

void TerrainPanel::BeginEditParams(Node* pNode)
{
	TerrainBrush* brush = EditorRoot::Instance()->GetTerrainEditor()->GetBrush();
	if(brush != NULL)
	{
		sldBrushSize->setSliderPosition(brush->GetBrushSize());
	}

	EditorRoot::Instance()->ActiveTool = TOOL_DEFORM;
}

void TerrainPanel::EndEditParams()
{
	EditorRoot::Instance()->ActiveTool = TOOL_SELECT;
}

void TerrainPanel::brushSizeValueChanged ( int value )
{
	lblBrushSize->setText(QString::number(sldBrushSize->sliderPosition(),10));

	TerrainEditor* pEditor = EditorRoot::Instance()->GetTerrainEditor();
	if(pEditor != NULL)
	{
		TerrainBrush* brush = pEditor->GetBrush();
		if(brush)
		{
			brush->SetBrushSize(sldBrushSize->sliderPosition());
		}
	}
}

void TerrainPanel::brushIntensityValueChanged ( int value )
{
	lblIntensityValue->setText(QString::number(sldIntensity->sliderPosition(),10));
}