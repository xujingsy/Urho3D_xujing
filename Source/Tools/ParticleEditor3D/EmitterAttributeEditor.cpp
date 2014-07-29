#include "stdafx.h"
#include "EmitterAttributeEditor.h"
#include "EditorApplication.h"
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>

EmitterAttributeEditor::EmitterAttributeEditor(Context* context) :
	ParticleEffectEditor(context)
{
	CreateEmitterTypeEditor();
	CreateMaxParticleEditor();
	CreateSelectMaterialEditor();
	CreateUpdateInvisible();

	CreateRelative();
	CreateActiveTime();
	CreateInActiveTime();
	CreateEmissionRate();
	CreateIsSorted();

	vBoxLayout_->addStretch(1);
}

EmitterAttributeEditor::~EmitterAttributeEditor()
{
}

//Data -> UI
void EmitterAttributeEditor::HandleUpdateWidget()
{
	ParticleEffect* effect_ = GetEffect();

	materialPathEditor_->setText(effect_->GetMaterial()->GetName().CString());
	emitterTypeEditor_->setCurrentIndex(effect_->GetEmitterType());
	maxParticleEditor_->setValue(effect_->GetNumParticles());

	activeTimeEditor_->setValue(effect_->GetActiveTime());
	inActiveTimeEditor_->setValue(effect_->GetInactiveTime());
	emissionRateMinEditor_->setValue(effect_->GetMinEmissionRate());
	emissionRateMaxEditor_->setValue(effect_->GetMaxEmissionRate());
}

void EmitterAttributeEditor::CreateEmitterTypeEditor()
{
	emitterTypeEditor_ = new QComboBox();
	vBoxLayout_->addWidget(emitterTypeEditor_);

	emitterTypeEditor_->addItem(tr("Sphere"));
	emitterTypeEditor_->addItem(tr("Box"));
	emitterTypeEditor_->setCurrentIndex(-1);

	connect(emitterTypeEditor_, SIGNAL(currentIndexChanged(int)), this, SLOT(HandleEmitterTypeEditorChanged(int)));
}

void EmitterAttributeEditor::CreateSelectMaterialEditor()
{
	QHBoxLayout* hBoxLayout = AddHBoxLayout();
	hBoxLayout->addWidget(new QLabel(tr("Material")));

	materialPathEditor_ = new QLineEdit();
	materialPathEditor_->setReadOnly(true);
	hBoxLayout->addWidget(materialPathEditor_, 1);

	QPushButton* selectButton = new QPushButton("...");
	selectButton->setFixedWidth(32);
	hBoxLayout->addWidget(selectButton);

	connect(selectButton, SIGNAL(clicked(bool)), this, SLOT(HandleSelectMaterialClick()));
}

void EmitterAttributeEditor::CreateMaxParticleEditor()
{
	maxParticleEditor_ = new IntEditor(tr("MaxParticles"));
	vBoxLayout_->addLayout(maxParticleEditor_);

	maxParticleEditor_->setRange(1, 2048);

	connect(maxParticleEditor_, SIGNAL(valueChanged(int)), this, SLOT(HandleMaxParticlesEditorValueChanged(int)));
}

//当粒子不可见时是否更新
void EmitterAttributeEditor::CreateUpdateInvisible()
{
	QCheckBox* chkUpdateInvisible = new QCheckBox("UpdateInvisible");
	vBoxLayout_->addWidget(chkUpdateInvisible);

	connect(chkUpdateInvisible, SIGNAL(stateChanged(int)), this, SLOT(HandleUpdateInvisibleChanged(int)));
}

void EmitterAttributeEditor::CreateRelative()
{
	QCheckBox* chkRelative = new QCheckBox("Relative");
	vBoxLayout_->addWidget(chkRelative);

	connect(chkRelative, SIGNAL(stateChanged(int)), this, SLOT(HandleRelativeChanged(int)));
}

