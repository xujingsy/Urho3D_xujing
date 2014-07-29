#include "stdafx.h"
#include "ParticleEffectEditor.h"
#include "EditorApplication.h"
#include <QColor>

ParticleEffectEditor::ParticleEffectEditor(Context* context) :
	Object(context)
{
	updatingWidget_ = false;
}

ParticleEffectEditor::~ParticleEffectEditor()
{

}

void ParticleEffectEditor::UpdateWidget()
{
	updatingWidget_ = true;

	HandleUpdateWidget();

	updatingWidget_ = false;
}

QString ParticleEffectEditor::getColorStyleString(float r, float g, float b)
{
	QColor qColor = QColor::fromRgbF(r, g, b, 255);

	char szStyle[512];
	sprintf(szStyle, "background-color: rgba(%d,%d,%d,255)",qColor.red() , qColor.green(), qColor.blue());

	return szStyle;
}

ParticleEffect* ParticleEffectEditor::GetEffect()
{
	return EditorApplication::Get()->GetEffect();
}

ParticleEmitter* ParticleEffectEditor::GetEmitter()
{
	return EditorApplication::Get()->GetEmitter();
}

Color ParticleEffectEditor::GetColorByQColor(const QColor& qColor)
{
	Color color(qColor.redF(), qColor.greenF(), qColor.blueF(), qColor.alphaF());
	return color;
}

QColor ParticleEffectEditor::GetQColorByColor(const Color& color)
{
	QColor qColor = QColor::fromRgbF(color.r_, color.g_, color.b_, color.a_);
	return qColor;
}