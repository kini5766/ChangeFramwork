#include "stdafx.h"
#include "Kachujin.h"

#include "Objects/Model/ModelAnimation.h"

KachujinInstance::KachujinInstance(ModelSkinnedInstance * instance)
	: instance(instance)
{
	animator = new Animator();
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	animator = new Animator(instance->GetAnimation());
	animator->AddBlendEdge(0, 1, 0.1f);
	animator->AddBlendEdge(0, 2, 0.1f);
	animator->AddBlendEdge(0, 3, 0.1f);
	animator->AddBlendEdge(0, 4, 0.1f);

	animator->AddBlendEdge(1, 0, 0.1f);
	animator->AddBlendEdge(1, 2, 0.1f);
	animator->AddBlendEdge(1, 3, 0.1f);
	animator->AddBlendEdge(1, 4, 0.1f);

	animator->AddBlendEdge(2, 0, 0.1f);
	animator->AddBlendEdge(2, 1, 0.1f);
	animator->AddBlendEdge(2, 3, 0.1f);
	animator->AddBlendEdge(2, 4, 0.1f);

	animator->SetSpeed(3, 2.0f);
	animator->AddBlendEdge(3, 0, 0.1f, true);
	animator->AddBlendEdge(3, 1, 0.1f);
	animator->AddBlendEdge(3, 2, 0.1f);
	animator->AddBlendEdge(3, 4, 0.1f);

	animator->AddBlendEdge(4, 0, 0.1f);
	animator->AddBlendEdge(4, 1, 0.1f);
	animator->AddBlendEdge(4, 2, 0.1f);
	animator->AddBlendEdge(4, 3, 0.1f);
}

KachujinInstance::~KachujinInstance()
{
	SafeDelete(animator);
}

void KachujinInstance::Update()
{
	animator->Update();
}
