#pragma once

#include "stdafx.h"
#include "Rendering/Camera/Main/IFocus.h"
#include "Rendering/Model/AnimationAdapter.h"
#include "Component/AttackAnimation.h"

struct EnemyDesc
{
	float HP = 100.0f;
	float RunSpeed = 6.0f;  // 전투 시 이동 속도
	float WalkSpeed = 3.0f;  // 순찰 속도
	float TurnSpeed = 1.0f;  // 회전 속도
	float DetectionRange = 30.0f;  // 경계 범위
	float AttackRange = 5.0f;  // 공격 범위
};

class IEnemy
{
public:
	virtual ~IEnemy() {}
	virtual ModelInstancing* GetModel() = 0;
	virtual void BindAnimation(Animator*, AnimationAdapter*) = 0;
	virtual AttackAnimation* MakeAttackInstance(Transform*) = 0;

	virtual const EnemyDesc* GetDesc() = 0;

	virtual void Update() {};
	virtual void Render() {};
};

// 인스턴스 생성 인자
struct EnemyInstanceDesc
{
	IFocus* Player;
	Animator* Animator;  // 모델 애니메이터
	Transform* Transform;  // 모델 위치
	AttackAnimation* Attack;
	const vector<Vector3>* PatrolPoints;  // 순찰 경로
	const EnemyDesc* Desc;
};