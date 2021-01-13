#include "stdafx.h"
#include "Kachujin.h"

#include "Objects/Model/ModelAnimation.h"


// --
// Paladin
// --

void Kachujin::BindAnimation(Animator* animator, ModelAnimation* model)
{
	animator->BindAll(model);

	animator->SetSpeed(3, 2.0f);
	animator->AddBlendEdge(3, 0, 0.1f, true);
}


KachujinInstance::KachujinInstance(ModelSkinnedInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	Kachujin::BindAnimation(animator, instance->GetAnimation());
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

KachujinInstance::~KachujinInstance()
{
	SafeDelete(animator);
}

void KachujinInstance::Update()
{
	animator->Update();
}
