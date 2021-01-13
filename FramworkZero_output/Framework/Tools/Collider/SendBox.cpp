#include "Framework.h"
#include "SendBox.h"

SendBox::SendBox(ColliderBox * collider)
	: collider(collider)
{
	message = new TriggerBoxMessageDesc();
	collider->Tag(COLLIDER_TAG_SENDBOX);
	
	collider->SetLayer(COLLIDER_LAYER_HITBOX);
	collider->SetSendMessageData(message);
	collider->SetReceiver(false);
}

SendBox::~SendBox()
{
	SafeRelease(collider);
	SafeDelete(message);
}

void SendBox::OnSendMessage()
{ 
	message->SendTime = Time::Get()->Running();
}

void SendBox::UpdateCycle(float delta)
{
	runningTime += delta;

	if (cycleTime <= runningTime)
	{
		OnSendMessage();
		runningTime -= cycleTime;
	}
}
