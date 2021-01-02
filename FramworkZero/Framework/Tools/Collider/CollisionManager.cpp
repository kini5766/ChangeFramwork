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
	for (auto d : sendBoxes)
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

	for (ReceiveBox* collider : colliders)
	{
		UINT layerMask = collider->GetMask();
		if (layerMask == COLLIDER_LAYER_NONE)
			continue;

		collider->UpdateBounding();
	}

	for (SendBox* send : sendBoxes)
	{
		UINT layerMask = send->GetMask();
		if (layerMask == COLLIDER_LAYER_NONE)
			continue;

		send->UpdateBounding();
	}
}

void CollisionManager::CheckCollision()
{
	for (ReceiveBox* collider : colliders)
	{
		collider->ClearMassage();
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
				ray->Output->IsCollision = true;
				ray->Output->OutMinDistance = temp;
			}
		}

		for (SendBox* send : sendBoxes)
		{
			if ((layerMask & send->GetMask()) == 0)
				continue;
			if (collider->GetCollider()->Intersection(send->GetCollider()))
				collider->Send(send->GetSendMessage());
		}
	}
}


#pragma region Instance

ReceiveBox * CollisionManager::CreateCollider()
{
	ReceiveBox* instance = nullptr;

	if (junkColliders.size() == 0)
	{
		UINT index = colliders.size();
		instance = new ReceiveBox(index);
		colliders.push_back(instance);
	}
	else
	{
		UINT index = junkColliders.back();
		instance = colliders[junkColliders.back()];
		junkColliders.pop_back();
	}
	instance->Recycle();

	return instance;
}

void CollisionManager::ReleaseCollider(ReceiveBox * value)
{
	junkColliders.push_back(value->Junk());
}

SendBox * CollisionManager::CreateSendBox()
{
	SendBox* instance = nullptr;

	if (junkSendBoxes.size() == 0)
	{
		UINT index = sendBoxes.size();
		instance = new SendBox(index);
		sendBoxes.push_back(instance);
	}
	else
	{
		UINT index = junkSendBoxes.back();
		instance = sendBoxes[index];
		junkSendBoxes.pop_back();
	}
	instance->Recycle();

	return instance;
}

void CollisionManager::ReleaseSendBox(SendBox * value)
{
	junkSendBoxes.push_back(value->Junk());
}

Raycast * CollisionManager::CreateRaycast(const Ray & ray, UINT layer)
{
	RaycastOutput* output = new RaycastOutput(ray);
	Raycast* result = new Raycast(output, layer);
	rays.push_back(new RaycastPair{ output, result });
	return result;
}

#pragma endregion

