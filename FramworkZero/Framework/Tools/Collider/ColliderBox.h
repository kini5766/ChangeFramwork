#pragma once

struct BoxMessageDesc
{
	int Id;  // 보내는 이
	wstring Tag;  // 받는 이
	Collider* Other;
	void* Message;
};


class ColliderBox
{
public:
	ColliderBox(UINT instanceId);
	~ColliderBox();

	// Getter, Setter
public:
	const BoxMessageDesc*const* const GetReceived(UINT* outCount);
	Transform* GetTransform() { return collider->GetTransform(); }

	bool IsActiveSelf() const { return bActive; }
	void SetActiveSelf(bool value) { bActive = value; }

	bool IsReceiver() { return bReceiver; }
	void SetReceiver(bool value) { bReceiver = value; }

	UINT GetLayer() const { return layer; }
	void SetLayer(UINT value) { layer = value; }

	// collision
public:
	UINT GetMask() const { return (bActive) ? layer : COLLIDER_LAYER_NONE; }
	const Collider* GetCollider() const { return collider; }
	void UpdateBounding() { collider->UpdateBounding(); }

	// instance
public:
	void Release();
	UINT Junk();
	void Recycle(int colliderId);

	// send
public:
	void Tag(wstring value) { message->Tag = value; }
	void SetSendMessageData(void* value) { message->Message = value; }
	void SendMassageColliderBox(ColliderBox* other);

	// Receive
public:
	void Send(const BoxMessageDesc* value) { receiveds.push_back(value); }
	int ColliderId() { return colliderId; }
	void ClearReceived() { receiveds.clear(); }

private:
	bool bActive;
	bool bReceiver = false;
	UINT layer;
	UINT instanceId;
	Collider* collider;
	int colliderId = -1;

private:
	BoxMessageDesc* message;
	vector<const BoxMessageDesc*> receiveds;
};