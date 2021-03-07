#pragma once

#include "V2EnemyDesc.h"

class V2EnemyInstance
{
public:
	V2EnemyInstance(const V2EnemyInstanceDesc& desc);
	~V2EnemyInstance();


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

	V2EnemyClipNumGroup clipGroup;

private:
	class HPSystem* hp;  // ü��, ���� �޴� ����
	class FieldBehavior* behavior;
};