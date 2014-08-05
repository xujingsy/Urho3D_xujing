#include "stdafx.h"
#include "MaterialColorFrameEditorDialog.h"

MaterialColorFrameEditorDialog::MaterialColorFrameEditorDialog()
{
	QVBoxLayout* vLayout = new QVBoxLayout();
	setLayout(vLayout);

	tblColorFrames = new QTableWidget();
	tblColorFrames->setSelectionBehavior(QAbstractItemView::SelectRows);
	tblColorFrames->setSelectionMode(QAbstractItemView::SingleSelection);
	tblColorFrames->setGridStyle(Qt::DotLine);

	tblColorFrames->setColumnCount(2);
	QStringList headers;
	headers << "Color" << "Time(s)";
	tblColorFrames->setHorizontalHeaderLabels(headers);

	vLayout->addWidget(tblColorFrames);

	connect(tblColorFrames, SIGNAL(itemSelectionChanged()), this, SLOT(HandleColorFrameTableSelectionChanged()));

	// Color FrameÏêÏ¸ÐÅÏ¢¿ò
	QHBoxLayout* hColorFrameDetail = new QHBoxLayout();
	vLayout->addLayout(hColorFrameDetail);

	btnSelColor = new QPushButton("Color");
	btnSelColor->setFixedWidth(60);
	hColorFrameDetail->addWidget(btnSelColor);
	connect(btnSelColor, SIGNAL(clicked(bool)), this, SLOT(HandleColorFrameButtonClicked()));

	//±à¼­°´Å¥
	QHBoxLayout* hLayout = new QHBoxLayout();
	btnAddColorFrame = new QPushButton("New");
	btnAddColorFrame->setFixedWidth(80);
	hLayout->addWidget(btnAddColorFrame);
	connect(btnAddColorFrame, SIGNAL(clicked(bool)), this, SLOT(HandleColorFrameButtonClicked()));

	btnDeleteColorFrame = new QPushButton("Delete");
	btnDeleteColorFrame->setFixedWidth(80);
	hLayout->addWidget(btnDeleteColorFrame);
	connect(btnDeleteColorFrame, SIGNAL(clicked(bool)), this, SLOT(HandleColorFrameButtonClicked()));

	vLayout->addLayout(hLayout);
}

MaterialColorFrameEditorDialog::~MaterialColorFrameEditorDialog()
{

}

void MaterialColorFrameEditorDialog::HandleColorFrameTableSelectionChanged()
{
	int row = tblColorFrames->currentRow();
	if(row < 0)
		return;

	//ValueAnimation& colorFrames
}

void MaterialColorFrameEditorDialog::HandleColorFrameButtonClicked()
{
	QPushButton* button = (QPushButton*)sender();
	if(button == btnDeleteColorFrame)
	{
		if(tblColorFrames->rowCount() <= 0)
			return;

		int row = tblColorFrames->currentRow();
		if(row < 0)
			return;

		tblColorFrames->removeRow(row);

		colorFrames.Erase(row);
	}
	else if(button == btnAddColorFrame)
	{
		VAnimKeyFrame frame;
		//frame.
		//frame.color_ = Color(0.2f, 0.3f, 0.9f);
		frame.time_ = 1.0f;
		colorFrames.Push(frame);

		//buildColorFrameTableFromEffect();
	}
}
