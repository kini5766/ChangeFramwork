#include "stdafx.h"
#include "Paladin.h"

#include "Objects/Model/ModelAnimation.h"

PaladinInstance::PaladinInstance(ModelSkinnedInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	instance->GetTransform()->Scale(0.03f, 0.03f, 0.03f);

	animator = new Animator(instance->GetAnimation());

	// 0 Idle
	animator->AddBlendEdge(0, 1, 0.1f);
	animator->AddBlendEdge(0, 2, 0.0f);
	animator->AddBlendEdge(0, 3, 0.1f);
	animator->AddBlendEdge(0, 4, 0.1f);
	animator->AddBlendEdge(0, 5, 0.1f);

	// 1 Walk
	animator->AddBlendEdge(1, 0, 0.1f);
	animator->AddBlendEdge(1, 2, 0.1f);
	animator->AddBlendEdge(1, 3, 0.1f);
	animator->AddBlendEdge(1, 4, 0.1f);
	animator->AddBlendEdge(1, 5, 0.1f);

	// 2 Run
	animator->AddBlendEdge(2, 0, 0.0f);
	animator->AddBlendEdge(2, 1, 0.1f);
	animator->AddBlendEdge(2, 3, 0.1f);
	animator->AddBlendEdge(2, 4, 0.0f);
	animator->AddBlendEdge(2, 5, 0.0f);

	// 3 Taunt
	animator->AddBlendEdge(3, 0, 0.1f);
	animator->AddBlendEdge(3, 1, 0.1f);
	animator->AddBlendEdge(3, 2, 0.0f, true);
	animator->AddBlendEdge(3, 4, 0.1f);
	animator->AddBlendEdge(3, 5, 0.1f);

	// 4 Attack
	animator->SetSpeed(4, 2.0f);
	animator->AddBlendEdge(4, 0, 0.1f, true);
	animator->AddBlendEdge(4, 1, 0.1f);
	animator->AddBlendEdge(4, 3, 0.1f);
	animator->AddBlendEdge(4, 4, 0.1f);
	animator->AddBlendEdge(4, 5, 0.1f);

	// 5 React
	animator->AddBlendEdge(5, 0, 0.1f, true);
	animator->AddBlendEdge(5, 1, 0.1f);
	animator->AddBlendEdge(5, 2, 0.1f);
	animator->AddBlendEdge(5, 3, 0.1f);
	animator->AddBlendEdge(5, 4, 0.1f);
}

PaladinInstance::~PaladinInstance()
{
	SafeDelete(animator);
}

void PaladinInstance::Update()
{
	animator->Update();
}
