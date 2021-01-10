#pragma once

#include "stdafx.h"
#include "Tools/Viewer/IFocus.h"
#include "Objects/Model/ModelAnimation.h"
#include "Component/AttackAnimation.h"

struct EnemyDesc
{
	float HP = 100.0f;
	float RunSpeed = 6.0f;  // ���� �� �̵� �ӵ�
	float WalkSpeed = 3.0f;  // ���� �ӵ�
	float TurnSpeed = 1.0f;  // ȸ�� �ӵ�
	float DetectionRange = 30.0f;  // ��� ����
	float AttackRange = 5.0f;  // ���� ����
};

class IEnemy
{
public:
	virtual ~IEnemy() {}
	virtual ModelSkinnedInstancing* GetModel() = 0;
	virtual void BindAnimation(Animator*, ModelAnimation*) = 0;
	virtual AttackAnimation* MakeAttackInstance(Transform*) = 0;

	virtual const EnemyDesc* GetDesc() = 0;

	virtual void Update() {};
	virtual void Render() {};
};

// �ν��Ͻ� ���� ����
struct EnemyInstanceDesc
{
	IFocus* Player;
	Animator* Animator;  // �� �ִϸ�����
	Transform* Transform;  // �� ��ġ
	AttackAnimation* Attack;
	const vector<Vector3>* PatrolPoints;  // ���� ���
	const EnemyDesc* Desc;
};