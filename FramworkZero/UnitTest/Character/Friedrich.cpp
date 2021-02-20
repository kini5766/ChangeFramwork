#include "stdafx.h"
#include "Friedrich.h"

#include "Rendering/Model/AnimationAdapter.h"

// --
// Friedrich
// --

void Friedrich::BindAnimation(Animator * animator, AnimationAdapter * model)
{
	animator->BindAll(model);

	// 0 Idle
	// 1 Walk
	// 2 Run

	// 3 Taunt
	animator->AddBlendEdge(3, 2, 0.1f, true);

	// 4 Attack
	animator->SetSpeed(4, 2.0f);
	animator->AddBlendEdge(4, 0, 0.1f, true);

	// 5 React
	animator->AddBlendEdge(5, 0, 0.1f, true);
}


// --
// FriedrichInstance
// --

FriedrichInstance::FriedrichInstance(ModelInstance * instance)
{
	animator = new Animator();
	Friedrich::BindAnimation(animator, instance->GetAnimAdapter());
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

FriedrichInstance::~FriedrichInstance()
{
	SafeDelete(animator);
}

void FriedrichInstance::Update()
{
	animator->Update();
}