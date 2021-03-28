#include "stdafx.h"
#include "FieldEnemy.h"

#include "Component/HPSystem.h"
#include "V2Enemy/Skill/IEnemySkill.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/PerceptionSystem.h"
#include "Component/PointMoveSystem.h"
#include "EnemyBehaviors/Patrolling.h"
#include "EnemyBehaviors/CombatPosture.h"

FieldEnemy::FieldEnemy(const FieldEnemyInput & desc)
	: oriStatus(desc.Status)
	, player(desc.Player)
	, transform(desc.Model->GetTransform())
	, animator(desc.Model->GetAnimator())
	, patrolPoints(*desc.PatrolPoints)

	, onLost(bind(&FieldEnemy::OnLost, this))
{
	status = oriStatus;

	// -- Transform -- //
	transform->LocalWorld(*desc.localWorld);


	// -- HPSystem -- //
	hp = new HPSystem(oriStatus.HP);
	hp->GetHpbar()->SetParent(transform);
	hp->GetHpbar()->Position(0, 180.0f, 0);
	hp->GetHurtbox()->GetTransform()->Scale(2.25f, 5.40f, 2.25f);
	hp->GetHurtbox()->GetTransform()->SetParent(transform);
	hp->GetHurtbox()->GetTransform()->Position(0.0f, 90.0f, 0.0f);
	hp->GetHurtbox()->GetTransform()->RotationEuler(
		EulerAngle::Degree(0.0f, 0.0f, 0.0f));
	hp->GetHurtbox()->SetLayer(
		COLLIDER_LAYER_ENEMY |
		COLLIDER_LAYER_HITBOX
	);
	hp->GetHurtbox()->Tag(L"Enemy");
	hp->AddReceiveTag(L"PlayerAttack");
	hp->SetFuncDamage(bind(&FieldEnemy::OnDamage, this));


	// -- Field Behavior의 값 업데이터들 -- //
	PerceptionInput inputED;
	inputED.FuncGetMine = [=]() {
		Vector3 out;
		transform->Position(&out);
		return out;
	};
	inputED.FuncGetFocus = [=]() {
		Vector3 out;
		player->Focus(&out);
		return out;
	};
	inputED.SightRangeSq = status.SightRangeSq;
	inputED.LoseRangeSq = status.LoseRangeSq;

	perceptor = new PerceptionSystem(inputED);
	movingSystem = new PointMoveSystem(transform);


	// -- Field Behavior -- //
	PatrollingDesc makePatroll;
	makePatroll.Anim = animator;
	makePatroll.ClipWalk = desc.ClipGroup.ClipWalk;
	makePatroll.ClipLookAround = desc.ClipGroup.ClipLookAround;
	makePatroll.PatrolPoints = patrolPoints.data();
	makePatroll.PatrolCount = patrolPoints.size();
	makePatroll.MovingSystem = movingSystem;
	makePatroll.WalkSpeed = &status.WalkSpeed;
	makePatroll.LookAroundTime = 1.0f;
	makePatroll.Perceptor = perceptor;
	makePatroll.FuncInRange = bind(&FieldEnemy::InRange, this);
	CombatPostureDesc makeCombat;
	makeCombat.StrafeAroundDesc.Anim = animator;
	makeCombat.StrafeAroundDesc.ApproachRangeSq = status.ApproachRangeSq;
	makeCombat.StrafeAroundDesc.MinRange = status.MinRange;
	makeCombat.StrafeAroundDesc.ClipIdle = desc.ClipGroup.ClipIdle;
	makeCombat.StrafeAroundDesc.ClipWalk = desc.ClipGroup.ClipWalk;
	makeCombat.StrafeAroundDesc.ClipRun = desc.ClipGroup.ClipRun;
	makeCombat.StrafeAroundDesc.MovingSystem = movingSystem;
	makeCombat.StrafeAroundDesc.Perceptor = perceptor;
	makeCombat.StrafeAroundDesc.RunSpeed = &status.RunSpeed;
	makeCombat.StrafeAroundDesc.WalkSpeed = &status.WalkSpeed;
	makeCombat.Skill = desc.Skill;

	ClipPlayerDesc makeDead;
	makeDead.Anim = animator;
	makeDead.Clip = desc.ClipGroup.ClipDead;
	ClipPlayerDesc makeHitted;
	makeHitted.Anim = animator;
	makeHitted.Clip = desc.ClipGroup.ClipHitted;
	ClipPlayerDesc makeInSight;
	makeInSight.Anim = animator;
	makeInSight.Clip = desc.ClipGroup.ClipInSight;
	PointMoverDesc makeComeback;
	makeComeback.MovingSystem = movingSystem;
	FutureAction makePlayRun = [=]() {
		animator->PlayUpdate(desc.ClipGroup.ClipRun);
		movingSystem->SetMoveSpeeder(&status.RunSpeed);
	};


	pat = new Patrolling(makePatroll);
	combat = new CombatPosture(makeCombat);

	clipDead = new ClipOncePlayer(makeDead);
	clipHitted = new ClipOncePlayer(makeHitted);
	clipInSight = new ClipOncePlayer(makeInSight);

	moveto = new PointMover(makeComeback);
	playRun = new FlowAction(makePlayRun);
	recall = new FlowRoutine();

	// 순서
	// 2. 이동
	// 1. 애니메이션
	recall->Tesks()->push_back(moveto);
	recall->Tesks()->push_back(playRun);

	reader = new FlowReader();
	reader->PushBack(pat);
	reader->Call();

}

FieldEnemy::~FieldEnemy()
{
	SafeDelete(reader);

	SafeDelete(playRun);
	SafeDelete(moveto);
	SafeDelete(recall);
	SafeDelete(combat);
	SafeDelete(pat);
	SafeDelete(clipInSight);
	SafeDelete(clipHitted);
	SafeDelete(clipDead);

	SafeDelete(movingSystem);
	SafeDelete(perceptor);

	SafeDelete(hp);
}

void FieldEnemy::Update()
{
	if (bLost) return;

	hp->Update();
	reader->Update();
}

void FieldEnemy::PostRender()
{
	if (bLost) return;

	hp->PostRender();
}

// 공격 받았을 때
void FieldEnemy::OnDamage()
{
	if (bDead) return;

	if (hp->HP() <= 0.0f)
	{
		// 죽음
		reader->Reset();
		reader->PushBack(clipDead);
		reader->Call(&onLost);

		bDead = true;
	}
	else
	{
		// 맞음
		reader->PushBack(clipHitted);
		reader->HoldBackNext();
	}
}

// 죽어서 사라짐
void FieldEnemy::OnLost()
{
	bLost = true;
	transform->Scale(0.0f, 0.0f, 0.0f);
}

// 적을 감지했을 때
void FieldEnemy::InRange()
{
	// 지금 위치를 복귀 위치로
	movingSystem->GetTransform()->Position(&moveto->GetDesc()->Point);

	// 실행 순서 (스택)
	// 4. 순찰
	// 3. 복귀
	// 2. 전투태세
	// 1. 적확인

	reader->PushBack(pat);
	reader->PushBack(recall);
	reader->PushBack(combat);
	reader->PushBack(clipInSight);
}
