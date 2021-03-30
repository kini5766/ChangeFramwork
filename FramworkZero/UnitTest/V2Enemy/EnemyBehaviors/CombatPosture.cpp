#include "stdafx.h"
#include "CombatPosture.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/EnemyBehaviors/StrafeAround.h"
#include "V2Enemy/Skill/IEnemySkill.h"
#include "V2Enemy/PerceptionSystem.h"


CombatPosture::CombatPosture(const CombatPostureDesc& input)
	: desc(input)
	, funcReset(bind(&CombatPosture::Reset, this))
{
	strafeAround = new StrafeAround(desc.StrafeAroundDesc);
	reader = new FlowReader();

	tesks.push_back(strafeAround);
}

CombatPosture::~CombatPosture()
{
	SafeDelete(reader);

	SafeDelete(strafeAround);
}

void CombatPosture::Call(const FutureAction * action)
{
	result.SetAction(action);
	reader->Call(&funcReset);
}

void CombatPosture::Update()
{
	desc.StrafeAroundDesc.Perceptor->Update();
	if (desc.StrafeAroundDesc.Perceptor->IsPerceived() == false)
	{
		result.OnAction();
		return;
	}

	if (desc.Skill->IsValid())
	{
		reader->PushBack(desc.Skill->GetTesk());
		reader->CancelNext();
	}

	reader->Update();
}

void CombatPosture::Hold()
{
	result.Clear();
	reader->Hold();
}

void CombatPosture::Reset()
{
	reader->Reset();
	if (tesks.size() == 0)
		return;

	reader->PushBacks(tesks.size(), tesks.data());
	reader->Call(&funcReset);
}
