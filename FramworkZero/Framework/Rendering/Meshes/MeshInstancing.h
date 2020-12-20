#pragma once

// �ν��Ͻ� �ִ� ����
constexpr auto MESH_INSTANCE_MAX_COUNT = 512;

class MeshInstance;

class MeshInstancing
{
public:
	// MeshData* : �״�� ��� delete�� MeshRenderer Ŭ��������
	MeshInstancing(Shader* shader, MeshData* data);
	~MeshInstancing();

public:
	void Update();
	void Render();

public:
	void Pass(UINT value) { renderer->Pass(value); }
	MeshRenderer* GetRenderer() { return renderer; }

public:
	MeshInstance* AddInstance();
	void RemoveInstance(MeshInstance* value);

	void UpdateTransforms();
	void UpdateColors();

public:
	void SetColor(UINT instance, const Color& color);

private: // render ����
	MeshRenderer* renderer;
	PerFrame* perframe = nullptr;

private: // instance ����
	vector<MeshInstance*> instances;
	vector<UINT> junkInstances;

	Matrix worlds[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceBuffer;

	Color colors[MESH_INSTANCE_MAX_COUNT];
	VertexBuffer* instanceColorBuffer;
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