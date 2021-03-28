#include "stdafx.h"
#include "MagicAttack.h"

#include "Component/AttackAnimation.h"
#include "Component/AttackSystem.h"
#include "Component/RotateSystem.h"

#include "Rendering/Camera/Main/IFocus.h"

MagicAttack::MagicAttack(IFocus* player, MeshInstancing* model)
	: player(player), sphere(model)
{
	rotateSystem = new RotateSystem();
	initTransform = new Transform();
}

MagicAttack::~MagicAttack()
{
	for (auto d : instances)
		SafeDelete(d);
	SafeDelete(initTransform);
	SafeDelete(rotateSystem);
}

AttackAnimation * MagicAttack::MakeAttackInstance(Transform * parent)
{
	Instance_M* i = new Instance_M(this, parent);
	instances.push_back(i);
	return i->Anim();
}

void MagicAttack::Update()
{
	list<Bullet>::iterator iter = bullets.begin();
	
	while (iter != bullets.end())
	{
		(*iter).LifeTime -= Time::Delta();
		if ((*iter).LifeTime <= 0.0f)
		{
			(*iter).System->EndAttack();
			(*iter).System->GetTransform()->UnLink();
			SafeDelete((*iter).System);
			SafeRelease((*iter).HitBox);
			SafeRelease((*iter).Model);
			bullets.erase(iter++);
			continue;
		}

		// 충돌되자 마자 삭제하면 실행 순서에 따라 메시지 보내지 못할 수도 있다
		UINT size;
		auto cs = (*iter).HitBox->GetReceived(&size);
		for (UINT i = 0; i < size; i++)
		{
			if (L"Magic" != cs[i]->Tag)
			{
				(*iter).LifeTime = 0.0f;
				break;
			}
		}

		Vector3 position;
		(*iter).Model->GetTransform()->Position(&position);

		Quaternion q;
		(*iter).Model->GetTransform()->Rotation(&q);
		{
			Vector3 focus;
			player->Focus(&focus);
			Vector3 target = focus - position;
			D3DXVec3Normalize(&target, &target);
			rotateSystem->SetDirection(target);

			Vector3 forward = (*iter).Model->GetTransform()->Forward();
			q = rotateSystem->GetRotation(q, forward, Time::Delta()* 1.5f);
		}
		(*iter).Model->GetTransform()->Rotation(q);
		position += (*iter).Model->GetTransform()->Forward() * 0.015f;
		(*iter).Model->GetTransform()->Position(position);

		Debug::Box->RenderBox((*iter).System->GetTransform());

		++iter;
	}

	for (Instance_M* i : instances)
		i->Anim()->Update();
}

void MagicAttack::FireBullet(const Vector3& point, const Quaternion& q)
{
	Bullet b;
	b.LifeTime = lifeTime;
	b.System = new AttackSystem();
	b.Model = sphere->AddInstance();
	b.HitBox = CollisionManager::Get()->CreateCollider();

	Vector3 focus;
	player->Focus(&focus);
	b.Model->GetTransform()->Position(point);
	b.Model->GetTransform()->Rotation(q);

	Matrix w;
	D3DXMatrixIdentity(&w);
	b.HitBox->GetTransform()->SetParent(b.Model->GetTransform()); 
	b.HitBox->GetTransform()->LocalWorld(w);
	b.HitBox->SetLayer(
		COLLIDER_LAYER_PLAYER |
		(COLLIDER_LAYER_DEFAULT &
		~COLLIDER_LAYER_CAMERA)
	);
	b.HitBox->Tag(L"Magic");
	b.HitBox->SetReceiver(true);

	b.System->GetTransform()->SetParent(b.Model->GetTransform());
	b.System->GetTransform()->LocalWorld(w);
	b.System->SetAttack(attack);
	b.System->SetTag(tag);

	b.System->StartAttack();
	bullets.push_back(b);
}


// --
// Instance_M
// --

MagicAttack::Instance_M::Instance_M(MagicAttack * init, Transform * parent)
	: init(init)
	, readyTime(init->readyTime)
	, delayTime(init->delayTime)
{
	anim = new AttackAnimation();
	anim->SetFuncStop(bind(&MagicAttack::Instance_M::Stop, this));
	anim->SetFuncAttackAble(bind(&MagicAttack::Instance_M::IsAttackAble, this));
	anim->SetFuncIsAttacking(bind(&MagicAttack::Instance_M::IsAttacking, this));
	anim->AddUnit(bind(&MagicAttack::Instance_M::Update, this, placeholders::_1));

	transform = new Transform();
	transform->SetParent(parent);
	Matrix w;
	init->InitTransform()->LocalWorld(&w);
	transform->LocalWorld(w);
}

MagicAttack::Instance_M::~Instance_M()
{
	SafeDelete(transform);
	SafeDelete(anim);
}

AttackAnimation * MagicAttack::Instance_M::Anim()
{
	return anim;
}

bool MagicAttack::Instance_M::Update(float runningTime)
{
	switch (state)
	{
	case MagicAttack::Instance_M::AttackState::None:
		if (runningTime == 0.0f)
		{
			// 공격 시작
			state = AttackState::Ready;
		}
		else
		{
			return true;
		}
		break;

	case MagicAttack::Instance_M::AttackState::Ready:
		if (runningTime >= readyTime)
		{
			Matrix w;
			transform->GlobalWorld(&w);
			//init->FireBullet(Vector3(w._41, w._42, w._43));
			Vector3 s, t;
			Quaternion r;
			D3DXMatrixDecompose(&s, &r, &t, &w);
			init->FireBullet(t, r);
			state = AttackState::Delay;
		}
		break;

	case MagicAttack::Instance_M::AttackState::Delay:
		if (runningTime >= delayTime)
		{
			state = AttackState::None;
			return true;
		}
		break;

	case MagicAttack::Instance_M::AttackState::Stop:  // 외부로 인한 종료
		if (runningTime == 0.0f)
		{
			state = AttackState::Ready;
		}
		else
		{
			state = AttackState::None;
			return true;
		}
		break;
	}
	return false;
}

bool MagicAttack::Instance_M::IsAttackAble()
{
	if (state == AttackState::None ||
		state == AttackState::Stop)
		return true;

	return false;
}

bool MagicAttack::Instance_M::IsAttacking()
{
	if (state == AttackState::Ready)
		return true;

	return false;
}

void MagicAttack::Instance_M::Stop()
{
	state = AttackState::Stop;
}
