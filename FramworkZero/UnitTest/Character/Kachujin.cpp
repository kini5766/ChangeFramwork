#include "stdafx.h"
#include "Kachujin.h"

#include "Objects/Model/ModelAnimation.h"

KachujinInstance::KachujinInstance(ModelSkinnedInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	animator = new Animator();
	animator->BindAll(instance->GetAnimation());

	animator->SetSpeed(3, 2.0f);
	animator->AddBlendEdge(3, 0, 0.1f, true);
}

KachujinInstance::~KachujinInstance()
{
	SafeDelete(animator);
}

void KachujinInstance::Update()
{
	animator->Update();
}
