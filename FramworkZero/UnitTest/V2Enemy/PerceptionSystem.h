#pragma once

struct PerceptionInput
{
	// �ڽ� ��ġ
	Vector3* MinePosition;

	// �� ��ġ
	Vector3* FocusPosition;

	// �� ���� ���� (����)
	float SightRangeSq = 0.0f;
	// �� ��ħ ���� (����)
	float LoseRangeSq = 0.0f;
};

// �� ���� �ý���
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
