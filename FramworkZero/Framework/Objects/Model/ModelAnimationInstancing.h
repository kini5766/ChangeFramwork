#pragma once

class ModelAnimationInstancing
{
public:
	ModelAnimationInstancing(ModelData* data);
	~ModelAnimationInstancing();

public:
	void Update();

public:
	ID3D11ShaderResourceView* GetOutputSrv();

private:
	void CreateCompute();

private:
	UINT clipCount;
	UINT boneCount;

	// Ŭ��Ʈ������
	class ModelClipTexture* clipBoneMap;

	ShaderSetter* computeShader;
	// in : �������� * �ν��Ͻ�
	ConstantBuffer* blendBuffer;
	// in : Ŭ��Ʈ������
	// in : Ű������ �� �ִ�ġ * �� ���� * Ŭ�� ��
	TextureBuffer* computeCountBuffer;
	// out : ���� ��*�ν��Ͻ� texture
	TextureBuffer* computeOutputBuffer;

private:
	struct KeyframeDesc
	{
		int Clip = -1;
		float Time = 0.0f;
		float RunningTime = 0.0f;
		float Speed = 1.0f;
	}keyframeDesc[MODEL_INSTANCE_MAX_COUNT];

	struct BlendDesc
	{
		UINT Mode = 0;  // bool 1����Ʈ �ָ���
		float Alpha = 0;
		float Padding[2];

		KeyframeDesc Clip[2];
	}/*blendDesc[MODEL_INSTANCE_MAX_COUNT]*/;

private:
	// Ű������ �� �ִ�ġ (1Ŭ��-1�� �߿�)
	// �ּҰ� �ƴ� ���� �Ѱܾ��ϱ� ������ 1�������� ����
	struct KeyframeCount
	{
		UINT Translation;  // r
		UINT Rotaion;  // g
		UINT Scale;  // b
		UINT MaxCount;  // a
	}*keyframeCount;
};