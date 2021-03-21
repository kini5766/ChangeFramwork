#pragma once

struct PerceptionInput
{
	// 자신 위치
	Vector3* MinePosition;

	// 적 위치
	Vector3* FocusPosition;

	// 적 감지 범위 (제곱)
	float SightRangeSq = 0.0f;
	// 적 놓침 범위 (제곱)
	float LoseRangeSq = 0.0f;
};

// 적 감지 시스템
class PerceptionSystem
{
public:
	PerceptionSystem(const PerceptionInput& input);
	~PerceptionSystem();

public:
	void Update();

	bool IsPerceived();
	float GetDistanceSq() { return distanceSq; }
	Vector3* GetFocus() { return desc.FocusPosition; }
	Vector3 GetDest() { return dest; }

private:
	PerceptionInput desc;
	bool isPerceived = false;
	float distanceSq = 0.0f;
	Vector3 dest;
};
