#pragma once
#include "Object.h"
#include "ParticleEffect.h"
#include "ParticleEmitter.h"

#include "Widgets/FloatEditor.h"
#include "Widgets/IntEditor.h"
#include "Widgets/ScrollAreaWidget.h"
#include "Widgets/ValueVarianceEditor.h"
#include "Widgets/Vector2Editor.h"
#include "Widgets/Vector3Editor.h"
#include "Widgets/ColorVarianceEditor.h"

class ParticleEffectEditor : public Object
{
	OBJECT(ParticleEffectEditor)
public:
	ParticleEffectEditor(Context* context);
	virtual ~ParticleEffectEditor();

	// Update widget
	void UpdateWidget();

protected:
	// 数据变化时更新界面
	virtual void HandleUpdateWidget() = 0;

	ParticleEffect* GetEffect();
	ParticleEmitter* GetEmitter();

	/// Is updating widget.
	bool updatingWidget_;
};
