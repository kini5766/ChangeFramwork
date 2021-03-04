#include "stdafx.h"
#include "GasMask.h"


// --
// Paladin
// --

void GasMask::BindAnimation(Animator* animator, AnimationAdapter* model)
{
	//animator->BindAll(model);

	animator->SetSpeed(3, 2.0f);
	animator->AddBlendEdge(3, 0, 0.1f, true);
}


GasMaskInstance::GasMaskInstance(ModelInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	//GasMask::BindAnimation(animator, instance->GetAnimAdapter());
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

GasMaskInstance::~GasMaskInstance()
{
	SafeDelete(animator);
}

void GasMaskInstance::Update()
{
	animator->Update();
}
