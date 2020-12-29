#include "Framework.h"
#include "ModelComputeAnimInst.h"

#include "ModelAnimMap.h"
#include "ModelBonesMap.h"

using namespace ShaderEffectName;

ModelComputeAnimInst::ModelComputeAnimInst(ModelData * data, Matrix* world)
{
	clipCount = data->ClipCount();
	boneCount = data->BoneCount();

	animMap = new ModelAnimMap(data);
	modelBonesMap = new ModelBonesMap(data);

	CreateComputeAnim();
	CreateComputeBone(world);
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

KeyframeDesc * ModelComputeAnimInst::GetDesc(UINT index)
{
	return keyframeDesc + index;
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
	blendBuffer = new ConstantBuffer(keyframeDesc, sizeof(KeyframeDesc) * MODEL_INSTANCE_MAX_COUNT);
	computeShaderAnim->SetConstantBuffer(CB_Keyframes, blendBuffer->Buffer());

	// in : Ŭ��Ʈ������
	computeShaderAnim->SetSRV("InputClipMap", animMap->ClipBoneMapSrv());

	// in : Ű������ �� �ִ�ġ * (�� ���� * Ŭ�� ����)
	computeShaderAnim->SetSRV("InputKeyframeCount", animMap->FrameCountsSrv());

	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = boneCount * 4u;
	desc.Height = MODEL_INSTANCE_MAX_COUNT;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));

	// out : ���� ��*�ν��Ͻ� texture
	computeOutputAnimBuffer = new TextureBuffer(texture);
	computeShaderAnim->SetUAV("Output", computeOutputAnimBuffer->UAV());
	SafeRelease(texture);  // �ؽ��� ����Ǽ� ������ �ʿ� ����
}

void ModelComputeAnimInst::CreateComputeBone(Matrix* world)
{
	computeShaderBone = new ShaderSetter(L"02_GetBonesInstance.fxo");

	// In : ��World
	computeShaderBone->SetMatrixPointer(CB_World, world);

	// In : (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	computeShaderBone->SetSRV("InputLocalBones", GetOutputAnimSrv());

	// In : inv ��, �� �θ� �ε�����
	computeShaderBone->SetSRV("InputboneDesc", modelBonesMap->SRV());


	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = boneCount * 4u;
	desc.Height = MODEL_INSTANCE_MAX_COUNT;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));

	// Out : Skinned�����ų ��*�ν���Ʈ texture
	computeOutputSrvBuffer = new TextureBuffer(texture);
	computeShaderBone->SetUAV("OutputSkinned", computeOutputSrvBuffer->UAV());
}
