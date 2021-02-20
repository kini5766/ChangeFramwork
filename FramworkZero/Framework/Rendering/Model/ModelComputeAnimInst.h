#pragma once

class ModelComputeAnimInst
{
public:
	ModelComputeAnimInst(const struct ModelAnimDesc& desc);
	~ModelComputeAnimInst();

public:
	void Update();

public:
	ID3D11ShaderResourceView* GetOutputAnimSrv();
	ID3D11ShaderResourceView* GetOutputBoneResultSrv();
	ID3D11Texture2D* CopyFromOutput();
	BlendDesc* GetDesc(UINT index);

private:
	void CreateComputeAnim();
	void CreateComputeBone();


private:
	UINT clipCount;
	UINT boneCount;

private:  // Anim
	class ModelAnimMap* animMap;
	BlendDesc blendDesc[MODEL_INSTANCE_MAX_COUNT];

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