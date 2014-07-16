#pragma once
#include <qwidget.h>
#include <qdialog.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qboxlayout.h>
#include <qicon.h>
#include <qpushbutton.h>

namespace Urho3D
{
	class Context;
}
using namespace Urho3D;

class ModelTool : public QDialog
{
	Q_OBJECT
public:
	ModelTool(QWidget* parent = 0);
	virtual ~ModelTool();

	void SetModelPath(const char* pPath);

private:
	bool import(const char* pPath);
	void initUi();
	Context* context_;

	//¿Ø¼þ
	QRadioButton* radModel;
	QRadioButton* radScene;
	QRadioButton* radNode;

	QCheckBox* chkB;
	QCheckBox* chkH;
	QCheckBox* chkI;
	QCheckBox* chkL;
	QCheckBox* chkNA;
	QCheckBox* chkNM;
	QCheckBox* chkNT;
	QCheckBox* chkNC;
	QCheckBox* chkNH;
	QCheckBox* chkNS;
	QCheckBox* chkNZ;
	QCheckBox* chkNF;
	QCheckBox* chkT;
	QCheckBox* chkV;
	QCheckBox* chkEAO;
	QCheckBox* chkCM;
	QCheckBox* chkCT;
	QCheckBox* chkCTN;

	QPushButton* btnExport;
};
