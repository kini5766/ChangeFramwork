#include "stdafx.h"
#include "StrafeAround.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "Component/PointMoveSystem.h"
#include "V2Enemy/PerceptionSystem.h"


StrafeAround::StrafeAround(const StrafeAroundDesc & input)
	: desc(input)
	, funcCase(bind(&StrafeAround::UpdateCase, this))
	, funcEndCase(bind(&StrafeAround::EndCase, this))
	, funcPositioning(bind(&StrafeAround::UpdatePositioning, this))
{
	switching = new FlowSwitching();


	playIdle = new FlowAction([=]() {
		desc.Anim->PlayUpdate(desc.ClipIdle);
	});
	playRun = new FlowAction([=]() {
		desc.Anim->PlayUpdate(desc.ClipRun);
		desc.MovingSystem->SetMoveSpeeder(desc.RunSpeed);
	});

	WaiterDesc makeWaiter;
	makeWaiter.Time = 1.0f;
	PointMoverDesc makeMover;
	makeMover.MovingSystem = desc.MovingSystem;
	FollowerDesc makeFollower;
	makeFollower.FuncGetFocus = [=]() { return desc.Perceptor->GetFocus(); };
	makeFollower.MovingSystem = desc.MovingSystem;

	waiter = new Waiter(makeWaiter);
	moveto = new PointMover(makeMover);
	follower = new Follower(makeFollower);

	wait = new FlowRoutine();
	move = new FlowRoutine();
	positioning = new FlowAction(funcPositioning);

	prowlRoutine = new FlowRoutine();
	followRoutine = new FlowRoutine();

	// ����
	// 2. ���
	// 1. �ִϸ��̼� ���
	wait->Tesks()->push_back(waiter);
	wait->Tesks()->push_back(playIdle);

	// ����
	// 2. ��ȸ
	// 1. ���
	move->Tesks()->push_back(moveto);
	move->Tesks()->push_back(playRun);


	// ����
	// 3. ���
	// 2. �̵�
	// 1. �̵��� ��ġ ���ϱ�
	prowlRoutine->Tesks()->push_back(wait);
	prowlRoutine->Tesks()->push_back(move);
	prowlRoutine->Tesks()->push_back(positioning);

	// ����
	// 2. ����
	// 1. ���
	followRoutine->Tesks()->push_back(follower);
	followRoutine->Tesks()->push_back(playRun);


	// ĳ�̽�
	// -1 : ���� ���� ���
	// 0 : �ֺ� ��ȸ
	// 1 : ���󰡱�
	switching->SetFuncCase(funcCase);
	switching->Cases()->push_back(prowlRoutine);
	switching->Cases()->push_back(followRoutine);
}

StrafeAround::~StrafeAround()
{
	SafeDelete(switching);

	SafeDelete(followRoutine);
	SafeDelete(prowlRoutine);

	SafeDelete(wait);
	SafeDelete(move);

	SafeDelete(waiter);
	SafeDelete(follower);
	SafeDelete(moveto);

	SafeDelete(playIdle);
	SafeDelete(playRun);
}

void StrafeAround::Call(const FutureAction * action)
{
	result.SetAction(action);
	switching->Call(&funcEndCase);
}

void StrafeAround::Update()
{
	switching->Update();
}

void StrafeAround::Hold()
{
	result.Clear();
	switching->Hold();
}


int StrafeAround::UpdateCase()
{
	bool next = desc.Perceptor->GetDistanceSq() <= desc.ApproachRangeSq;
	return (next) ? 0 : 1;
}

void StrafeAround::EndCase()
{
	result.OnAction();
}


void StrafeAround::UpdatePositioning()
{
	// �ֺ� �ɵ���
	Vector3 d = desc.Perceptor->GetDest();

	Vector3 dc(0, 1, 0);
	D3DXVec3Cross(&dc, &d, &dc);


	// -45�� ~ 45��
	float r = Math::Random(-Math::PI * 0.25f, Math::PI * 0.25f);
	//float r = Math::Random(Math::PI * 0.16f, Math::PI * 0.32f);
	Vector2 axis;
	axis.x = -cosf(r);
	axis.y = -sinf(r);

	Vector3 forward = d * axis.x;
	Vector3 right = dc * axis.y;
	Vector3 direction = forward + right;

	D3DXVec3Normalize(&direction, &direction);
	direction *= Math::Random(desc.MinRange, desc.MinRange * 2.0f);

	Vector3 focus = desc.Perceptor->GetFocus();
	moveto->GetDesc()->Point = focus + direction;

	waiter->GetDesc()->Time = Math::Random(0.125f, 0.25f);
}

