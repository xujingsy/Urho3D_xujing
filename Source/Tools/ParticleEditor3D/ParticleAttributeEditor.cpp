#include "stdafx.h"
#include "ParticleAttributeEditor.h"
#include <QMessageBox>

ParticleAttributeEditor::ParticleAttributeEditor(Context* context) : ParticleEffectEditor(context)
{
	CreateRotationSpeed();
	CreateDirection();
	CreateVelocity();
	CreateParticleSize();

	//逐帧增大小
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

	buildColorFrameTableFromEffect();
}

void ParticleAttributeEditor::CreateRotationSpeed()
{
	rotationSpeedMinEditor_ = new FloatEditor("RotationSpeedMin", false);
	rotationSpeedMinEditor_->setRange(0.0f, 360.0f);
	vBoxLayout_->addLayout(rotationSpeedMinEditor_);
	connect(rotationSpeedMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleRotationSpeedChanged(float)));

	rotationSpeedMaxEditor_ = new FloatEditor("RotationSpeedMax", false);
	rotationSpeedMaxEditor_->setRange(0.0f, 360.0f);
	vBoxLayout_->addLayout(rotationSpeedMaxEditor_);
	connect(rotationSpeedMaxEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleRotationSpeedChanged(float)));
}

void ParticleAttributeEditor::CreateDirection()
{
	directionMinEditor_ = new Vector3Editor("DirectionMin");
	directionMinEditor_->setRange(Vector3(), Vector3(360.0f, 360.0f, 360.0f));
	vBoxLayout_->addWidget(directionMinEditor_);
	connect(directionMinEditor_, SIGNAL(valueChanged(const Vector3&)), this, SLOT(HandleDirectionChanged(const Vector3&)));

	directionMaxEditor_ = new Vector3Editor("DirectionMax");
	directionMaxEditor_->setRange(Vector3(), Vector3(360.0f, 360.0f, 360.0f));
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
	particleSizeMinEditor_->setRange(Vector2(0.01f, 0.01f), Vector2(10.0f, 10.0f));
	vBoxLayout_->addWidget(particleSizeMinEditor_);
	connect(particleSizeMinEditor_, SIGNAL(valueChanged(const Vector2&)), this, SLOT(HandleParticleSizeChanged(const Vector2&)));

	particleSizeMaxEditor_ = new Vector2Editor("ParticleSizeMax");
	particleSizeMaxEditor_->setRange(Vector2(0.01f, 0.01f), Vector2(10.0f, 10.0f));
	vBoxLayout_->addWidget(particleSizeMaxEditor_);
	connect(particleSizeMaxEditor_, SIGNAL(valueChanged(const Vector2&)), this, SLOT(HandleParticleSizeChanged(const Vector2&)));
}

//逐帧增大小，定义为每秒增加的缩放比
void ParticleAttributeEditor::CreateSizeAdd()
{
	sizeAddEditor_ = new FloatEditor("SizeAdd", false);
	sizeAddEditor_->setRange(0.0f, 10.0f);
	vBoxLayout_->addLayout(sizeAddEditor_);
	connect(sizeAddEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleSizeAddChanged(float)));
}

//和SizeAdd不同的是，这里的增加是按照乘法累积递增的
void ParticleAttributeEditor::CreateSizeMul()
{
	sizeMulEditor_ = new FloatEditor("SizeMul", false);
	sizeMulEditor_->setRange(0.0f, 5.0f);
	vBoxLayout_->addLayout(sizeMulEditor_);
	connect(sizeMulEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleSizeMulChanged(float)));
}

void ParticleAttributeEditor::CreateTimeToLive()
{
	timeToLiveMinEditor_ = new FloatEditor("TimeToLiveMin", false);
	timeToLiveMinEditor_->setRange(0, 360.0f);
	vBoxLayout_->addLayout(timeToLiveMinEditor_);
	connect(timeToLiveMinEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleTimeToLiveChanged(float)));

	timeToLiveMaxEditor_ = new FloatEditor("TimeToLiveMax", false);
	timeToLiveMaxEditor_->setRange(0, 360.0f);
	vBoxLayout_->addLayout(timeToLiveMaxEditor_);
	connect(timeToLiveMaxEditor_, SIGNAL(valueChanged(float)), this, SLOT(HandleTimeToLiveChanged(float)));
}

