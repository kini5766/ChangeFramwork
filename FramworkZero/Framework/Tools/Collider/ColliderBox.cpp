#include "Framework.h"
#include "ColliderBox.h"

ColliderBox::ColliderBox(UINT instanceId)
	: instanceId(instanceId)
{
	collider = new Collider();
	message = new BoxMessageDesc();
	message->Id = instanceId;
	//message->Other = collider;
	message->Other = this;
}

ColliderBox::~ColliderBox()
{
	SafeDelete(message);
	SafeDelete(collider);
}

const BoxMessageDesc *const* const ColliderBox::GetReceived(UINT * outCount)
{
	*outCount = receiveds.size();
	return receiveds.data();
}


void ColliderBox::Release()
{
	CollisionManager::Get()->ReleaseCollider(this);
}

UINT ColliderBox::Junk()
{
	bActive = false;
	layer = COLLIDER_LAYER_NONE;
	Transform* t = collider->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	receiveds.clear();

	message->Other = nullptr;
	message->Message = nullptr;

	bReceiver = false;

	return instanceId;
}

void ColliderBox::Recycle(int colliderId)
{
	Matrix m;
	D3DXMatrixIdentity(&m);
	collider->GetTransform()->LossyWorld(m);

	layer = COLLIDER_LAYER_DEFAULT;
	bActive = true;

	this->colliderId = colliderId;
	message->Id = colliderId;

	//message->Other = collider;
	message->Other = this;
}


void ColliderBox::SendMassageColliderBox(ColliderBox * other)
{
	other->Send(message);
}
