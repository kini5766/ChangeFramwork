#pragma once

// �ν��Ͻ� �ִ� ����
constexpr auto MESH_INSTANCE_MAX_COUNT = 512;

class MeshInstance;

class MeshInstRenderer
{
public:
	MeshInstRenderer(unique_ptr<MeshData> data);
	~MeshInstRenderer();

public:
	void UpdateTransforms();

	void Render();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	MeshRenderer* GetRenderer() const { return renderer; }

public:
	MeshInstance* AddInstance();
	MeshInstance* GetInstance(UINT index) { return instances[index]; }
	void RemoveInstance(MeshInstance* value);

private: // render ����
	MeshRenderer* renderer;
	unique_ptr<MeshData> meshData;

private: // instance ����
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
	MeshInstance(MeshInstRenderer* perent, UINT id)
		: perent(perent), id(id)
	{
		transform = new Transform();
	}
	~MeshInstance()
	{
		SafeDelete(transform);
	}

public:
	MeshInstRenderer* Perent() { return perent; }
	Transform* GetTransform() { return transform; }
	UINT Id() { return id; }
	void Release() { perent->RemoveInstance(this); }

private:
	MeshInstRenderer* perent;
	Transform* transform;
	UINT id;
};