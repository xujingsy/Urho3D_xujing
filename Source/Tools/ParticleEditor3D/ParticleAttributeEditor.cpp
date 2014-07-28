#include "stdafx.h"
#include "ParticleAttributeEditor.h"

ParticleAttributeEditor::ParticleAttributeEditor(Context* context) : ParticleEffectEditor(context)
{
	CreateRotationSpeed();
	CreateDirection();
	CreateVelocity();
	CreateParticleSize();

	//��֡����С
	CreateSizeAdd();
	CreateSizeMul();

	CreateTimeToLive();
	CreateConstantForce();
	CreateColorFrame();

	vBoxLayout_->addStretch(1);
}

ParticleAttributeEditor::~ParticleAttributeEditor()
{

}

void ParticleAttributeEditor::HandleUpdateWidget()
{
	ParticleEffect* effect_ = GetEffect();

	rotationSpeedMinEditor_->setValue(effect_->GetMinRotationSpeed());
	rotationSpeedMaxEditor_->setValue(effect_->GetMaxRotationSpeed());

	directionMinEditor_->setValue(effect_->GetMinDirection());
	directionMaxEditor_->setValue(effect_->GetMaxDirection());

	velocityMinEditor_->setValue(effect_->GetMinVelocity());
	velocityMaxEditor_->setValue(effect_->GetMaxVelocity());

	particleSizeMinEditor_->setValue(effect_->GetMinParticleSize());
	particleSizeMaxEditor_->setValue(effect_->GetMaxParticleSize());

	sizeAddEditor_->setValue(effect_->GetSizeAdd());
	sizeMulEditor_->setValue(effect_->GetSizeMul());

	timeToLiveMinEditor_->setValue(effect_->GetMinTimeToLive());
	timeToLiveMaxEditor_->setValue(effect_->GetMaxTimeToLive());

	constantForceEditor_->setValue(effect_->GetConstantForce());
}

void ParticleAttributeEditor::CreateRotationSpeed()
{
	rotationSpeedMinEditor_ = new FloatEditor("RotationSpeedMin", false);
	vBoxLayout_->addLayout(rotationSpeedMinEditor_);
	connect(rotationSpeedMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleRotationSpeedChanged(float)));

	rotationSpeedMaxEditor_ = new FloatEditor("RotationSpeedMax", false);
	vBoxLayout_->addLayout(rotationSpeedMaxEditor_);
	connect(rotationSpeedMaxEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleRotationSpeedChanged(float)));
}

void ParticleAttributeEditor::CreateDirection()
{
	directionMinEditor_ = new Vector3Editor("DirectionMin");
	vBoxLayout_->addWidget(directionMinEditor_);
	connect(directionMinEditor_, SIGNAL(valueChanged(const Vector3&)), this, SLOT(HandleDirectionChanged(const Vector3&)));

	directionMaxEditor_ = new Vector3Editor("DirectionMax");
	vBoxLayout_->addWidget(directionMaxEditor_);
	connect(directionMaxEditor_, SIGNAL(valueChanged(const Vector3&)), this, SLOT(HandleDirectionChanged(const Vector3&)));
}

void ParticleAttributeEditor::CreateVelocity()
{
	velocityMinEditor_ = new FloatEditor("VelocityMin", false);
	vBoxLayout_->addLayout(velocityMinEditor_);
	connect(velocityMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleVelocityChanged(float)));

	velocityMaxEditor_ = new FloatEditor("VelocityMax", false);
	vBoxLayout_->addLayout(velocityMaxEditor_);
	connect(velocityMaxEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleVelocityChanged(float)));
}

void ParticleAttributeEditor::CreateParticleSize()
{
	particleSizeMinEditor_ = new Vector2Editor("ParticleSizeMin");
	vBoxLayout_->addWidget(particleSizeMinEditor_);
	connect(particleSizeMinEditor_, SIGNAL(valueChanged(const Vector2&)), this, SLOT(HandleParticleSizeChanged(const Vector2&)));

	particleSizeMaxEditor_ = new Vector2Editor("ParticleSizeMax");
	vBoxLayout_->addWidget(particleSizeMaxEditor_);
	connect(particleSizeMaxEditor_, SIGNAL(valueChanged(const Vector2&)), this, SLOT(HandleParticleSizeChanged(const Vector2&)));
}

