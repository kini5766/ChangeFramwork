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

	// ColliderBox
public:
	ColliderBox* CreateCollider();
	void ReleaseCollider(ColliderBox* value);
	void GetColliders(vector<ColliderBox*>* out) { (*out) = colliders; }
private:
	vector<ColliderBox*> colliders;
	vector<UINT> junkColliders;
	int idNext = 0;

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