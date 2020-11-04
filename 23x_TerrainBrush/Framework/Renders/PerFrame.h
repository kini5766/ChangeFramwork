#pragma once

// �� ������ ������Ʈ�� �ܽ�źƮ����
// �ܽ�źƮ���۴� IASet�Լ� ȣ�� �� �ص� �ٷ� ����
class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

public:
	void Update();
	void Render();

private:
	Shader* shader;
	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	ConstantBuffer* lightBuffer;
	ID3DX11EffectConstantBuffer* sLightBuffer;

private:
	struct BufferDesc
	{
		Matrix View;
		Matrix ViewInverse;
		Matrix Projection;
		Matrix VP;

		float Time;
		// 16����Ʈ ���� �е��ؾ���
		float Padding[3];
		//Vector3 Padding;

	} bufferDesc;

	struct LightDesc // <- �޽�
	{
		Color Ambient;
		Color Specular;
		Vector3 Direction; float Padding_L1;
		Vector3 Position; float Padding_L2;
	} lightDesc;
};