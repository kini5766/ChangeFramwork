#pragma once

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

// ���� �� ������ ����źƮ ����
class PerFrameBuffer
{
public:
	PerFrameBuffer();
	~PerFrameBuffer();

public:
	void Update();
	void Render();

public:
	ConstantBuffer* BufferPerFrame() { return buffer; }
	ConstantBuffer* LightBuffer() { return lightBuffer; }
	ConstantBuffer* PointLightBuffer() { return pointLightBuffer; }
	ConstantBuffer* SpotLightBuffer() { return spotLightBuffer; }
	ConstantBuffer* BurntLightBuffer() { return burntLightBuffer; }

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


// ----------------------------------------------------------------------------
// PerFrame
// ----------------------------------------------------------------------------

// ���̴��� �ִ� �� ������ ����źƮ ����
class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

public:
	void Update();
	void Render();

private:
	ShaderSetter* shader;

	PerFrameBuffer* buffer;
};