#include "Framework.h"
#include "ModelAnimation.h"

using namespace ShaderEffectName;

ModelAnimation::ModelAnimation(ModelData * data)
{
	boneCount = data->BoneCount();

	// Todo : 클립맵 만들기
	// keyframeCount = new
}

ModelAnimation::~ModelAnimation()
{
	SafeDeleteArray(keyframeCount);

	SafeDelete(computeOutputBuffer);
	SafeDelete(computeCountBuffer);
	SafeDelete(blendBuffer);
	SafeDelete(computeShader);
}

void ModelAnimation::CreateCompute()
{
	computeShader = new ShaderSetter(L"02_AnimationInstance.fxo");

	// in : 블랜드정보 * 인스턴스
	blendBuffer = new ConstantBuffer(keyframeDesc, sizeof(KeyframeDesc) * MODEL_INSTANCE_MAX_COUNT);
	computeShader->SetConstantBuffer(CB_Keyframes, blendBuffer->Buffer());

	// in : 클립트랜스폼
	computeShader->SetSRV("InputClipMap", srvClipBoneMap);

	// in : 키프레임 별 최대치 * 본 개수
	computeCountBuffer = new StructuredBuffer(keyframeCount, sizeof(KeyframeCount), boneCount);
	computeShader->SetSRV("InputKeyframeCount", computeCountBuffer->SRV());

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
	computeOutputBuffer = new TextureBuffer(texture);
	computeShader->SetUAV("Output", computeOutputBuffer->UAV());
}
