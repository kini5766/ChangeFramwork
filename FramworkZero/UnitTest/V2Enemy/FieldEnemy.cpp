#include "stdafx.h"
#include "FieldEnemy.h"

#include "Component/HPSystem.h"
#include "EnemyBehaviors/FieldBehavior.h"

FieldEnemy::FieldEnemy(const FieldEnemyInput & desc)
	: oriStatus(desc.Desc->Status)
	, clipGroup(desc.Desc->ClipGroup)
	, player(desc.Desc->Player)

	, transform(desc.Model->GetTransform())
	, animator(desc.Model->GetAnimator())

	, patrolPoints(*desc.PatrolPoints)
{
	// -- Transform -- //
	transform->LocalWorld(*desc.localWorld);


	// -- HPSystem -- //
	hp = new HPSystem(oriStatus.HP);
	hp->GetHpbar()->SetParent(transform);
	hp->GetHpbar()->Position(0, 180.0f, 0);
	hp->GetHurtbox()->GetTransform()->Scale(2.25f, 5.40f, 2.25f);
	hp->GetHurtbox()->GetTransform()->SetParent(transform);
	//hp->GetHurtbox()->GetTransform()->Scale(75.0f, 180.0f, 75.0f);
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


	// -- FieldBehavior -- //
	FieldBehaviorInput input;
	input.Trans = transform;
	input.Anim = animator;
	input.ClipWalk = clipGroup.ClipWalk;
	input.ClipLookAround = clipGroup.ClipLookAround;
	input.ClipInSight = clipGroup.ClipInSight;
	input.PatrolPoints = patrolPoints.data();
	input.PatrolCount = patrolPoints.size();
	behavior = new FieldBehavior(input);
	behavior->SetFocus(player);
}

FieldEnemy::~FieldEnemy()
{
	SafeDelete(behavior);
	SafeDelete(hp);
}

void FieldEnemy::Update()
{
	hp->Update();
	behavior->Update();
}

void FieldEnemy::PostRender()
{
	hp->PostRender();
}


void FieldEnemy::OnDamage()
{
	if (hp->HP() <= 0.0f)
	{
		// On Dead
	}
	else
	{
		// On Hit Reaction
	}
}
