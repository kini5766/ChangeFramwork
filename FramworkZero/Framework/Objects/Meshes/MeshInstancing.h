#pragma once

// 인스턴스 최대 개수
constexpr auto MESH_INSTANCE_MAX_COUNT = 512;

class MeshInstance;

class MeshInstancing
{
public:
	// MeshData* : 그대로 사용 delete는 이 클레스에서
	MeshInstancing(Shader* shader, MeshData* data);
	~MeshInstancing();

public:
	void Update();
	void Render();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	MeshRenderer* GetRenderer() const { return renderer; }

public:
	MeshInstance* AddInstance();
	MeshInstance* GetInstance(UINT index) { return instances[index]; }
	void RemoveInstance(MeshInstance* value);

	void UpdateTransforms();


private: // render 관련
	MeshRenderer* renderer;
	MeshData* meshData;
	PerFrame* perframe = nullptr;

private: // instance 관련
	vector<MeshInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;
};


// --
// MeshInstance
// --

class MeshInstance
{
public:
	MeshInstance(MeshInstancing* perent, UINT id) 
		: perent(perent), id(id)
	{
		transform = new Transform();
	}
	~MeshInstance()
	{
		SafeDelete(transform);
	}

public:
	MeshInstancing* Perent() { return perent; }
	Transform* GetTransform() { return transform; }
	UINT Id() { return id; }
	void Release() { perent->RemoveInstance(this); }

private:
	MeshInstancing* perent;
	Transform* transform;
	UINT id;
};