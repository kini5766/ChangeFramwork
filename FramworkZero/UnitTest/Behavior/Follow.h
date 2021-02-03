#pragma once

struct FollowDesc
{
	Vector3* Dest;

	Transform* Target;
	class RotateSystem* Ratate;  // 회전 보간
	UINT* AnimNaxt;
	float* RunSpeed;
	float* TurnSpeed;
};

class Follow
{
public:
	Follow(const FollowDesc& desc);
	~Follow();

public:
	void Update();

private:
	FollowDesc* desc;
};