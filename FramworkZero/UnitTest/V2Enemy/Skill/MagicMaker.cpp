#include "stdafx.h"
#include "MagicMaker.h"

#include "Component/AttackSystem.h"
#include "Component/RotateSystem.h"
#include "Rendering/Camera/Main/IFocus.h"

MagicMaker::MagicMaker(MeshInstancing* model)
	: model(model)
{
	rotateSystem = new RotateSystem();
	initTransform = new Transform();
}

MagicMaker::~MagicMaker()
{
	SafeDelete(initTransform);
	SafeDelete(rotateSystem);
}

void MagicMaker::Update()
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
			(*iter).Focus->Focus(&focus);
			Vector3 target = focus - position;
			D3DXVec3Normalize(&target, &target);
			rotateSystem->SetDirection(target);

			Vector3 forward = (*iter).Model->GetTransform()->Forward();
			q = rotateSystem->GetRotation(q, forward, Time::Delta() * 1.5f);
		}
		(*iter).Model->GetTransform()->Rotation(q);
		position += (*iter).Model->GetTransform()->Forward() * 0.015f;
		(*iter).Model->GetTransform()->Position(position);

		Debug::Box->RenderBox((*iter).System->GetTransform());

		++iter;
	}
}

void MagicMaker::AddBullet(const MagicBulletInput& input)
{
	Bullet b;
	b.Focus = input.Focus;
	b.LifeTime = lifeTime;
	b.System = new AttackSystem();
	b.Model = model->AddInstance();
	b.HitBox = CollisionManager::Get()->CreateCollider();

	b.Model->GetTransform()->Position(input.Point);
	b.Model->GetTransform()->Rotation(input.Rotation);

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
	b.System->SetAttack(input.Attack);
	b.System->SetTag(input.Tag);

	b.System->StartAttack();
	bullets.push_back(b);

}
