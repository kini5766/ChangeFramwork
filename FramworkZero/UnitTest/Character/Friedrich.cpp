#include "stdafx.h"
#include "Friedrich.h"

void Friedrich::BindAnimation(Animator * animator, ModelAnimation * model)
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


FriedrichInstance::FriedrichInstance(ModelSkinnedInstance * instance)
{
}

FriedrichInstance::~FriedrichInstance()
{
}

void FriedrichInstance::Update()
{
}