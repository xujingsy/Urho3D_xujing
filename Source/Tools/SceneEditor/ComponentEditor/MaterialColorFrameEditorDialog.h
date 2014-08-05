#pragma once
#include <QPushButton>
#include <QDialog>
#include <QTableWidget>

#include "Vector.h"
#include "ValueAnimation.h"
using namespace Urho3D;

class MaterialColorFrameEditorDialog : public QDialog
{
	Q_OBJECT;
public:
	MaterialColorFrameEditorDialog();
	~MaterialColorFrameEditorDialog();

	void SetColorFrames(const Vector<VAnimKeyFrame>& cFrames)
	{
		colorFrames = cFrames;
	}

	Vector<VAnimKeyFrame> GetColorFrames()
	{
		return colorFrames;
	}
private slots:
	void HandleColorFrameTableSelectionChanged();
	void HandleColorFrameButtonClicked();
private:
	QTableWidget* tblColorFrames;
	QPushButton* btnSelColor;
	QPushButton* btnDeleteColorFrame;
	QPushButton* btnAddColorFrame;

	Vector<VAnimKeyFrame> colorFrames;
};
