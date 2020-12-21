#pragma once

class ModelAnimation
{
public:
	ModelAnimation(ModelData* data);
	~ModelAnimation();

private:
	void CreateCompute();

private:
	UINT boneCount;
	// Ŭ��Ʈ������
	ID3D11ShaderResourceView* srvClipBoneMap = nullptr;


	ShaderSetter* computeShader;
	// in : �������� * �ν��Ͻ�
	ConstantBuffer* blendBuffer;
	// in : Ŭ��Ʈ������
	// in : Ű������ �� �ִ�ġ * �� ����
	StructuredBuffer* computeCountBuffer;
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
	// Ű������ �� �ִ�ġ
	struct KeyframeCount
	{
		UINT Translation;
		UINT Rotaion;
		UINT Scale;
		UINT MaxCount;
	}*keyframeCount;
};