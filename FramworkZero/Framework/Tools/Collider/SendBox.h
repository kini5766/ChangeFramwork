#pragma once

struct TriggerBoxMessageDesc
{
	float SendTime;
	void* Message;
};

class SendBox
{
public:
	SendBox(ColliderBox* collider);
	~SendBox();

	// Getter, Setter
public:
	Transform* GetTransform() { return collider->GetTransform(); }

	bool IsActiveSelf() const { return collider->IsActiveSelf(); }
	void SetActiveSelf(bool value) { collider->SetActiveSelf(value); }

	UINT GetMask() const { return collider->GetMask(); }
	void SetLayer(UINT value) { collider->SetLayer(value); }

	// send massage
public:
	void SetTag(wstring value) { collider->Tag(value); }
	void SetSendMessageData(void* value) { message->Message = value; }
	void OnSendMessage();

private:
	ColliderBox* collider;
	TriggerBoxMessageDesc* message;
};