#pragma once

struct ShadowDesc
{
	Matrix View;
	Matrix Projection;

	// ��
	Vector2 MapSize;

	//
	float Bias = -0.001f;

	// �ε��� ���
	UINT Quality = 2;
};


class Shadow
{
public:
	Shadow(const Vector3& at, float radius, float width = 1024, float height = 1024);
	~Shadow();

public:
	void Update();
	void PreRender();

public:
	void ImGuiRender();
	void At(const Vector3& value);

private:
	ShadowDesc desc;
	PreShadowValue* output;
	ID3D11SamplerState* shadowSampler;
	class ShadowCamera* camera;
};
