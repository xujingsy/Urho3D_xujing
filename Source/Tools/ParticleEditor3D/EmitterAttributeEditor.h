#pragma once
#include "ParticleEffectEditor.h"

/// 发射器相关属性编辑

class EmitterAttributeEditor : public ScrollAreaWidget, public ParticleEffectEditor
{
	Q_OBJECT
	OBJECT(EmitterAttributeEditor)
public:
	EmitterAttributeEditor(Context* context);
	virtual ~EmitterAttributeEditor();
private slots:

private:
	virtual void HandleUpdateWidget();

};
