#include "stdafx.h"
#include "Patrolling.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/PerceptionSystem.h"


Patrolling::Patrolling(const PatrollingDesc & input)
	: desc(input)
{
	loop = new FlowLoop();
	loop->SetJudgment(UpdateLoop());

	playLookAround = new FlowAction(PlayLookAround());
	waiter = new Waiter(desc.MakeWaiter());

	UINT size = desc.PatrolCount;
	moveToPoints.reserve(size);
	for (int i = size - 1; i >= 0; i--)
		moveToPoints.push_back(new PointMover(desc.MakeMover(i)));
	playWalk = new FlowAction(PlayWalk());

	lookAround = new FlowRoutine();

	patrolls.reserve(size);
	for (PointMover* mov : moveToPoints)
	{
		// �ѷ�����
		// 2. ���
		// 1. �ִϸ��̼� ���
		lookAround->Tesks()->push_back(waiter);
		lookAround->Tesks()->push_back(playLookAround);

		// ��Ʈ��
		// 2. �̵�
		// 1. �ִϸ��̼� ���
		FlowRoutine* pat = new FlowRoutine();
		patrolls.push_back(pat);
		pat->Tesks()->push_back(mov);
		pat->Tesks()->push_back(playWalk);

		// ����
		// 2. �ѷ�����
		// 1. ���� ��ó�� �ƴϸ� ������������
		loop->Tesks()->push_back(lookAround);
		loop->Tesks()->push_back(pat);
	}
}

Patrolling::~Patrolling()
{
	SafeDelete(loop);

	for (auto d : patrolls)
		SafeDelete(d);
	for (auto d : moveToPoints)
		SafeDelete(d);

	SafeDelete(waiter);
	SafeDelete(playWalk);
	SafeDelete(lookAround);
	SafeDelete(playLookAround);
}


void Patrolling::Call(const FutureAction * action)
{
	loop->Call(action);
}

void Patrolling::Update()
{
	loop->Update();
}

void Patrolling::Cancel()
{
	loop->Cancel();
}


Judgment Patrolling::UpdateLoop()
{
	return [=]() {
		desc.Perceptor->Update();
		if (desc.Perceptor->IsPerceived())
		{
			desc.FuncInRange();
			return false;
		}
		return true; 
	};
}

#include "Component/PointMoveSystem.h"
FutureAction Patrolling::PlayWalk()
{
	return [=]() {
		desc.Anim->Play(desc.ClipWalk);
		desc.MovingSystem->SetMoveSpeeder(desc.WalkSpeed);
	};
}

FutureAction Patrolling::PlayLookAround()
{
	return [=]() {
		desc.Anim->Play(desc.ClipLookAround);
	};
}

Judgment Patrolling::IsAround()
{
	return [=]() {
		return desc.MovingSystem->IsAround();
	};
}
