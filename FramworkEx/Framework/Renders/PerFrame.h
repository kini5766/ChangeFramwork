#pragma once

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

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
	ConstantBuffer* BufferLight() { return lightBuffer; }

private:
	ConstantBuffer* buffer;
	ConstantBuffer* lightBuffer;

private:
	struct BufferDesc
	{
		Matrix View;
		Matrix ViewInverse;
		Matrix Projection;
		Matrix VP;

		float Time;
		// 16바이트 단위 패딩해야함
		float Padding[3];
		//Vector3 Padding;

	} bufferDesc;

	struct LightDesc // <- 메쉬
	{
		Color Ambient;
		Color Specular;
		Vector3 Direction; float Padding_L1;
		Vector3 Position; float Padding_L2;
	} lightDesc;
};


// ----------------------------------------------------------------------------
// PerFrame
// ----------------------------------------------------------------------------

class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

public:
	void Update();
	void Render();

private:
	Shader* shader = nullptr;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectConstantBuffer* sLightBuffer;

	PerFrameBuffer* buffer;
};