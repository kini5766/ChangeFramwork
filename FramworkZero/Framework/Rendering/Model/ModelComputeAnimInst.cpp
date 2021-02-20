#include "Framework.h"
#include "ModelComputeAnimInst.h"

#include "ModelAnimDesc.h"
#include "ModelAnimMap.h"
#include "ModelBonesMap.h"

using namespace ShaderEffectName;

ModelComputeAnimInst::ModelComputeAnimInst(const ModelAnimDesc& desc)
	: clipCount(desc.ClipCount)
	, boneCount(desc.BoneCount)
{
	animMap = new ModelAnimMap(desc);
	modelBonesMap = new ModelBonesMap(desc);

	CreateComputeAnim();
	CreateComputeBone();
}

ModelComputeAnimInst::~ModelComputeAnimInst()
{
	SafeDelete(computeOutputSrvBuffer);
	SafeDelete(computeShaderBone);
	SafeDelete(modelBonesMap);

	SafeDelete(computeOutputAnimBuffer);
	SafeDelete(blendBuffer);
	SafeDelete(computeShaderAnim);

	SafeDelete(animMap);
}

ID3D11ShaderResourceView * ModelComputeAnimInst::GetOutputAnimSrv()
{
	return computeOutputAnimBuffer->OutputSRV();
}

ID3D11ShaderResourceView * ModelComputeAnimInst::GetOutputBoneResultSrv()
{
	return computeOutputSrvBuffer->OutputSRV();
}

ID3D11Texture2D * ModelComputeAnimInst::CopyFromOutput()
{
	return computeOutputSrvBuffer->CopyFromOutput();
}

BlendDesc * ModelComputeAnimInst::GetDesc(UINT index)
{
	return blendDesc + index;
}

void ModelComputeAnimInst::Update()
{
	blendBuffer->Render();

	computeShaderAnim->Render();
	computeShaderAnim->GetShader()->Dispatch(0, 0, boneCount, 1, 1);

	computeShaderBone->Render();
	computeShaderBone->GetShader()->Dispatch(0, 0, boneCount, 1, 1);
}

void ModelComputeAnimInst::CreateComputeAnim()
{
	computeShaderAnim = new ShaderSetter(L"02_AnimationInstance.fxo");

	// in : �������� * �ν��Ͻ�
	blendBuffer = new ConstantBuffer(blendDesc, sizeof(BlendDesc) * MODEL_INSTANCE_MAX_COUNT);
	computeShaderAnim->SetConstantBuffer(CB_BLENDING_FRAME, blendBuffer->Buffer());

	// in : Ŭ��Ʈ������
	computeShaderAnim->SetSRV(INPUT_CLIP_MAP, animMap->ClipBoneMapSrv());

	// in : Ű������ �� �ִ�ġ * (�� ���� * Ŭ�� ����)
	computeShaderAnim->SetSRV(INPUT_KEYFRAME_COUNTS, animMap->FrameCountsSrv());

	Texture2DDesc tex2DDesc;
	D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
	desc.Width = boneCount * 4u;
	desc.Height = MODEL_INSTANCE_MAX_COUNT;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	ID3D11Texture2D* texture = tex2DDesc.CreateBlankTexture();

	// out : ���� ��*�ν��Ͻ� texture
	computeOutputAnimBuffer = new TextureBuffer(texture);
	computeShaderAnim->SetUAV(OUTPUT, computeOutputAnimBuffer->UAV());
	SafeRelease(texture);  // �ؽ��� ����Ǽ� ������ �ʿ� ����
}

void ModelComputeAnimInst::CreateComputeBone()
{
	computeShaderBone = new ShaderSetter(L"02_GetBonesInstance.fxo");

	// In : (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	computeShaderBone->SetSRV(INPUT_LOCAL_BONES, GetOutputAnimSrv());

	// In : inv ��, �� �θ� �ε�����
	computeShaderBone->SetSRV(INPUT_BONE_DESC, modelBonesMap->SRV());


	Texture2DDesc tex2DDesc;
	D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
	desc.Width = boneCount * 4u;
	desc.Height = MODEL_INSTANCE_MAX_COUNT;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	ID3D11Texture2D* texture = tex2DDesc.CreateBlankTexture();

	// Out : Skinned�����ų ��*�ν���Ʈ texture
	computeOutputSrvBuffer = new TextureBuffer(texture);
	computeShaderBone->SetUAV(OUTPUT_SKINNED, computeOutputSrvBuffer->UAV());

	SafeRelease(texture);  // �ؽ��� ����Ǽ� ������ �ʿ� ����
}
