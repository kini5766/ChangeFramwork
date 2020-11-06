#include "Framework.h"
#include "ModelAnimator.h"

#include "ModelMesh.h"
#include "ModelClip.h"

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(clipMap);
	SafeDelete(animation);

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(transform);
	SafeDelete(model);
}


void ModelAnimator::Update()
{
	if (animation == nullptr)
		Apply();

	animation->UpdateTweening();

	for (ModelMeshClipMap* mesh : renderers)
		mesh->Update();
}

void ModelAnimator::Render()
{
	animation->Render();

	for (ModelMeshClipMap* mesh : renderers)
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void ModelAnimator::PlayClip(UINT clip, float speed, float takeTime)
{
	animation->PlayClip(clip, speed, takeTime);
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

void ModelAnimator::Pass(UINT value)
{
	for (ModelMeshClipMap* mesh : renderers)
		mesh->Pass(value);
}


void ModelAnimator::Apply()
{
	for (ModelMaterial* material : model->Materials())
		material->SetShader(shader);

	SafeDelete(clipMap);
	clipMap = new ModelClipMap(model);
	ID3D11ShaderResourceView* srv = clipMap->GetSRV();
	
	for (MeshData* data : model->Meshes())
	{
		ModelMeshClipMap* renderer = new ModelMeshClipMap();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(model->MaterialByName(data->PBind->MaterialName));
		renderer->TransformsSRV(srv, data->PBind->BoneIndex);
		renderers.push_back(renderer);
	}
	
	SafeDelete(animation);
	animation = new ModelAnimation(model->Clips().data(), model->ClipCount());
	animation->CreateBuffer(shader);
}
