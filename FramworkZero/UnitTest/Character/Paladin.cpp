#include "stdafx.h"
#include "Paladin.h"

#include "Objects/Model/ModelAnimation.h"

PaladinInstance::PaladinInstance(ModelSkinnedInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	instance->GetTransform()->Scale(0.03f, 0.03f, 0.03f);

	animator = new Animator();
	animator->BindAll(instance->GetAnimation());

	// 0 Idle
	animator->AddBlendEdge(0, 2, 0.0f);
	// 1 Walk
	// 2 Run
	animator->AddBlendEdge(2, 0, 0.0f);
	animator->AddBlendEdge(2, 4, 0.0f);
	animator->AddBlendEdge(2, 5, 0.0f);

	// 3 Taunt
	animator->AddBlendEdge(3, 2, 0.0f, true);

	// 4 Attack
	animator->SetSpeed(4, 2.0f);
	animator->AddBlendEdge(4, 0, 0.1f, true);

	// 5 React
	animator->AddBlendEdge(5, 0, 0.1f, true);
}

PaladinInstance::~PaladinInstance()
{
	SafeDelete(animator);
}

void PaladinInstance::Update()
{
	animator->Update();
}