//加速度
void ParticleAttributeEditor::CreateConstantForce()
{
	constantForceEditor_ = new Vector3Editor("ConstantForce");
	constantForceEditor_->setRange(Vector3(), Vector3(100.0f, 100.0f, 100.0f));
	vBoxLayout_->addWidget(constantForceEditor_);

	connect(constantForceEditor_, SIGNAL(valueChanged(const Vector3&)), this, SLOT(HandleConstanceForceChanged(const Vector3&)));
}

void ParticleAttributeEditor::CreateColorFrame()
{
	tblColorFrames = new QTableWidget();
	tblColorFrames->setGridStyle(Qt::DotLine);
	
	tblColorFrames->setColumnCount(2);
	QStringList headers;
	headers << "Color" << "Time(s)";
	tblColorFrames->setHorizontalHeaderLabels(headers);

	vBoxLayout_->addWidget(tblColorFrames);

	//编辑按钮
	QHBoxLayout* hLayout = AddHBoxLayout();
	btnSaveColorFrame = new QPushButton("Save");
	btnSaveColorFrame->setFixedWidth(80);
	hLayout->addWidget(btnSaveColorFrame);
	
	btnDeleteColorFrame = new QPushButton("Delete");
	btnDeleteColorFrame->setFixedWidth(80);
	hLayout->addWidget(btnDeleteColorFrame);
	connect(btnDeleteColorFrame, SIGNAL(clicked(bool)), this, SLOT(HandleColorFrameButtonClicked()));

	btnAddColorFrame = new QPushButton("New");
	btnAddColorFrame->setFixedWidth(80);
	hLayout->addWidget(btnAddColorFrame);
	connect(btnAddColorFrame, SIGNAL(clicked(bool)), this, SLOT(HandleColorFrameButtonClicked()));
}

void ParticleAttributeEditor::buildColorFrameTableFromEffect()
{
	ParticleEffect* effect = GetEffect();
	Vector<ColorFrame> colorFrames = effect->GetColorFrames();

	tblColorFrames->setRowCount(colorFrames.Size());

	QStringList headers;
	for(int i = 0;i < tblColorFrames->rowCount();i ++)
	{
		char szNum[32];
		sprintf(szNum, "%d", i);
		headers << szNum;

		Color& color = colorFrames[i].color_;
		QColor qColor = QColor::fromRgbF(color.r_, color.g_, color.b_);

		char szColor[64];
		sprintf(szColor, "(%d,%d,%d)", qColor.red(), qColor.green(), qColor.blue());
		QTableWidgetItem* itemColor = new QTableWidgetItem(szColor);
		itemColor->setBackgroundColor(qColor);
		tblColorFrames->setItem(i, 0, itemColor);
		
		char szTime[64];
		sprintf(szTime, "%f", colorFrames[i].time_);
		QTableWidgetItem* itemTime = new QTableWidgetItem(szTime);
		tblColorFrames->setItem(i, 1, itemTime);
	}
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

void ParticleAttributeEditor::HandleColorFrameButtonClicked()
{
	QPushButton* button = (QPushButton*)sender();
	if(button == btnSaveColorFrame)
	{

	}
	else if(button == btnDeleteColorFrame)
	{
		if(tblColorFrames->rowCount() <= 0)
			return;

		if(tblColorFrames->rowCount() == 1)
		{
			QMessageBox msg(QMessageBox::Warning, "Warning", "At less one color frame!");
			msg.exec();
			return;
		}

		int row = tblColorFrames->currentRow();
		if(row < 0)
			return;

		tblColorFrames->removeRow(row);

		Vector<ColorFrame> colorFrames = GetEffect()->GetColorFrames();
		colorFrames.Erase(row);

		GetEffect()->SetColorFrames(colorFrames);
	}
	else if(button == btnAddColorFrame)
	{
		Vector<ColorFrame> colorFrames = GetEffect()->GetColorFrames();
		ColorFrame frame;
		frame.color_ = Color(0.2f, 0.3f, 0.9f);
		frame.time_ = 1.0f;
		colorFrames.Push(frame);

		GetEffect()->SetColorFrames(colorFrames);
		buildColorFrameTableFromEffect();
	}
}
