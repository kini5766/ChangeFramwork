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

// 순찰
// 전투 중이 아닐 때
// 인식 범위 밖일 때
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
// 적 인식
// 전투 중이 아닐 때
// 인식 범위 안 일 때
struct Patrol
{
	int num;
	bool* InBattle;
	bool* InRecognition;
};

// 다가가기
// 전투 중일 때
// 공격 범위 보다 멀 때
struct Patrol
{
	int num;
	bool* InBattle;
	bool* OutAttackFar;
};

// 옆으로 이동
// 전투 중일 때
// 공격 범위 안에 있을 떄
struct Patrol
{
	int num;
	bool* InBattle;
	bool* OutAttackFar;
	bool* InAttackNear;
};

// 떨어지기
// 전투 중일 때
// 공격 범위 보다 가까울 때
struct Patrol
{
	int num;
	bool* InBattle;
	bool* InAttackNear;
};

// 복귀
// 전투 중일 때
// 전투 범위 밖일 때
struct Patrol
{
	int num;
	bool* InBattle;
	bool* OutBattle;
};

// 근거리 스킬
// 전투 중일 때
// 스킬 사용 대기 중이 아닐 때
// 스킬 범위 안일 때

// 원거리 스킬
// 전투 중일 때
// 스킬 사용 대기 중이 아닐 때
// 스킬 범위 안일 때
struct Patrol
{
	int num;
	bool* InBattle;
	bool* IsSkillAble;
};
*/