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

private:
	virtual void HandleUpdateWidget();
};
