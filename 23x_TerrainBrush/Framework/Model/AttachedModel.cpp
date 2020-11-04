#include "Framework.h"
#include "AttachedModel.h"

#include "ModelMesh.h"
#include "ClipTransformMap.h"
#include "ClipAnimator.h"

AttachedModel::AttachedModel(Shader * shader)
	: shader(shader)
{
	transform = new Transform(shader);
	originalModel = new Model();
}

AttachedModel::~AttachedModel()
{
	for (auto d : attachedModels)
		SafeDelete(d);
	for (auto d : attachedMaps)
		SafeDelete(d);

	SafeDelete(animator);
	SafeDelete(originalModel);
	SafeDelete(originalMap);
	SafeDelete(transform);
}

void AttachedModel::UpdateTweening()
{
	animator->UpdateTweening();
	UpdateMesh();
}

void AttachedModel::UpdateNoTweening()
{
	animator->UpdateNoTweening();
	UpdateMesh();
}

void AttachedModel::UpdateMesh()
{
	for (ModelMesh* mesh : selectedModel->Meshes())
		mesh->Update();
}

void AttachedModel::Render()
{
	animator->Render();

	for (ModelMesh* mesh : selectedModel->Meshes())
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void AttachedModel::Pass(UINT value)
{
	for (ModelMesh* mesh : originalModel->Meshes())
		mesh->Pass(value);

	for (Model* model : attachedModels)
	{
		for (ModelMesh* mesh : model->Meshes())
			mesh->Pass(value);
	}
}

void AttachedModel::PlayClip(UINT clip, float speed, float takeTime)
{
	animator->PlayClip(clip, speed, takeTime);
}

void AttachedModel::ChangeOriginal()
{
	selectedModel = originalModel;
}

void AttachedModel::ChangeModel(UINT num)
{
	selectedModel = attachedModels[num];
}

float AttachedModel::GetClipLength(UINT clip)
{
	return animator->GetClipLength(clip);
}

float AttachedModel::GetClipRunTime()
{
	return animator->GetClipRunTime();
}


void AttachedModel::ReadMaterial(wstring file)
{
	originalModel->ReadMaterial(file);
}

void AttachedModel::ReadMesh(wstring file)
{
	originalModel->ReadMesh(file);
}

void AttachedModel::ReadClip(wstring file)
{
	originalModel->ReadClip(file);
}

void AttachedModel::ApplyOriginModel()
{
	SafeDelete(originalMap);
	originalMap = new ClipTransformMap(originalModel);

	ID3D11ShaderResourceView* srv = originalMap->GetSRV();
	for (ModelMesh* mesh : originalModel->Meshes())
	{
		mesh->SetShader(shader);
		mesh->TransformsSRV(srv);
	}

	SafeDelete(animator);
	animator = new ClipAnimator(originalModel->Clips().data(), originalModel->ClipCount());
	animator->CreateBuffer(shader);

	ChangeOriginal();
}

void AttachedModel::AddAttach(Shader * shader, Model * model, int parentBoneIndex, Transform * offset)
{
	Model* newModel = nullptr;
	originalModel->Copy(&newModel);
	newModel->Attach(shader, model, parentBoneIndex, offset);

	AddAttachedModel(newModel);
}

void AttachedModel::AddAttachedModel(Model * attachedModel)
{
	ClipTransformMap* attachedMap = new ClipTransformMap(attachedModel);

	ID3D11ShaderResourceView* srv = attachedMap->GetSRV();
	for (ModelMesh* mesh : attachedModel->Meshes())
		mesh->TransformsSRV(srv);

	attachedModels.push_back(attachedModel);
	attachedMaps.push_back(attachedMap);
}
