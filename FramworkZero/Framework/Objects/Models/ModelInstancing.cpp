#include "Framework.h"
#include "ModelInstancing.h"

#include "Rendering/Model/ModelAnimDesc.h"
#include "Rendering/Model/ModelComputeAnimInst.h"
using namespace ShaderEffectName;

ModelInstancing::ModelInstancing(const ModelDesc& desc)
{
	renderer = new ModelRenderer(desc);
	instanceBuffer = new VertexBuffer(worlds, MODEL_INSTANCE_MAX_COUNT, sizeof(Matrix), 1, true);

	Shader* shader = renderer->GetDefaultShader();
	shadowCaster = Context::Get()->AddShadowCaster({ shader,
		bind(&ModelInstancing::PreRender_Depth, this) }
	);
	envCubeCaster = Context::Get()->AddEnvCubeCaster({ shader,
		bind(&ModelInstancing::PreRender_EnvCube, this) }
	);

	ModelData* data = renderer->GetModel();

	enableAnim = data->ClipCount() != 0;
	if (enableAnim)
	{
		SkinnedMeshData* skinnedData = data->GetRawData();
		compute = new ModelComputeAnimInst(data);

		UINT materialCount = skinnedData->Materials.size();
		for (UINT i = 0; i < materialCount; i++)
			skinnedData->Materials[i]->SetSRV(BONESMAP, compute->GetOutputBoneResultSrv());
	
		animData = new AnimData();
		UINT size = data->ClipCount();
		const ModelClipData*const* clips = data->Clips();

		for (UINT i = 0; i < size; i++)
		{
			ClipData clipData;
			clipData.Duration = clips[i]->Duration;
			clipData.FrameRate = clips[i]->FrameRate;

			for (UINT j = 0; j < size; j++)
			{
				BlendData blendData;
				if (i == j)
				{
					blendData.bDefault = true;
					blendData.TweeningTime = 0.0f;
				}
				else
				{
					blendData.bDefault = false;
					blendData.TweeningTime = 0.1f;
				}

				blendData.End = j;
				clipData.Blends.push_back(blendData);
			}

			animData->Clips.push_back(clipData);
		}
	}

	instanceDesc = new ModelInstanceDesc();
	instanceDesc->EnableAnim = enableAnim;
	instanceDesc->BoneCount = data->BoneCount();
	instanceDesc->AnimData = animData;
	instanceDesc->FuncRelease = bind(&ModelInstancing::RemoveInstance, this, placeholders::_1);
}

ModelInstancing::~ModelInstancing()
{
	SafeDelete(animData);

	SafeDelete(instanceDesc);
	SafeDelete(compute);
	SafeDelete(instanceBuffer);
	SafeDelete(renderer);

	SafeRelease(envCubeCaster);
	SafeRelease(shadowCaster);
}


#pragma region Renders

void ModelInstancing::Render()
{
	renderer->Pass(2);
	instanceBuffer->Render();
	renderer->RenderInstance(instances.size());
}

void ModelInstancing::Render_EnvCube()
{
	renderer->Pass(4);
	instanceBuffer->Render();
	renderer->RenderInstance(instances.size());
}

void ModelInstancing::PreRender_Depth()
{
	renderer->Pass(1);
	instanceBuffer->Render();
	renderer->RenderInstance(instances.size());
}

void ModelInstancing::PreRender_EnvCube()
{
	renderer->Pass(3);
	instanceBuffer->Render();
	renderer->RenderInstance(instances.size());
}

#pragma endregion


#pragma region Updates

void ModelInstancing::Update()
{
	if (enableAnim)
	{
		for (ModelInstance* instance : instances)
			instance->Update();

		compute->Update();
	}

	renderer->Update();
}

void ModelInstancing::UpdateBoneTracking()
{
	ID3D11Texture2D* texture = compute->CopyFromOutput();
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		UINT size = instances.size();
		for (UINT i = 0; i < size; i++)
		{
			// 2의 승수로 받기 때문에 넣을 때 크기가 다를 수 있음
			// RowPitch : 1줄, DepthPitch : 1장
			BYTE* start = (BYTE*)subResource.pData + (i * subResource.RowPitch);
			instances[i]->UpdateBoneTracking((Matrix*)start);
		}
	}
	D3D::GetDC()->Unmap(texture, 0);
}

void ModelInstancing::UpdateTransforms()
{
	for (ModelInstance* instance : instances)
		instance->GetTransform()->UpdateWorld();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MODEL_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

#pragma endregion


#pragma region Instance

ModelInstance * ModelInstancing::AddInstance()
{
	ModelInstance* instance = nullptr;

	if (junkInstances.size() == 0)
	{
		UINT index = instances.size();

		instanceDesc->Id = index;
		if (enableAnim)
			instanceDesc->Blend = compute->GetDesc(index);

		instance = new ModelInstance(*instanceDesc);
		instances.push_back(instance);

		instance->GetTransform()->ReplaceMatrixGetter(worlds + index);
	}
	else
	{
		UINT index = junkInstances.back();

		instance = instances[index];

		Matrix m;
		D3DXMatrixIdentity(&m);
		instance->GetTransform()->GlobalWorld(m);

		junkInstances.pop_back();
	}

	return instance;
}

void ModelInstancing::RemoveInstance(ModelInstance * value)
{
	assert(instances[value->Id()] == value);

	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->GlobalWorld(m);

	junkInstances.push_back(value->Id());
}

#pragma endregion


// --
// MeshInstance
// --

#pragma region MeshInstance

ModelInstance::ModelInstance(const ModelInstanceDesc& desc)
	: boneCount(desc.BoneCount), blendDesc(desc.Blend)
	, id(desc.Id), funcRelease(desc.FuncRelease)
	, enableAnim(desc.EnableAnim)
{
	transform = new Transform();

	if (enableAnim)
		animator = new Animator(*desc.AnimData);
}

ModelInstance::~ModelInstance()
{
	SafeDeleteArray(bones);
	SafeDelete(animator);
	SafeDelete(transform);
}

void ModelInstance::Update()
{
	if (enableAnim)
	{
		animator->Update();
		animator->GetAnimDesc(blendDesc);
	}
}

Animator * ModelInstance::GetAnimator()
{
	return animator;
}

void ModelInstance::UpdateBoneTracking(Matrix * tracking)
{
	if (bBoneTracking == false)
	{
		bones = new Matrix[boneCount];
		bBoneTracking = true;
	}
	memcpy(bones, tracking, sizeof(Matrix) * boneCount);
}

Matrix ModelInstance::GetAttachBone(UINT boneIndex)
{
	assert(bBoneTracking);
	return bones[boneIndex];
}

#pragma endregion
