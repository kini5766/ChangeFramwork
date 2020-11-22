#include "Framework.h"
#include "ModelAnimator.h"

#include "ModelMesh.h"
#include "ModelClip.h"

using namespace ShaderEffctConstantName;

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
	perFrame = new PerFrameBuffer();
	animation = new ModelAnimation();
	materials = new MaterialGroup();

	transform->CreateBuffer();

	materials->SetConstantBuffer(CB_PerFrame, perFrame->BufferPerFrame());
	materials->SetConstantBuffer(CB_Light, perFrame->BufferLight());
	materials->SetConstantBuffer(CB_World, transform->Buffer());
	materials->SetConstantBuffer(CB_AnimationFrame, animation->Buffer());
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(clipMap);

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(animation);
	SafeDelete(perFrame);
	SafeDelete(materials);
	SafeDelete(transform);
	SafeDelete(model);
}


void ModelAnimator::Update()
{
	if (clipMap == nullptr)
		Apply();

	animation->UpdateTweening();
	perFrame->Update();
	transform->Update();

	for (ModelMesh* mesh : renderers)
		mesh->Update();
}

void ModelAnimator::Render()
{
	animation->Render();
	perFrame->Render();
	transform->Render();
	materials->Render();

	for (ModelMesh* mesh : renderers)
		mesh->Render();
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
	for (ModelMesh* mesh : renderers)
		mesh->Pass(value);
}


void ModelAnimator::Apply()
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
