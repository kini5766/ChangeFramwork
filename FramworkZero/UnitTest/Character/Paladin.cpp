#include "stdafx.h"
#include "Paladin.h"

#include "Rendering/Model/AnimationAdapter.h"


// --
// Paladin
// --

void Paladin::BindAnimation(Animator* animator, AnimationAdapter* model)
{
	animator->BindAll(model);

	// 0 Idle
	animator->AddBlendEdge(0, 2, 0.0f);
	// 1 Walk
	// 2 Run
	animator->AddBlendEdge(2, 0, 0.0f);
	animator->AddBlendEdge(2, 4, 0.0f);
	//animator->AddBlendEdge(2, 5, 0.0f);

	// 3 Taunt
	animator->AddBlendEdge(3, 2, 0.0f, true);

	// 4 Attack
	animator->SetSpeed(4, 2.0f);
	animator->AddBlendEdge(4, 2, 0.0f);
	animator->AddBlendEdge(4, 0, 0.1f, true);

	// 5 React
	animator->AddBlendEdge(5, 0, 0.1f, true);
}


// --
// PaladinInstance
// --

PaladinInstance::PaladinInstance(ModelInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	Paladin::BindAnimation(animator, instance->GetAnimAdapter());

	instance->GetTransform()->Scale(0.03f, 0.03f, 0.03f);
}

PaladinInstance::~PaladinInstance()
{
	SafeDelete(animator);
}

void PaladinInstance::Update()
{
	animator->Update();
}
