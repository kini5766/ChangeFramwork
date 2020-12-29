#pragma once


class ModelComputeAnimInst
{
public:
	ModelComputeAnimInst(ModelData* data, Matrix* world);
	~ModelComputeAnimInst();

public:
	void Update();

public:
	ID3D11ShaderResourceView* GetOutputAnimSrv();
	ID3D11ShaderResourceView* GetOutputBoneResultSrv();
	ID3D11Texture2D* CopyFromOutput();
	KeyframeDesc* GetDesc(UINT index);

private:
	void CreateComputeAnim();
	void CreateComputeBone(Matrix* world);


private:
	UINT clipCount;
	UINT boneCount;

private:  // Anim
	class ModelAnimMap* animMap;
	KeyframeDesc keyframeDesc[MODEL_INSTANCE_MAX_COUNT];

private:  // Compute
	ShaderSetter* computeShaderAnim;
	// in : 블랜드정보 * 인스턴스
	ConstantBuffer* blendBuffer;
	// in : 클립트랜스폼
	// in : 키프레임 별 최대치 * 본 개수 * 클립 수
	// out : 로컬 본*인스턴스 texture
	TextureBuffer* computeOutputAnimBuffer;

private:  // Bone
	ShaderSetter* computeShaderBone;
	// In : 모델World
	// In : (애니메이션out) 로컬 본*인스턴스 texture
	// In : inv 본, 본 부모 인덱스들
	class ModelBonesMap* modelBonesMap;
	// Out : Skinned적용시킬 본*인스턴트 texture
	TextureBuffer* computeOutputSrvBuffer;
};