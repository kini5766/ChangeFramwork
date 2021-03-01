#pragma once

#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"
#include "Rendering/Model/AnimationAdapter.h"
#include "Component/AttackAnimation.h"
#include "DelayFunction/DelayFunction.h"

struct V2EnemyStatus
{
	float HP = 100.0f;
};


struct V2EnemyInstanceDesc
{
	DelayReader* BehaviourSystem;
};