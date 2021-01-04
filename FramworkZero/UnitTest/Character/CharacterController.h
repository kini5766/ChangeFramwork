#pragma once

#include "Tools/Viewer/IFocus.h"

class CharacterController : public IFocus
{
public:
	CharacterController(Transform* transform, Animator* animator);
	~CharacterController();

public:
	void Update();

private:
	// IFocus을(를) 통해 상속됨
	virtual void Focus(Vector3 * outPosition) override;

private:
	Transform* transform;
	Animator* animator;
	class UserKeyState * userInput;
	float speed = 15.0f;
	UINT currState = 0;
};