//unit:second
void EmitterAttributeEditor::CreateActiveTime()
{
	activeTimeEditor_ = new FloatEditor("ActiveTime");
	activeTimeEditor_->setRange(0.0f, 360.f);	//6 minute
	vBoxLayout_->addLayout(activeTimeEditor_);

	connect(activeTimeEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleActiveTimeChanged(float)));
}

void EmitterAttributeEditor::CreateInActiveTime()
{
	inActiveTimeEditor_ = new FloatEditor("InActiveTime");
	activeTimeEditor_->setRange(0.0f, 360.f);
	vBoxLayout_->addLayout(inActiveTimeEditor_);

	connect(inActiveTimeEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleInActiveTimeChanged(float)));
}

void EmitterAttributeEditor::CreateEmissionRate()
{
	emissionRateMinEditor_ = new FloatEditor("EmissionRateMin", false);
	emissionRateMinEditor_->setRange(1, 3600);
	vBoxLayout_->addLayout(emissionRateMinEditor_);

	connect(emissionRateMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleEmissionRateChanged(float)));

	emissionRateMaxEditor_ = new FloatEditor("EmissionRateMax", false);
	emissionRateMaxEditor_->setRange(1, 3600);
	vBoxLayout_->addLayout(emissionRateMaxEditor_);

	connect(emissionRateMaxEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleEmissionRateChanged(float)));
}

void EmitterAttributeEditor::CreateIsSorted()
{
	QCheckBox* chk = new QCheckBox("IsSorted");
	vBoxLayout_->addWidget(chk);

	connect(chk, SIGNAL(stateChanged(int)), this, SLOT(HandleIsSortedChanged(int)));
}

//slots
void EmitterAttributeEditor::HandleSelectMaterialClick()
{
	if(updatingWidget_)
		return;

	QString fileName = QFileDialog::getOpenFileName(0, tr("Material"), "./Data/Materials/", "*.xml");
	if(fileName.isEmpty())
		return;

	//must in app path
	static QString dataPath = qApp->applicationDirPath() + "/Data/";
	if(fileName.toUpper().left(dataPath.toUpper().length()) != dataPath.toUpper())
		return;

	fileName = fileName.right(fileName.length() - dataPath.length());

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	Material* material = cache->GetResource<Material>(fileName.toLatin1().data());
	if(material == NULL)
		return;

	materialPathEditor_->setText(fileName);
	GetEffect()->SetMaterial(material);
	GetEmitter()->SetMaterial(material);
}

void EmitterAttributeEditor::HandleEmitterTypeEditorChanged(int value)
{
	if(updatingWidget_)
		return;

	EmitterType emitterType = (EmitterType)value;

	GetEffect()->SetEmitterType(emitterType);
}

void EmitterAttributeEditor::HandleMaxParticlesEditorValueChanged(int value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetNumParticles(maxParticleEditor_->value());
	GetEmitter()->ApplyEffect();
}

void EmitterAttributeEditor::HandleUpdateInvisibleChanged(int value)
{
	if(updatingWidget_)
		return;

	QCheckBox* chk = (QCheckBox*)sender();

	GetEffect()->SetUpdateInvisible(chk->checkState() == Qt::Checked);
}

void EmitterAttributeEditor::HandleRelativeChanged(int value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetRelative(value == 1);
}

void EmitterAttributeEditor::HandleActiveTimeChanged(float value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetActiveTime(value);
	GetEmitter()->ApplyEffect();
}

void EmitterAttributeEditor::HandleInActiveTimeChanged(float value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetInactiveTime(value);
}

void EmitterAttributeEditor::HandleEmissionRateChanged(float value)
{
	if(updatingWidget_)
		return;

	FloatEditor* editor = (FloatEditor*)sender();
	if(editor == emissionRateMinEditor_)
	{
		GetEffect()->SetMinEmissionRate(value);
	}
	else if(editor == emissionRateMaxEditor_)
	{
		GetEffect()->SetMaxEmissionRate(value);
	}
}

void EmitterAttributeEditor::HandleIsSortedChanged(int value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetSorted(value == 1);
	GetEmitter()->SetSorted(value == 1);
}
