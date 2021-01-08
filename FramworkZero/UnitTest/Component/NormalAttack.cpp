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
	Instance* i = new Instance(this, parent);
	instances.push_back(i);
	return i->Anim();
}

void NormalAttack::Update()
{
	for (Instance* i : instances)
		i->Anim()->Update();
}


// -- 
// EnemyAttackInstance
// --

#include "Component/AttackSystem.h"

NormalAttack::Instance::Instance(NormalAttack* init, Transform* parent)
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
	anim->SetFuncStop(bind(&NormalAttack::Instance::Stop, this));
	anim->SetFuncAttackAble(bind(&NormalAttack::Instance::IsAttackAble, this));
	anim->AddUnit(bind(&NormalAttack::Instance::Update, this, placeholders::_1));
}

NormalAttack::Instance::~Instance()
{
	SafeDelete(anim);
	SafeDelete(system);
}

AttackAnimation * NormalAttack::Instance::Anim() 
{
	return anim;
}

bool NormalAttack::Instance::Update(float runningTime)
{
	if (system->IsAttacking())
		Debug::Box->RenderBox(system->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

	switch (state)
	{
	case NormalAttack::Instance::AttackState::None:
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

	case NormalAttack::Instance::AttackState::Ready:
		if (runningTime >= readyTime)
		{
			system->StartAttack();
			state = AttackState::Attack;
		}
		break;

	case NormalAttack::Instance::AttackState::Attack:
		if (runningTime >= endTime)
		{
			system->EndAttack();
			state = AttackState::Delay;
		}
		break;

	case NormalAttack::Instance::AttackState::Delay:
		if (runningTime >= delayTime)
		{
			state = AttackState::None;
			return true;
		}
		break;

	case NormalAttack::Instance::AttackState::Stop:  // 외부로 인한 종료
		state = AttackState::None;
		return true;
		break;
	}
	return false;
}

bool NormalAttack::Instance::IsAttackAble()
{
	if (state != AttackState::None)
		return false;

	return true;
}

void NormalAttack::Instance::Stop()
{
	state = AttackState::Stop;
}
