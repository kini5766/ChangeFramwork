#include "Framework.h"
#include "ReceiveBox.h"

ReceiveBox::ReceiveBox(UINT instanceId)
	: instanceId(instanceId)
{
	collider = new Collider();
}

ReceiveBox::~ReceiveBox()
{
	SafeDelete(collider);
}

void ReceiveBox::ClearMassage()
{
	findIndex = 0;
	sends.clear();
}

void ReceiveBox::Send(SendBoxMessage * message)
{
	if (message == nullptr) return;

	bool bfound = false;
	for (ReceiveBoxMessage& m : receives)
	{
		if (m.SendId == message->Id)
		{
			if (m.SendActiveTime != message->SendTime)
			{
				sends.push_back(message);
				m.SendActiveTime = message->SendTime;
			}
			bfound = true;
			break;
		}
	}
	if (bfound == false)
	{
		sends.insert(sends.begin(), message);
		receives.push_back({ message->Id, message->SendTime });
	}
}

void ReceiveBox::Release()
{
	CollisionManager::Get()->ReleaseCollider(this);
}

UINT ReceiveBox::Junk()
{
	bActive = false;
	layer = COLLIDER_LAYER_NONE;
	Transform* t = collider->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	ClearMassage();

	return instanceId;
}

void ReceiveBox::Recycle()
{
	Matrix m;
	D3DXMatrixIdentity(&m);
	collider->GetTransform()->LossyWorld(m);

	layer = COLLIDER_LAYER_DEFAULT;
	bActive = true;
}
