#pragma once

class Particle
{
public:
	// file : xml파일 경로
	Particle(wstring xmlFile);
	~Particle();

	// Update
public:
	void Update();
	void Render();
	void Reset();  // 다시 그려짐
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
	// 1/60 초 계산할 시간
	float currentTime = 0.0f;
	// 1/60 초 계산할 시각
	float lastAddTime = 0.0f;

	// gpu ~ lead : 복사할 것 (스트리밍으로 넘김)
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

		// float4 ( 배속, 크기, 회전, 색상 )
		Vector4 Random;
		// Emmitte, Active가 된 시각
		float Time;
	};
};