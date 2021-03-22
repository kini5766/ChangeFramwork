#include "stdafx.h"
#include "CombatPosture.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/EnemyBehaviors/StrafeAround.h"
#include "V2Enemy/Skill/IEnemySkill.h"


CombatPosture::CombatPosture(const CombatPostureDesc& input)
	: desc(input)
	, funcReset(bind(&CombatPosture::Reset, this))
{
	desc.StrafeAroundDesc.FuncOutRange = [=]() { 
		result.OnAction();
	};
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

	//if (desc.Skill->IsValid())
	//{
	//	reader->PushBack(desc.Skill->GetTesk());
	//	reader->HoldBackNext();
	//}

	// if (desc.Perceptor->GetDistanceSq() < desc.ApproachRangeSq)

	reader->Update();
}

void CombatPosture::Cancel()
{
	result.Clear();
	reader->Cancel();
}

void CombatPosture::Reset()
{
	reader->Clear();
	if (tesks.size() == 0)
		return;

	reader->PushBacks(tesks.size(), tesks.data());
	reader->Call(&funcReset);
}
