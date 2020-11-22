#include "Framework.h"
#include "ModelAttacher.h"

#include "ModelMesh.h"
#include "ModelClip.h"

using namespace ShaderEffctConstantName;

ModelAttacher::ModelAttacher(Shader * shader)
	: shader(shader)
{
	model = new Model();
	perFrame = new PerFrameBuffer();
	animation = new ModelAnimation();
	materials = new MaterialGroup();

	transform->CreateBuffer();

	materials->SetConstantBuffer(CB_PerFrame, perFrame->BufferPerFrame());
	materials->SetConstantBuffer(CB_Light, perFrame->BufferLight());
	materials->SetConstantBuffer(CB_World, transform->Buffer());
	materials->SetConstantBuffer(CB_AnimationFrame, animation->Buffer());
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

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(animation);
	SafeDelete(perFrame);
	SafeDelete(materials);
	SafeDelete(transform);
	SafeDelete(model);
}

void ModelAttacher::Update()
{
	animation->UpdateNoTweening();
	perFrame->Update();
	transform->Update();

	for (ModelMesh* mesh : renderers)
		mesh->Update();

	for (AttachedItem* item : attachedItems)
	{
		if (item->IsActiveSelf == false) continue;

		for (ModelMesh* mesh : item->Renderers)
			mesh->Update();
	}
}

void ModelAttacher::Render()
{
	animation->Render();
	perFrame->Render();
	transform->Render();
	materials->Render();

	for (ModelMesh* mesh : renderers)
		mesh->Render();

	for (AttachedItem* item : attachedItems)
	{
		if (item->IsActiveSelf == false) continue;

		for (ModelMesh* mesh : item->Renderers)
			mesh->Render();
	}
}

void ModelAttacher::PlayClip(UINT clip, float speed, float takeTime)
{
	animation->PlayClip(clip, speed, takeTime);
}

void ModelAttacher::AttachItem(Model * item, int parentBoneIndex, Transform * offset)
{
	model->AttachWeakly(item, parentBoneIndex, offset);

	for (ModelMaterial* material : item->Materials())
		material->SetShader(shader);

	AttachedItem* newItem = new AttachedItem();
	newItem->Model = item;

	ModelClipMap* newMap = new ModelClipMap(item);
	newItem->ClipMap = newMap;

	for (ModelMeshData* data : item->Meshes())
	{
		ModelMesh* renderer = new ModelMesh();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(item->MaterialByName(data->PBind->MaterialName));
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
	for (ModelMaterial* material : model->Materials())
	{
		material->SetShader(shader);
		materials->AddMaterial(material);
	}

	SafeDelete(clipMap);
	clipMap = new ModelClipMap(model);
	materials->SetSRV("TransformsMap", clipMap->GetSRV());
	animation->SetClips(model->Clips().data(), model->ClipCount());

	for (ModelMeshData* data : model->Meshes())
	{
		ModelMesh* renderer = new ModelMesh();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(model->MaterialByName(data->PBind->MaterialName));
		renderers.push_back(renderer);
	}
}


void ModelAttacher::Pass(UINT value)
{
	for (ModelMesh* mesh : renderers)
		mesh->Pass(value);

	for (AttachedItem* item : attachedItems)
		for (ModelMesh* mesh : item->Renderers)
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
