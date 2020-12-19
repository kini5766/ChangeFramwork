#pragma once

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

// 순수 매 프레임 컨스탄트 버퍼
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

// 쉐이더가 있는 매 프레임 컨스탄트 버퍼
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