//��֡����С
void ParticleAttributeEditor::CreateSizeAdd()
{
	sizeAddEditor_ = new FloatEditor("SizeAdd", false);
	vBoxLayout_->addLayout(sizeAddEditor_);
	connect(sizeAddEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleSizeAddChanged(float)));
}

void ParticleAttributeEditor::CreateSizeMul()
{
	sizeMulEditor_ = new FloatEditor("SizeMul", false);
	vBoxLayout_->addLayout(sizeMulEditor_);
	connect(sizeMulEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleSizeMulChanged(float)));
}

void ParticleAttributeEditor::CreateTimeToLive()
{
	timeToLiveMinEditor_ = new FloatEditor("TimeToLiveMin", false);
	vBoxLayout_->addLayout(timeToLiveMinEditor_);
	connect(timeToLiveMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleTimeToLiveChanged(float)));

	timeToLiveMaxEditor_ = new FloatEditor("TimeToLiveMax", false);
	vBoxLayout_->addLayout(timeToLiveMaxEditor_);
	connect(timeToLiveMaxEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleTimeToLiveChanged(float)));
}

void ParticleAttributeEditor::CreateConstantForce()
{
	constantForceEditor_ = new Vector3Editor("ConstantForce");
	vBoxLayout_->addWidget(constantForceEditor_);
}

void ParticleAttributeEditor::CreateColorFrame()
{
	//todo:
}

//slots
void ParticleAttributeEditor::HandleRotationSpeedChanged(float value)
{
	if(updatingWidget_)
		return;

	FloatEditor* editor = (FloatEditor*)sender();
	if(editor == rotationSpeedMinEditor_)
	{
		GetEffect()->SetMinRotationSpeed(value);
	}
	else if(editor == rotationSpeedMaxEditor_)
	{
		GetEffect()->SetMaxRotationSpeed(value);
	}
}

void ParticleAttributeEditor::HandleDirectionChanged(const Vector3& value)
{
	if(updatingWidget_)
		return;

	Vector3Editor* editor = (Vector3Editor*)sender();

	if(editor == directionMinEditor_)
	{
		GetEffect()->SetMinDirection(value);
	}
	else if(editor == directionMaxEditor_)
	{
		GetEffect()->SetMaxDirection(value);
	}
}

void ParticleAttributeEditor::HandleVelocityChanged(float value)
{
	if(updatingWidget_)
		return;

	FloatEditor* editor = (FloatEditor*)sender();

	if(editor == velocityMinEditor_)
	{
		GetEffect()->SetMinVelocity(value);
	}
	else if(editor == velocityMaxEditor_)
	{
		GetEffect()->SetMaxVelocity(value);
	}
}	

void ParticleAttributeEditor::HandleParticleSizeChanged(const Vector2& value)
{
	if(updatingWidget_)
		return;

	Vector2Editor* editor = (Vector2Editor*)sender();

	if(editor == particleSizeMinEditor_)
	{
		GetEffect()->SetMinParticleSize(value);
	}
	else if(editor == particleSizeMaxEditor_)
	{
		GetEffect()->SetMaxParticleSize(value);
	}
}

void ParticleAttributeEditor::HandleSizeAddChanged(float value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetSizeAdd(sizeAddEditor_->value());
}

void ParticleAttributeEditor::HandleSizeMulChanged(float value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetSizeMul(sizeMulEditor_->value());
}

void ParticleAttributeEditor::HandleTimeToLiveChanged(float value)
{
	if(updatingWidget_)
		return;

	FloatEditor* editor = (FloatEditor*)sender();
	if(editor == timeToLiveMinEditor_)
	{
		GetEffect()->SetMinTimeToLive(value);
	}
	else if(editor == timeToLiveMaxEditor_)
	{
		GetEffect()->SetMaxTimeToLive(value);
	}
}

void ParticleAttributeEditor::HandleConstanceForceChanged(const Vector3& value)
{
	if(updatingWidget_)
		return;

	GetEffect()->SetConstantForce(value);
}