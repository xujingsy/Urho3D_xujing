#include "stdafx.h"
#include "ParticleEffectEditor.h"
#include "EditorApplication.h"

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

ParticleEffect* ParticleEffectEditor::GetEffect()
{
	return EditorApplication::Get()->GetEffect();
}

ParticleEmitter* ParticleEffectEditor::GetEmitter()
{
	return EditorApplication::Get()->GetEmitter();
}
