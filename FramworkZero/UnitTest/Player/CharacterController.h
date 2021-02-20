#pragma once

#include "Rendering/Camera/Main/IFocus.h"

class CharacterController : public IFocus
{
public:
	CharacterController(Transform* transform, Animator* animator);
	~CharacterController();

public:
	void Update();

public:
	void Fall();
	void Riseup();
	void SetFuncAttack(class AttackAnimation* value);
	void SetFuncLost(function<void(void)> value) { funcLost = value; }

private:
	// IFocus을(를) 통해 상속됨
	virtual void Focus(Vector3 * outPosition) override;

private:
	// 이벤트함수
	void ClipEnd(UINT clip);

private:
	Transform* transform;
	Animator* animator;
	class UserKeyState * userInput;
	class RotateSystem* ratate;
	float speed = 15.0f;
	// 0:Idle, 1:Walk, 2:Run, 3:Slash, 4:Fall
	UINT currState = 0;
	UINT nextState = 0;
	bool bDelay = false;

	class AttackAnimation* attack;
	function<void(void)> funcLost = []() {};
};