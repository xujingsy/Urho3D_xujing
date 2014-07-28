#pragma once
#include "ParticleEffectEditor.h"

class ParticleAttributeEditor : public ScrollAreaWidget, public ParticleEffectEditor
{
	Q_OBJECT;
	OBJECT(ParticleAttributeEditor);
public:
	ParticleAttributeEditor(Context* context);
	virtual ~ParticleAttributeEditor();
private slots:
	void HandleRotationSpeedChanged(float value);
	void HandleDirectionChanged(const Vector3& value);
	void HandleVelocityChanged(float value);
	void HandleParticleSizeChanged(const Vector2& value);
	void HandleSizeAddChanged(float value);
	void HandleSizeMulChanged(float value);
	void HandleTimeToLiveChanged(float value);

	void HandleConstanceForceChanged(const Vector3& value);
private:
	void CreateRotationSpeed();
	void CreateDirection();
	void CreateVelocity();
	void CreateParticleSize();

	//ึ๐ึกิ๖ด๓ะก
	void CreateSizeAdd();
	void CreateSizeMul();

	void CreateTimeToLive();
	void CreateConstantForce();
	void CreateColorFrame();

	virtual void HandleUpdateWidget();

	FloatEditor* rotationSpeedMinEditor_;
	FloatEditor* rotationSpeedMaxEditor_;

	Vector3Editor* directionMinEditor_;
	Vector3Editor* directionMaxEditor_;

	FloatEditor* velocityMinEditor_;
	FloatEditor* velocityMaxEditor_;

	Vector2Editor* particleSizeMinEditor_;
	Vector2Editor* particleSizeMaxEditor_;

	FloatEditor* sizeAddEditor_;
	FloatEditor* sizeMulEditor_;

	FloatEditor* timeToLiveMinEditor_;
	FloatEditor* timeToLiveMaxEditor_;

	Vector3Editor* constantForceEditor_;
};
