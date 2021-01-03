#include "Framework.h"
#include "ReceiveBox.h"

ReceiveBox::ReceiveBox(ColliderBox * collider)
	: collider(collider)
{
	collider->SetLayer(collider->GetLayer() | COLLIDER_LAYER_HITBOX);
	collider->SetReceiver(true);
}

ReceiveBox::~ReceiveBox()
{
}

void ReceiveBox::Update()
{
	received.clear();

	UINT size;
	const BoxMessageDesc*const* ms = collider->GetReceived(&size);

	for (UINT i = 0; i < size; i++)
	{
		if (CheckTag(ms[i]) == false)
			continue;
		ReceiveBoxMessage(ms[i]);
	}
}

bool ReceiveBox::CheckTag(const BoxMessageDesc * value)
{
	for (wstring& tag : receiveTags)
	{
		if (tag == value->Tag)
			return true;
	}
	return false;
}

void ReceiveBox::ReceiveBoxMessage(const BoxMessageDesc * value)
{
	auto m = (TriggerBoxMessageDesc*)value->Message;
	for (ReceiveMessageDesc& r : receiveDescs)
	{
		if (r.Id != value->Id) 
			continue;

		if (r.Time == m->SendTime)
			return;

		received.push_back({ value->Tag, m->Message });
		r.Time = m->SendTime;
		return;
	}

	received.push_back({ value->Tag, m->Message });
	receiveDescs.insert(receiveDescs.begin(), { value->Id, m->SendTime });
}
