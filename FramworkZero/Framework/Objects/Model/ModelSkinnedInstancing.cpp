#include "Framework.h"
#include "ModelSkinnedInstancing.h"

using namespace ShaderEffectName;

ModelSkinnedInstancing::ModelSkinnedInstancing(Shader* shader, const ModelDesc& desc)
{
	transform = new Transform(&world);
	data = new ModelData();
	// Todo : 로드

	ApplyModel(shader);
}

ModelSkinnedInstancing::~ModelSkinnedInstancing()
{
	SafeDelete(computeOutputBoneBuffer);
	SafeDelete(computeOutputSrvBuffer);
	SafeDelete(computeBoneDescBuffer);
	SafeDelete(computeShader);

	SafeDelete(animation);
	SafeDeleteArray(boneDesc);
	SafeDelete(transform);
	SafeDelete(renderer);
	SafeDelete(data);
}

void ModelSkinnedInstancing::Update()
{
	animation->Update();

	transform->UpdateWorld();
	computeShader->Render();
	computeShader->GetShader()->Dispatch(0, 0, boneCount, 1, 1);

	renderer->Update();
}

void ModelSkinnedInstancing::Render()
{
	renderer->Render();
}

ModelSkinnedInstance * ModelSkinnedInstancing::AddInstance()
{
	ModelSkinnedInstance* instance = nullptr;

	if (junkInstances.size() == 0)
	{
		UINT index = instances.size();

		instance = new ModelSkinnedInstance(this, index);
		instances.push_back(instance);

		instance->GetTransform()->ReplaceMatrixGetter(worlds + index);
		colors[index] = Color(0, 0, 0, 1);
	}
	else
	{
		UINT index = junkInstances.back();

		instance = instances[index];

		Matrix m;
		D3DXMatrixIdentity(&m);
		instance->GetTransform()->LossyWorld(m);

		colors[index] = Color(0, 0, 0, 1);

		junkInstances.pop_back();
	}

	return instance;
}

void ModelSkinnedInstancing::RemoveInstance(ModelSkinnedInstance * value)
{
	assert(value->Perent() == this);

	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	junkInstances.push_back(value->Id());
}

void ModelSkinnedInstancing::UpdateTransforms()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	for (ModelSkinnedInstance* instance : instances)
		instance->GetTransform()->UpdateWorld();

	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MESH_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

void ModelSkinnedInstancing::UpdateColors()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	D3D::GetDC()->Map(instanceColorBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, colors, sizeof(Color) * MESH_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceColorBuffer->Buffer(), 0);
}

void ModelSkinnedInstancing::SetColor(UINT instance, const Color & color)
{
	colors[instance] = color;
}


void ModelSkinnedInstancing::ApplyModel(Shader* shader)
{
	boneCount = data->BoneCount();
	boneDesc = new BoneDesc[boneCount];
	for (UINT i = 0; i < boneCount; i++)
	{
		ModelBoneData* bone = data->BoneByIndex(i);

		boneDesc[i].Parent = bone->ParentIndex;
		D3DXMatrixInverse(&boneDesc[i].InvBone, nullptr, &bone->Transform);
	}

	animation = new ModelAnimationInstancing(data);

	CreateCompute();

	renderer = new SkinnedMeshRenderer();
	UINT size = data->MeshCount();
	for (UINT i = 0; i < size; i++)
	{
		ModelMeshData* mesh = data->MeshByIndex(i);
		renderer->Renderers().push_back(new MeshRenderer(shader, mesh->Mesh));
	}
	renderer->BindPose()->SrvBonesMap = computeOutputSrvBuffer->OutputSRV();
	renderer->SetMaterials(data->Materials().data(), data->Materials().size());
}

void ModelSkinnedInstancing::CreateCompute()
{
	computeShader = new ShaderSetter(L"02_GetBones.fxo");

	// In : 모델World
	computeShader->SetMatrixPointer(CB_World, &world);

	// In : (애니메이션out) 로컬 본*인스턴스 texture
	srvLocalBones = animation->GetOutputSrv();
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
