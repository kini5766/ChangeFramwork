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

public:
	Collider* CreateCollider();
	void ReleaseCollider(Collider* value);
	void GetColliders(vector<Collider*>* out) { (*out) = colliders; }

public:
	Raycast* CreateCameraRaycast(const Ray& ray);

private:
	void UpdateInput();
	void CheckCollision();

private:
	vector<Collider*> colliders;
	vector<UINT> junkInstances;

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
	list<RaycastPair*> cameraRays;
};