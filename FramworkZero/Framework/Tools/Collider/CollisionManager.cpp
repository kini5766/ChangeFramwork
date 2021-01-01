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
	for (auto d : cameraRays)
		SafeDelete(d);
	for (auto d : colliders)
		SafeDelete(d);
}

void CollisionManager::Update()
{
	for (Collider* collider : colliders)
	{
		UINT layerMask = collider->GetMask();
		if (layerMask == Collider::COLLIDER_LAYER_NONE)
			continue;

		collider->UpdateBounding();
	}
	UpdateInput();
	CheckCollision();
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

Raycast * CollisionManager::CreateCameraRaycast(const Ray & ray)
{
	RaycastOutput* output = new RaycastOutput(ray);
	Raycast* result = new Raycast(output);
	cameraRays.push_back(new RaycastPair{ output, result });
	return result;
}

void CollisionManager::UpdateInput()
{
	list<RaycastPair*>::iterator iter = cameraRays.begin();
	while (iter != cameraRays.end())
	{
		if ((*iter)->Input->IsInput() == false)
		{
			++iter;
			continue;
		}

		if ((*iter)->Input->IsDestroy())
		{
			list<RaycastPair*>::iterator erase = iter++;
			cameraRays.erase(erase);
			continue;
		}

		(*iter)->Output->Ray = (*iter)->Input->GetRay();
		(*iter)->Output->IsActive = (*iter)->Input->IsActive();
		(*iter)->Output->IsCollision = false;
		++iter;
	}
}

void CollisionManager::CheckCollision()
{
	CollisionManager::Get()->GetColliders(&colliders);

	for (Collider* collider : colliders)
	{
		UINT layerMask = collider->GetMask();
		if (layerMask == Collider::COLLIDER_LAYER_NONE)
			continue;

		if ((collider->GetMask() & Collider::COLLIDER_LAYER_CAMERA) != 0)
		{
			for (RaycastPair* ray : cameraRays)
			{
				if (ray->Output->IsActive = false) continue;
				if ((collider->GetMask() & Collider::COLLIDER_LAYER_CAMERA) != 0)
				{
					float temp;
					if (collider->Intersection(
						ray->Output->Ray.Position,
						ray->Output->Ray.Direction,
						&temp
					))
					{
						ray->Output->IsCollision = true;
						ray->Output->OutMinDistance = temp;
					}
				}
			}
		}
	}
}
