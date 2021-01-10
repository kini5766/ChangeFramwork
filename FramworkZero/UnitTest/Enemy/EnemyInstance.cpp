#include "stdafx.h"
#include "EnemyInstance.h"

#include "Character/Paladin.h"
#include "Tools/Viewer/IFocus.h"

#include "Component/HPSystem.h"
#include "Component/RotateSystem.h"
#include "Enemy/EnemyDesc.h"

EnemyInstance::EnemyInstance(const EnemyInstanceDesc* desc)
	: player(desc->Player), transform(desc->Transform), animator(desc->Animator), attack(desc->Attack)
	, runSpeed(desc->Desc->RunSpeed), walkSpeed(desc->Desc->WalkSpeed), turnSpeed(desc->Desc->TurnSpeed)
	, detectionRange(desc->Desc->DetectionRange), attackRange(desc->Desc->AttackRange)
	, patrolPoints(*desc->PatrolPoints)
{
	hp = new HPSystem(desc->Desc->HP);
	hp->GetHpbar()->SetParent(transform);
	hp->GetHpbar()->Position(0, 180.0f, 0);

	hp->GetHurtbox()->GetTransform()->SetParent(transform);
	hp->GetHurtbox()->GetTransform()->Position(0.0f, 90.0f, 0.0f);
	hp->GetHurtbox()->GetTransform()->Rotation(0.0f, 0.0f, 0.0f);
	hp->GetHurtbox()->GetTransform()->Scale(75.0f, 180.0f, 75.0f); 
	hp->GetHurtbox()->SetLayer(
		COLLIDER_LAYER_ENEMY |
		COLLIDER_LAYER_HITBOX
	);
	hp->GetHurtbox()->Tag(L"Enemy");
	hp->AddReceiveTag(L"PlayerAttack");
	hp->SetFuncDamage(bind(&EnemyInstance::OnDamage, this));

	animator->SetFuncNext(
		bind(&EnemyInstance::OnNextAnimation, this, placeholders::_1)
	);

	ratate = new RotateSystem();
}

EnemyInstance::~EnemyInstance()
{
	SafeDelete(ratate);
	SafeDelete(hp);
	SafeDelete(animator);
}

void EnemyInstance::Update()
{
	if (bLost)
		return;

	UpdateState();
	animator->Update();
	hp->Update();
}

void EnemyInstance::Render()
{
	if (bLost)
		return;

	hp->Render();
}

void EnemyInstance::UpdateState()
{
	if (hp->HP() <= 0.0f)
	{
		if (currAction != 6)
		{
			animator->Play(6);
			currAction = 6;
		}
		return;
	}

	if (reactRunTime > 0.0f)
	{
		reactRunTime -= Time::Delta();
		if (currAction != 5)
		{
			animator->Play(5);
			currAction = 5;
		}
		return;
	}

	UINT next = currAction;

	Vector3 position;
	transform->Position(&position);

	Vector3 focus;
	player->Focus(&focus);
	Vector3 dest = focus - position;
	float lengthSq = D3DXVec3LengthSq(&dest);

	Debug::Log->Show("length : " + to_string(lengthSq));
	Debug::Log->Show("attackRange : " + to_string(attackRange * attackRange));

	if (lengthSq <= attackRange * attackRange)
		NextAtteck(next, dest);
	else if (lengthSq <= detectionRange * detectionRange)
		NextRun(next, dest);
	else
		NextIdle(next);

	if (currAction != next)
	{
		animator->Play(next);
		currAction = next;
	}
}

// 애니메이션 이벤트
void EnemyInstance::OnNextAnimation(UINT next)
{
	if (currAction == 3)
		currAction = next;

	if (currAction == 4)
		currAction = next;

	if (next == 6 && bFall)
	{
		bLost = true;
		transform->Scale(0.0f, 0.0f, 0.0f);
	}

	if (next == 6)
	{
		bFall = true;
	}
}

void EnemyInstance::OnDamage()
{
	if (hp->HP() <= 0.0f)
	{
		attack->Stop();
	}
	else if (reactRunTime <= 0.0f && currAction != 4)
	{
		reactRunTime = reactTime;
	}
}

void EnemyInstance::NextAtteck(UINT & next, const Vector3 & dest)
{
	if (next == 4)
		return;

	float speedDelta = runSpeed * Time::Delta();

	Vector3 dest2 = dest;
	dest2.y = 0.0f;
	D3DXVec3Normalize(&dest2, &dest2);

	// 회전
	ratate->SetTarget(dest2);
	Quaternion q;
	transform->Rotation(&q);
	float rad;
	q = ratate->GetRotation(q, -transform->Forward(), speedDelta * turnSpeed, &rad);

	if (rad <= 0.007f)
		next = 0;
	else
	{
		next = 2;
		transform->Rotation(q);
	}


	if (attack->IsAttackAble())
	{
		attack->Play();
		next = 4;
	}
}

void EnemyInstance::NextRun(UINT & next, const Vector3 & dest)
{
	if (next != 3 && next != 2 && next != 4)
	{
		// 전투 중일 때 바로 달리기
		if (bWariness)
			next = 2;
		// 처음 전투에 진입
		else 
		{
			//UINT size = patrolPoints.size();
			//currPatrol = (currPatrol + size - 1) % size;
			next = 3;
		}

		bWariness = true;
	}

	if ((next == 3) || (next == 2))
	{
		float speedDelta = runSpeed * Time::Delta();

		Vector3 dest2 = dest;
		dest2.y = 0.0f;
		D3DXVec3Normalize(&dest2, &dest2);

		// 회전
		ratate->SetTarget(dest2);
		Quaternion q;
		transform->Rotation(&q);
		q = ratate->GetRotation(q, -transform->Forward(), speedDelta * turnSpeed);
		transform->Rotation(q);

		if (next == 2)
		{
			// 이동
			Vector3 position;
			transform->Position(&position);
			position += -transform->Forward() * speedDelta;
			transform->Position(position);
		}
	}
}

void EnemyInstance::NextIdle(UINT & next)
{
	if (next == 4) 
		return;

	if (next == 3)
	{
		// 전투 끝
		next = 0;
		bWariness = false;
		idleRunTime = 0;
		//currPatrol = (currPatrol + 1) % patrolPoints.size();
		return;
	}

	if (next == 0)
	{
		idleRunTime += Time::Delta();
		if (idleRunTime > 1.0f)
		{
			// 패트롤 시작
			next = 1;
		}
	}

	Vector3 position;
	transform->Position(&position);
	Vector3 focus = patrolPoints[currPatrol];
	Vector3 dest = focus - position;
	dest.y = 0.0f;
	float lengthSq = D3DXVec3LengthSq(&dest);

	if (lengthSq < 1.0f)
	{
		next = 0;
		idleRunTime = 0.0f;
		currPatrol = (currPatrol + 1) % patrolPoints.size();
		bWariness = false;
	}
	else
	{
		float speedDelta = 0.0f;
		if (next == 1)
			speedDelta = walkSpeed * Time::Delta();
		else if (next == 2)
			speedDelta = runSpeed * Time::Delta();

		// 순찰
		Vector3 dest2;
		D3DXVec3Normalize(&dest2, &(dest));
		ratate->SetTarget(dest2);

		Quaternion q;
		transform->Rotation(&q);
		q = ratate->GetRotation(q, -transform->Forward(), speedDelta * turnSpeed);
		transform->Rotation(q);

		position += -transform->Forward() * speedDelta;
		transform->Position(position);
	}


}
