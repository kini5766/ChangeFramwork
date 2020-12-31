#include "Framework.h"

CollisionManager* CollisionManager::instance = nullptr;

void CollisionManager::Create()
{
	assert(instance == nullptr);
	instance = new CollisionManager();
}

void CollisionManager::Delete()
{
	SafeDelete(instance);
}

CollisionManager * CollisionManager::Get()
{
	return instance;
}

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
	for (auto d : colliders)
		SafeDelete(d);
}


Collider * CollisionManager::CreateCollider()
{
	Collider* instance = nullptr;

	if (junkInstances.size() == 0)
	{
		UINT index = colliders.size();

		instance = new Collider(index);
		colliders.push_back(instance);
	}
	else
	{
		UINT index = junkInstances.back();

		instance = colliders[index];

		Matrix m;
		D3DXMatrixIdentity(&m);
		instance->GetTransform()->LossyWorld(m);

		junkInstances.pop_back();
	}

	instance->SetLayer(Collider::COLLIDER_LAYER_ALL);
	instance->SetActiveSelf(true);

	return instance;
}

void CollisionManager::ReleseCollider(Collider* value)
{
	value->SetActiveSelf(false);
	value->SetLayer(Collider::COLLIDER_LAYER_ALL);
	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	junkInstances.push_back(value->Id());
}
