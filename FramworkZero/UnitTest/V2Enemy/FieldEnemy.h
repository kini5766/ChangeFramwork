#pragma once

#include "V2EnemyDesc.h"

class FieldEnemy
{
public:
	FieldEnemy(const FieldEnemyInput& desc);
	~FieldEnemy();


public:
	void Update();
	void PostRender();

private:
	void OnDamage();


private:
	V2EnemyStatus oriStatus;  // ���� �ɷ�ġ
	class IFocus* player;  // ���� ��� (��ġ)
	Transform* transform;  // �� ��ġ
	Animator* animator;  // �� �ִϸ�����
	vector<Vector3> patrolPoints;  // ���� ���

	FieldEnemyClipNumGroup clipGroup;

private:
	class HPSystem* hp;  // ü��, ���� �޴� ����
	class FieldBehavior* behavior;
};