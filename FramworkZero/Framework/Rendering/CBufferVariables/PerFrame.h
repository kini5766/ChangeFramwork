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

private:
	ConstantBuffer* buffer;
	ConstantBuffer* lightBuffer;

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

	struct LightDesc
	{
		Color Ambient;
		Color Specular;
		Vector3 Direction;
		float Padding;

		Vector3 Position;
		float Padding2;
	}lightDesc;
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