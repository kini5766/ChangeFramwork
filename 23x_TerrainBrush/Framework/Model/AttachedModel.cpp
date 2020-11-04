#include "Framework.h"
#include "AttachedModel.h"

/*

#include "ModelMesh.h"
#include "ClipTransformMap.h"
#include "AnimationClip.h"

AttachedModel::AttachedModel(Shader * shader)
	: shader(shader)
{
	transform = new Transform(shader);

	Model* originalModel = new Model();

	attachedModels.push_back(originalModel);
	attachedMaps.push_back(nullptr);
	rendererArr2D.push_back(vector<ModelMeshBoneMap*>());
}

AttachedModel::~AttachedModel()
{
	for (auto d : attachedMaps)
		SafeDelete(d);
	for (auto d : attachedModels)
		SafeDelete(d);

	for (auto ds : rendererArr2D)
		for (auto d : ds)
			SafeDelete(d);

	SafeDelete(animator);
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
	for (ModelMeshBoneMap* mesh : rendererArr2D[selectedModel])
		mesh->Update();
}

void AttachedModel::Render()
{
	animator->Render();

	for (ModelMeshBoneMap* mesh : rendererArr2D[selectedModel])
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void AttachedModel::Pass(UINT value)
{
	for (auto renderers : rendererArr2D)
		for (ModelMeshBoneMap* mesh : renderers)
			mesh->Pass(value);
}

void AttachedModel::PlayClip(UINT clip, float speed, float takeTime)
{
	animator->PlayClip(clip, speed, takeTime);
}

void AttachedModel::ChangeOriginal()
{
	selectedModel = 0;
}

void AttachedModel::ChangeModel(UINT num)
{
	selectedModel = num;
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
	GetOriginalModel()->ReadMaterial(file);
}

void AttachedModel::ReadMesh(wstring file)
{
	GetOriginalModel()->ReadMesh(file);
}

void AttachedModel::ReadClip(wstring file)
{
	GetOriginalModel()->ReadClip(file);
}

void AttachedModel::ApplyOriginModel()
{
	Model* originalModel = GetOriginalModel();

	SafeDelete(attachedMaps[0]);
	ClipTransformMap* originalMap = new ClipTransformMap(originalModel);
	attachedMaps[0] = originalMap;

	for (Material* material : originalModel->Materials())
		material->SetShader(shader);

	ID3D11ShaderResourceView* srv = originalMap->GetSRV();
	for (MeshData* data : originalModel->Meshes())
	{
		ModelMeshBoneMap* renderer = new ModelMeshBoneMap();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(originalModel->MaterialByName(data->PBind->MaterialName));
		renderer->TransformsSRV(srv, data->PBind->BoneIndex);
		rendererArr2D[0].push_back(renderer);
	}

	SafeDelete(animator);
	animator = new AnimationClip(originalModel->Clips().data(), originalModel->ClipCount());
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

*/