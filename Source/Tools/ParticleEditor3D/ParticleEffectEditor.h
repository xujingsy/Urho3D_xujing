#pragma once
#include "Object.h"
#include "Color.h"
#include "ParticleEffect.h"
#include "ParticleEmitter.h"
#include <QString>
#include <QColor>

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

	static Color GetColorByQColor(const QColor& qColor);
	static QColor GetQColorByColor(const Color& color);

protected:
	// 数据变化时更新界面
	virtual void HandleUpdateWidget() = 0;

	QString getColorStyleString(float r, float g, float b);

	ParticleEffect* GetEffect();
	ParticleEmitter* GetEmitter();

	/// Is updating widget.
	bool updatingWidget_;
};
