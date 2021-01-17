#pragma once

class Particle
{
public:
	// file : xml���� ���
	Particle(wstring xmlFile);
	~Particle();

	// Update
public:
	void Update();
	void Render();
	void Reset();  // �ٽ� �׷���
	void Emit(const Vector3& position);

	// Getter
public:
	Material* GetMaterial() { return material; }
	ParticleData& GetData() { return data; }
	void SetTexture(wstring file);

private:
	void MapVertices();
	void Activation();
	void Deactivation();

private:
	void ReadFile(wstring xmlFile);

private:
	ParticleData data;
	Texture* map;

	ConstantBuffer* buffer;

private:
	MeshData mesh;
	Material* material;
	PerTransform* perTransform;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

private:
	// 1/60 �� ����� �ð�
	float currentTime = 0.0f;
	// 1/60 �� ����� �ð�
	float lastAddTime = 0.0f;

	// gpu ~ lead : ������ �� (��Ʈ�������� �ѱ�)
	UINT leadCount = 0;  // near
	UINT gpuCount = 0;  // front
	// deactive ~ active
	UINT activeCount = 0;  // near
	UINT deactiveCount = 0;  // front

private:
	struct Desc
	{
		Color MinColor;
		Color MaxColor;

		Vector3 Gravity;
		float EndVelocity;

		Vector2 StartSize;
		Vector2 EndSize;

		Vector2 RotateSpeed;
		float ReadyTime;
		float ReadyRandomTime;

		float CurrentTime;
		float Padding[3];
	}desc;

private:
	struct VertexParticle
	{
		Vector3 Position;
		Vector2 Corner;  // uv
		Vector3 Velocity;

		// float4 ( ���, ũ��, ȸ��, ���� )
		Vector4 Random;
		// Emmitte, Active�� �� �ð�
		float Time;
	};
};