#include "Framework.h"
#include "ModelAttacher.h"

#include "ModelMesh.h"
#include "ModelClip.h"


ModelAttacher::ModelAttacher(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
}

ModelAttacher::~ModelAttacher()
{
	for (auto item : attachedItems)
	{
		SafeDelete(item->ClipMap);

		for (auto d : item->Renderers)
			SafeDelete(d);

		SafeDelete(item->Model);
		SafeDelete(item);
	}

	SafeDelete(clipMap);
	SafeDelete(animation);

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(transform);
	SafeDelete(model);
}

void ModelAttacher::Update()
{
	animation->UpdateNoTweening();

	for (ModelMeshClipMap* mesh : renderers)
		mesh->Update();

	for (AttachedItem* item : attachedItems)
	{
		if (item->IsActiveSelf == false) continue;

		for (ModelMeshClipMap* mesh : item->Renderers)
			mesh->Update();
	}
}

void ModelAttacher::Render()
{
	animation->Render();

	for (ModelMeshClipMap* mesh : renderers)
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}

	for (AttachedItem* item : attachedItems)
	{
		if (item->IsActiveSelf == false) continue;

		for (ModelMeshClipMap* mesh : item->Renderers)
		{
			mesh->SetTransform(transform);
			mesh->Render();
		}
	}
}

void ModelAttacher::PlayClip(UINT clip, float speed, float takeTime)
{
	animation->PlayClip(clip, speed, takeTime);
}

void ModelAttacher::AttachItem(Model * item, int parentBoneIndex, Transform * offset)
{
	model->AttachWeakly(item, parentBoneIndex, offset);

	for (Material* material : item->Materials())
		material->SetShader(shader);

	AttachedItem* newItem = new AttachedItem();
	newItem->Model = item;

	ModelClipMap* newMap = new ModelClipMap(item);
	newItem->ClipMap = newMap;

	ID3D11ShaderResourceView* srv = newMap->GetSRV();
	for (MeshData* data : item->Meshes())
	{
		ModelMeshClipMap* renderer = new ModelMeshClipMap();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(item->MaterialByName(data->PBind->MaterialName));
		renderer->TransformsSRV(srv, data->PBind->BoneIndex);
		newItem->Renderers.push_back(renderer);
	}

	attachedItems.push_back(newItem);
}

void ModelAttacher::SetActiveItem(UINT index, bool active)
{
	attachedItems[index]->IsActiveSelf = active;
}

void ModelAttacher::OnlyOrigin()
{
	for (AttachedItem* item : attachedItems)
		item->IsActiveSelf = false;
}


void ModelAttacher::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAttacher::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelAttacher::ReadClip(wstring file)
{
	model->ReadClip(file);
}

void ModelAttacher::ApplyOriginModel()
{
	for (Material* material : model->Materials())
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


void ModelAttacher::Pass(UINT value)
{
	for (ModelMeshClipMap* mesh : renderers)
		mesh->Pass(value);

	for (AttachedItem* item : attachedItems)
		for (ModelMeshClipMap* mesh : item->Renderers)
			mesh->Pass(value);
}

float ModelAttacher::GetClipLength(UINT clip)
{
	return animation->GetClipLength(clip);
}

float ModelAttacher::GetClipRunTime()
{
	return animation->GetClipRunTime();
}
