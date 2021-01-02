#include "Framework.h"
#include "SendBox.h"

SendBox::SendBox(UINT instanceId)
	: instanceId(instanceId)
{
	collider = new Collider();
	message = new SendBoxMessage();
	message->Id = instanceId;
	message->SendTime = 0.0f;
}

SendBox::~SendBox()
{
	SafeDelete(message);
	SafeDelete(collider);
}

void SendBox::Release()
{
	CollisionManager::Get()->ReleaseSendBox(this);
}

UINT SendBox::Junk()
{
	bActive = false;
	layer = COLLIDER_LAYER_NONE;
	Transform* t = collider->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	message->Message = nullptr;
	message->SendTime = 0.0f;

	return instanceId;
}

void SendBox::Recycle()
{
	Matrix m;
	D3DXMatrixIdentity(&m);
	collider->GetTransform()->LossyWorld(m);

	layer = COLLIDER_LAYER_DEFAULT;
	bActive = true;
}

SendBoxMessage * SendBox::GetSendMessage() const
{ 
	if (message->Message != nullptr)
		return message;
	else return nullptr;
}
