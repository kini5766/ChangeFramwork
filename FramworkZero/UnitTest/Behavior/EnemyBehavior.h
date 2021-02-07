#pragma once

typedef function<bool()> IsAble;

class EnemyBehavior
{
public:
	EnemyBehavior();
	~EnemyBehavior();

private:
	bool inBattle;
	bool inRecognition;
	class BehaviorSelecter* selecter;
};

// ����
// ���� ���� �ƴ� ��
// �ν� ���� ���� ��
struct PatrolChecker
{
	bool* InBattle = nullptr;
	bool* InRecognition = nullptr;

	bool IsAble() const
	{
		return (*InBattle == false)
			&& (*InRecognition == false);
	};
};
/*
// �� �ν�
// ���� ���� �ƴ� ��
// �ν� ���� �� �� ��
struct Patrol
{
	int num;
	bool* InBattle;
	bool* InRecognition;
};

// �ٰ�����
// ���� ���� ��
// ���� ���� ���� �� ��
struct Patrol
{
	int num;
	bool* InBattle;
	bool* OutAttackFar;
};

// ������ �̵�
// ���� ���� ��
// ���� ���� �ȿ� ���� ��
struct Patrol
{
	int num;
	bool* InBattle;
	bool* OutAttackFar;
	bool* InAttackNear;
};

// ��������
// ���� ���� ��
// ���� ���� ���� ����� ��
struct Patrol
{
	int num;
	bool* InBattle;
	bool* InAttackNear;
};

// ����
// ���� ���� ��
// ���� ���� ���� ��
struct Patrol
{
	int num;
	bool* InBattle;
	bool* OutBattle;
};

// �ٰŸ� ��ų
// ���� ���� ��
// ��ų ��� ��� ���� �ƴ� ��
// ��ų ���� ���� ��

// ���Ÿ� ��ų
// ���� ���� ��
// ��ų ��� ��� ���� �ƴ� ��
// ��ų ���� ���� ��
struct Patrol
{
	int num;
	bool* InBattle;
	bool* IsSkillAble;
};
*/