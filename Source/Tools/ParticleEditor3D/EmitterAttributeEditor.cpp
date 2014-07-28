#include "stdafx.h"
#include "EmitterAttributeEditor.h"
#include "EditorApplication.h"
#include <QCheckBox>

EmitterAttributeEditor::EmitterAttributeEditor(Context* context) :
	ParticleEffectEditor(context)
{
	CreateEmitterTypeEditor();
	CreateMaxParticleEditor();
	CreateUpdateInvisible();

	CreateRelative();
	CreateActiveTime();
	CreateInActiveTime();
	CreateEmissionRate();
	CreateIsSorted();

	vBoxLayout_->addStretch(1);
	maxParticlesChanged_ = false;

	SubscribeToEvent(E_POSTUPDATE, HANDLER(EmitterAttributeEditor, HandlePostUpdate));
}

EmitterAttributeEditor::~EmitterAttributeEditor()
{
}

//Data -> UI
void EmitterAttributeEditor::HandleUpdateWidget()
{
	ParticleEffect* effect_ = GetEffect();

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

void EmitterAttributeEditor::CreateActiveTime()
{
	activeTimeEditor_ = new FloatEditor("ActiveTime");
	vBoxLayout_->addLayout(activeTimeEditor_);

	connect(activeTimeEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleActiveTimeChanged(float)));
}

void EmitterAttributeEditor::CreateInActiveTime()
{
	inActiveTimeEditor_ = new FloatEditor("InActiveTime");
	vBoxLayout_->addLayout(inActiveTimeEditor_);

	connect(inActiveTimeEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleInActiveTimeChanged(float)));
}

void EmitterAttributeEditor::CreateEmissionRate()
{
	emissionRateMinEditor_ = new FloatEditor("EmissionRate", false);
	vBoxLayout_->addLayout(emissionRateMinEditor_);

	connect(emissionRateMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleEmissionRateChanged(float)));

	emissionRateMaxEditor_ = new FloatEditor("EmissionRate", false);
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

///
void EmitterAttributeEditor::HandlePostUpdate(StringHash eventType, VariantMap& eventData)
{
	if(!maxParticlesChanged_)
		return;

	maxParticlesChanged_ = false;

	GetEffect()->SetNumParticles(maxParticleEditor_->value());
}
