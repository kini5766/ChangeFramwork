#include "Framework.h"
#include "ModelMesh.h"

using namespace ShaderEffectName;

ModelSkinnedInstance::ModelSkinnedInstance()
{
	renderer = new SkinnedMeshRenderer();
	transform = new Transform(&world);

	boneCount = data->BoneCount();
	boneDesc = new BoneDesc[boneCount];
	for (UINT i = 0; i < boneCount; i++)
	{
		ModelBoneData* bone = data->BoneByIndex(i);

		boneDesc[i].Parent = bone->ParentIndex;
		D3DXMatrixInverse(&boneDesc[i].InvBone, nullptr, &bone->Transform);
	}

	CreateCompute();
	renderer->BindPose()->SrvBonesMap = computeOutputSrvBuffer->OutputSRV();
}

ModelSkinnedInstance::~ModelSkinnedInstance()
{
	SafeDelete(computeOutputBoneBuffer);
	SafeDelete(computeOutputSrvBuffer);
	SafeDelete(computeBoneDescBuffer);
	SafeDelete(computeShader);

	SafeDeleteArray(boneDesc);
	SafeDelete(transform);
	SafeDelete(renderer);
}

void ModelSkinnedInstance::Update()
{
	transform->UpdateWorld();
	computeShader->Render();
	computeShader->GetShader()->Dispatch(0, 0, boneCount, 1, 1);

	renderer->Update();
}

void ModelSkinnedInstance::CreateCompute()
{
	computeShader = new ShaderSetter(L"02_GetBones.fxo");

	// In : 모델World
	computeShader->SetMatrixPointer(CB_World, &world);

	// In : (애니메이션out) 로컬 본*인스턴스 texture
	computeShader->SetSRV("InputLocalBones", srvLocalBones);

	// In : inv 본, 본 부모 인덱스들
	computeBoneDescBuffer = new StructuredBuffer(boneDesc, sizeof(BoneDesc), boneCount);
	computeShader->SetSRV("InputboneDesc", computeBoneDescBuffer->SRV());


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

	// Out : 로직 본*인스턴스 texture
	computeOutputBoneBuffer = new TextureBuffer(texture);
	computeShader->SetUAV("OutputBone", computeOutputBoneBuffer->UAV());

	// Out : Skinned적용시킬 본*인스턴트 texture
	computeOutputSrvBuffer = new TextureBuffer(texture);
	computeShader->SetUAV("OutputSkinned", computeOutputSrvBuffer->UAV());
}
