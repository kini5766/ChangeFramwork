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

	// in : 블랜드정보 * 인스턴스
	blendBuffer = new ConstantBuffer(keyframeDesc, sizeof(KeyframeDesc) * MODEL_INSTANCE_MAX_COUNT);
	computeShaderAnim->SetConstantBuffer(CB_Keyframes, blendBuffer->Buffer());

	// in : 클립트랜스폼
	computeShaderAnim->SetSRV("InputClipMap", animMap->ClipBoneMapSrv());

	// in : 키프레임 별 최대치 * (본 개수 * 클립 개수)
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

	// out : 로컬 본*인스턴스 texture
	computeOutputAnimBuffer = new TextureBuffer(texture);
	computeShaderAnim->SetUAV("Output", computeOutputAnimBuffer->UAV());
	SafeRelease(texture);  // 텍스쳐 복사되서 유지할 필요 없음
}

void ModelComputeAnimInst::CreateComputeBone(Matrix* world)
{
	computeShaderBone = new ShaderSetter(L"02_GetBonesInstance.fxo");

	// In : 모델World
	computeShaderBone->SetMatrixPointer(CB_World, world);

	// In : (애니메이션out) 로컬 본*인스턴스 texture
	computeShaderBone->SetSRV("InputLocalBones", GetOutputAnimSrv());

	// In : inv 본, 본 부모 인덱스들
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

	// Out : Skinned적용시킬 본*인스턴트 texture
	computeOutputSrvBuffer = new TextureBuffer(texture);
	computeShaderBone->SetUAV("OutputSkinned", computeOutputSrvBuffer->UAV());
}
