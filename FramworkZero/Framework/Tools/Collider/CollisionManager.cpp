#include "Framework.h"


#pragma region Single tone

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

#pragma endregion


CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
	for (auto d : rays)
		SafeDelete(d);
	for (auto d : colliders)
		SafeDelete(d);
}

void CollisionManager::Update()
{
	UpdateInput();
	CheckCollision();
}

void CollisionManager::UpdateInput()
{
	// look
	list<RaycastPair*>::iterator iter = rays.begin();
	while (iter != rays.end())
	{
		if ((*iter)->Input->IsInput() == false)
		{
			++iter;
			continue;
		}

		if ((*iter)->Input->IsDestroy())
		{
			list<RaycastPair*>::iterator erase = iter++;
			SafeDelete(*erase);
			rays.erase(erase);
			continue;
		}

		(*iter)->Output->Ray = (*iter)->Input->GetRay();
		(*iter)->Output->LayerMask = (*iter)->Input->GetMask();
		(*iter)->Output->IsCollision = false;
		++iter;
	}

	for (ColliderBox* collider : colliders)
	{
		UINT layerMask = collider->GetMask();
		if (layerMask == COLLIDER_LAYER_NONE)
			continue;

		collider->UpdateBounding();
		collider->ClearReceived();
	}
	// unlook
}

void CollisionManager::CheckCollision()
{
	for (ColliderBox* collider : colliders)
	{
		UINT layerMask = collider->GetMask();
		if (layerMask == COLLIDER_LAYER_NONE)
			continue;

		for (RaycastPair* ray : rays)
		{
			if ((layerMask & ray->Output->LayerMask) == 0)
				continue;

			float temp;
			if (collider->GetCollider()->Intersection(
				ray->Output->Ray.Position,
				ray->Output->Ray.Direction,
				&temp
			))
			{
				if (ray->Output->IsCollision)
				{
					ray->Output->OutMinDistance = min(ray->Output->OutMinDistance, temp);
				}
				else
				{
					ray->Output->IsCollision = true;
					ray->Output->OutMinDistance = temp;
				}
			}
		}

		if (collider->IsReceiver() == false)
			continue;
		
		for (ColliderBox* send : colliders)
		{
			if ((layerMask & send->GetMask()) == 0)
				continue;

			if (collider->GetCollider()->Intersection(send->GetCollider()) == false)
				continue;

			// look
			send->SendMassageColliderBox(collider);
			// unlook
		}
	}
}


#pragma region Instance

ColliderBox * CollisionManager::CreateCollider()
{
	ColliderBox* instance = nullptr;

	if (junkColliders.size() == 0)
	{
		UINT index = colliders.size();
		instance = new ColliderBox(index);
		colliders.push_back(instance);
	}
	else
	{
		UINT index = junkColliders.back();
		instance = colliders[junkColliders.back()];
		junkColliders.pop_back();
	}
	instance->Recycle(idNext++);

	return instance;
}

void CollisionManager::ReleaseCollider(ColliderBox * value)
{
	junkColliders.push_back(value->Junk());
}

Raycast * CollisionManager::CreateRaycast(const Ray & ray, UINT layer)
{
	RaycastOutput* output = new RaycastOutput(ray);
	Raycast* result = new Raycast(output, layer);
	rays.push_back(new RaycastPair{ output, result });
	return result;
}

#pragma endregion

