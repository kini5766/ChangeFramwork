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
	// 클립트랜스폼
	ID3D11ShaderResourceView* srvClipBoneMap = nullptr;


	ShaderSetter* computeShader;
	// in : 블랜드정보 * 인스턴스
	ConstantBuffer* blendBuffer;
	// in : 클립트랜스폼
	// in : 키프레임 별 최대치 * 본 개수
	StructuredBuffer* computeCountBuffer;
	// out : 로컬 본*인스턴스 texture
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
		UINT Mode = 0;  // bool 1바이트 애매함
		float Alpha = 0;
		float Padding[2];

		KeyframeDesc Clip[2];
	}/*blendDesc[MODEL_INSTANCE_MAX_COUNT]*/;

private:
	// 키프레임 별 최대치
	struct KeyframeCount
	{
		UINT Translation;
		UINT Rotaion;
		UINT Scale;
		UINT MaxCount;
	}*keyframeCount;
};