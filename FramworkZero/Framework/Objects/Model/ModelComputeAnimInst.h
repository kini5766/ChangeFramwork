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
	// in : �������� * �ν��Ͻ�
	ConstantBuffer* blendBuffer;
	// in : Ŭ��Ʈ������
	// in : Ű������ �� �ִ�ġ * �� ���� * Ŭ�� ��
	// out : ���� ��*�ν��Ͻ� texture
	TextureBuffer* computeOutputAnimBuffer;

private:  // Bone
	ShaderSetter* computeShaderBone;
	// In : ��World
	// In : (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	// In : inv ��, �� �θ� �ε�����
	class ModelBonesMap* modelBonesMap;
	// Out : Skinned�����ų ��*�ν���Ʈ texture
	TextureBuffer* computeOutputSrvBuffer;
};