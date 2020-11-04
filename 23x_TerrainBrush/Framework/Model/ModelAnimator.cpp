#include "Framework.h"
#include "ModelAnimator.h"

#include "ModelMesh.h"
#include "ClipTransformMap.h"
#include "ClipAnimator.h"

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(transfromMap);
	SafeDelete(animator);

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(transform);
	SafeDelete(model);
}


void ModelAnimator::Update()
{
	if (animator == nullptr)
		Apply();

	animator->UpdateTweening();

	for (ModelMeshBoneMap* mesh : renderers)
		mesh->Update();
}

void ModelAnimator::Render()
{
	animator->Render();

	for (ModelMeshBoneMap* mesh : renderers)
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void ModelAnimator::PlayClip(UINT clip, float speed, float takeTime)
{
	animator->PlayClip(clip, speed, takeTime);
}


void ModelAnimator::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelAnimator::ReadClip(wstring file)
{
	model->ReadClip(file);
}


void ModelAnimator::Apply()
{
	for (Material* material : model->Materials())
		material->SetShader(shader);

	SafeDelete(transfromMap);
	transfromMap = new ClipTransformMap(model);
	ID3D11ShaderResourceView* srv = transfromMap->GetSRV();
	
	for (ModelMeshData* data : model->Meshes())
	{
		ModelMeshBoneMap* renderer = new ModelMeshBoneMap();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(model->MaterialByName(data->PBind->MaterialName));
		renderer->TransformsSRV(srv, data->PBind->BoneIndex);
		renderers.push_back(renderer);
	}
	
	SafeDelete(animator);
	animator = new ClipAnimator(model->Clips().data(), model->ClipCount());
	animator->CreateBuffer(shader);
}

void ModelAnimator::Pass(UINT value)
{
	for (ModelMeshBoneMap* mesh : renderers)
		mesh->Pass(value);
}
