#pragma once

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

// ���� �� ������ ����źƮ ����
class PerFrame
{
public:
	PerFrame();
	~PerFrame();

public:
	void Update();
	void Render();

public:
	void SetAtMaterial(ShaderSetter* target);

private:
	ConstantBuffer* buffer;
	ConstantBuffer* lightBuffer;
	ConstantBuffer* pointLightBuffer;
	ConstantBuffer* spotLightBuffer;
	ConstantBuffer* burntLightBuffer;

private:
	struct Desc
	{
		Matrix View;
		Matrix ViewInverse;
		Matrix Projection;
		Matrix VP;

		Plane Culling[4];
		Plane Clipping;

		float Time;
		float Padding[3];
	} desc;
};
