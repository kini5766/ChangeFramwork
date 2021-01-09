#include "stdafx.h"
#include "NormalAttack.h"


// --
// EnemyAttackSystem
// --

#include "Component/AttackAnimation.h"

NormalAttack::NormalAttack()
{
	initTransform = new Transform();
}

NormalAttack::~NormalAttack()
{
	SafeDelete(initTransform);
	for (auto d : instances)
		SafeDelete(d);
}

AttackAnimation * NormalAttack::MakeInstance(Transform* parent)
{
	Instance_N* i = new Instance_N(this, parent);
	instances.push_back(i);
	return i->Anim();
}

void NormalAttack::Update()
{
	for (Instance_N* i : instances)
		i->Anim()->Update();
}


// -- 
// EnemyAttackInstance
// --

#include "Component/AttackSystem.h"

NormalAttack::Instance_N::Instance_N(NormalAttack* init, Transform* parent)
	: endTime(init->endTime)
	, readyTime(init->readyTime)
	, delayTime(init->delayTime)
{
	system = new AttackSystem();
	system->GetTransform()->SetParent(parent);
	Matrix w;
	init->InitTransform()->LocalWorld(&w);
	system->GetTransform()->LocalWorld(w);
	system->SetTag(init->Tag());

	anim = new AttackAnimation();
	anim->SetFuncStop(bind(&NormalAttack::Instance_N::Stop, this));
	anim->SetFuncAttackAble(bind(&NormalAttack::Instance_N::IsAttackAble, this));
	anim->AddUnit(bind(&NormalAttack::Instance_N::Update, this, placeholders::_1));
}

NormalAttack::Instance_N::~Instance_N()
{
	SafeDelete(anim);
	SafeDelete(system);
}

AttackAnimation * NormalAttack::Instance_N::Anim()
{
	return anim;
}

bool NormalAttack::Instance_N::Update(float runningTime)
{
	if (system->IsAttacking())
		Debug::Box->RenderBox(system->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

	switch (state)
	{
	case NormalAttack::Instance_N::AttackState::None:
		if (runningTime == 0.0f)
		{
			// 공격 시작
			state = AttackState::Ready;
		}
		else
		{
			return true;
		}
		break;

	case NormalAttack::Instance_N::AttackState::Ready:
		if (runningTime >= readyTime)
		{
			system->StartAttack();
			state = AttackState::Attack;
		}
		break;

	case NormalAttack::Instance_N::AttackState::Attack:
		if (runningTime >= endTime)
		{
			system->EndAttack();
			state = AttackState::Delay;
		}
		break;

	case NormalAttack::Instance_N::AttackState::Delay:
		if (runningTime >= delayTime)
		{
			state = AttackState::None;
			return true;
		}
		break;

	case NormalAttack::Instance_N::AttackState::Stop:  // 외부로 인한 종료
		state = AttackState::None;
		return true;
		break;
	}
	return false;
}

bool NormalAttack::Instance_N::IsAttackAble()
{
	if (state != AttackState::None)
		return false;

	return true;
}

void NormalAttack::Instance_N::Stop()
{
	state = AttackState::Stop;
}
