#pragma once

struct SendBoxMessage
{
	int Id;  // 보내는 이
	wstring Tag;  // 받는 이
	float SendTime;  // 보낸 시각
	void* Message;
};

class SendBox
{
public:
	SendBox(UINT instanceId);
	~SendBox();

	// Getter, Setter
public:
	bool IsActiveSelf() const { return bActive; }
	void SetActiveSelf(bool value) { bActive = value; }

	void SetLayer(UINT value) { layer = value; }
	UINT GetMask() const { return (bActive) ? layer : COLLIDER_LAYER_NONE; }

	const Collider* GetCollider() const { return collider; }
	Transform* GetTransform() { return collider->GetTransform(); }
	void UpdateBounding() { collider->UpdateBounding(); }

	// instance
public:
	void Release();
	UINT Junk();
	void Recycle();

	// send massage
public:
	SendBoxMessage* GetSendMessage() const;
	void SetTag(wstring value) { message->Tag = value; }
	void SetSendMessageData(void* value) { message->Message = value; }
	void OnSendMessage() { message->SendTime = Time::Get()->Running(); }

private:
	Collider* collider;
	UINT instanceId;
	bool bActive = true;
	UINT layer = COLLIDER_LAYER_DEFAULT;

	SendBoxMessage* message;
};