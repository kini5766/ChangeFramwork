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
	Collider* CreateCollider();
	void ReleseCollider(Collider* value);
	void GetColliders(vector<Collider*>* out) { (*out) = colliders; }

private:
	vector<Collider*> colliders;
	vector<UINT> junkInstances;
};