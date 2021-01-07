#pragma once

#include "Tools/Viewer/IFocus.h"

class CharacterController : public IFocus
{
public:
	CharacterController(Transform* transform, Animator* animator);
	~CharacterController();

public:
	void Update();

public:
	void SetFuncAttack(function<void(void)> attack, function<bool(void)> isAble = []() { return true; });

private:
	// IFocus��(��) ���� ��ӵ�
	virtual void Focus(Vector3 * outPosition) override;

private:
	// �̺�Ʈ�Լ�
	void ClipEnd(UINT clip);

private:
	Transform* transform;
	Animator* animator;
	class UserKeyState * userInput;
	class RotateSystem* ratate;
	float speed = 15.0f;
	UINT currState = 0;
	UINT nextState = 0;
	bool bDelay;

	function<bool(void)> funcIsAttackAble = []() { return false; };
	function<void(void)> funcAttack;
};