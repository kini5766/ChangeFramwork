#pragma once


class ReceiveBox
{
public:
	ReceiveBox(UINT instanceId);
	~ReceiveBox();

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

	// Receive
public:
	void ClearMassage();
	vector<SendBoxMessage*>& GetMessages() { return sends; }
	void Send(SendBoxMessage* message);

private:
	bool bActive = false;
	UINT layer = 0;
	UINT instanceId;
	Collider* collider;

private:
	struct ReceiveBoxMessage
	{
		int SendId;
		float SendActiveTime;
	};
	UINT findIndex = 0;
	list<ReceiveBoxMessage> receives;
	vector<SendBoxMessage*> sends;
};