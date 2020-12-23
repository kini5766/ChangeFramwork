#include "Framework.h"
#include "ModelAnimationInstancing.h"
#include "ModelClipTexture.h"

using namespace ShaderEffectName;

ModelAnimationInstancing::ModelAnimationInstancing(ModelData * data)
{
	clipCount = data->ClipCount();
	boneCount = data->BoneCount();

	clipBoneMap = new ModelClipTexture(data);

	// keyframeCount[clipIndex * boneCount + boneIndex]
	keyframeCount = new KeyframeCount[clipCount * boneCount];
	for (UINT c = 0; c < clipCount; c++)
	{
		UINT boneSize = data->ClipByIndex(c)->Bones.size();
		for (UINT b = 0; b < boneSize; b++)
		{
			KeyframeCount& k = keyframeCount[c * boneCount + b];
			ClipBoneData* bone = data->Clips()[c]->Bones[b];
			k.Translation = bone->KeyTranslations.size();
			k.Rotaion = bone->KeyRotations.size();
			k.Scale = bone->KeyScales.size();
			k.MaxCount = bone->MaxFrame;
		}
	}

	CreateCompute();
}

ModelAnimationInstancing::~ModelAnimationInstancing()
{
	SafeDelete(computeOutputBuffer);
	SafeDelete(computeCountBuffer);
	SafeDelete(blendBuffer);
	SafeDelete(computeShader);

	SafeDeleteArray(keyframeCount);
	SafeDelete(clipBoneMap);
}

ID3D11ShaderResourceView * ModelAnimationInstancing::GetOutputSrv()
{
	return computeOutputBuffer->OutputSRV();
}

void ModelAnimationInstancing::Update()
{
	computeShader->Render();
	computeShader->GetShader()->Dispatch(0, 0, boneCount, 1, 1);
}

void ModelAnimationInstancing::CreateCompute()
{
	computeShader = new ShaderSetter(L"02_AnimationInstance.fxo");

	// in : 블랜드정보 * 인스턴스
	blendBuffer = new ConstantBuffer(keyframeDesc, sizeof(KeyframeDesc) * MODEL_INSTANCE_MAX_COUNT);
	computeShader->SetConstantBuffer(CB_Keyframes, blendBuffer->Buffer());

	// in : 클립트랜스폼
	computeShader->SetSRV("InputClipMap", clipBoneMap->GetSRV());

	Texture2D t2d = Texture2D(boneCount, clipCount);
	t2d.Format(4u * 4u, DXGI_FORMAT_R32G32B32A32_UINT);
	t2d.SetColors(keyframeCount);
	t2d.CreateTexture();

	// in : 키프레임 별 최대치 * (본 개수 * 클립 개수)
	computeCountBuffer = new TextureBuffer(t2d.GetTexture());
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
	SafeRelease(texture);  // 텍스쳐 복사되서 유지할 필요 없음
}
