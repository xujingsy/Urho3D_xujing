#pragma once
#include "Object.h"
#include "Widgets/FloatEditor.h"
#include "Widgets/IntEditor.h"
#include "Widgets/ScrollAreaWidget.h"
#include "Widgets/ValueVarianceEditor.h"
#include "Widgets/Vector2Editor.h"
#include "Widgets/ColorVarianceEditor.h"

class ParticleEffectEditor : public Object
{
	OBJECT(ParticleEffectEditor)
public:
	ParticleEffectEditor(Context* context);
	virtual ParticleEffectEditor();

	// Update widget
	void UpdateWidget();

protected:
	// 数据变化时更新界面
	virtual void HandleUpdateWidget() = 0;

	/// Is updating widget.
	bool updatingWidget_;
};
