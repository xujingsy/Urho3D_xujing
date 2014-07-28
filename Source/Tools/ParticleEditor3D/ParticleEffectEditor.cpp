#include "ParticleEffectEditor.h"

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
