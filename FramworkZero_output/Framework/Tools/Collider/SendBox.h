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

	UINT GetLayer() const { return collider->GetLayer(); }
	void SetLayer(UINT value) { collider->SetLayer(value); }

	// send massage
public:
	void SetTag(wstring value) { collider->Tag(value); }
	void SetSendMessageData(void* value) { message->Message = value; }
	void OnSendMessage();

	// Renewal cycle
public:
	void SetCycle(float value) { cycleTime = value; runningTime = 0.0f; }
	void UpdateCycle(float delta);

private:
	ColliderBox* collider;
	TriggerBoxMessageDesc* message;
	float cycleTime = 0.0f;
	float runningTime = 0.0f;
};