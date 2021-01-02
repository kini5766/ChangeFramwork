#pragma once

class CollisionManager
{
public:
	static void Create();
	static void Delete();
	static CollisionManager* Get();
private:
	static CollisionManager* instance;

private:
	CollisionManager();
	~CollisionManager();

public:
	void Update();

private:
	void UpdateInput();
	void CheckCollision();

	// ReceiveBox
public:
	ReceiveBox* CreateCollider();
	void ReleaseCollider(ReceiveBox* value);
	void GetColliders(vector<ReceiveBox*>* out) { (*out) = colliders; }
private:
	vector<ReceiveBox*> colliders;
	vector<UINT> junkColliders;

	// SendBox
public:
	SendBox* CreateSendBox();
	void ReleaseSendBox(SendBox* value);
private:
	vector<SendBox*> sendBoxes;
	vector<UINT> junkSendBoxes;

	// Raycast
public:
	Raycast* CreateRaycast(const Ray& ray, UINT layer);
private:
	struct RaycastPair
	{
		RaycastOutput* Output;
		Raycast* Input;

		~RaycastPair()
		{
			SafeDelete(Output);
			SafeDelete(Input);
		}
	};
	list<RaycastPair*> rays;
};