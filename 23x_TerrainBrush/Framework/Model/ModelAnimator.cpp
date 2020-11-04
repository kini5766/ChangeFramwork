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
	SafeDelete(animator);
	SafeDelete(transfromMap);

	SafeDelete(transform);
	SafeDelete(model);
}


void ModelAnimator::Update()
{
	if (animator == nullptr)
		Apply();

	animator->UpdateTweening();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::Render()
{
	animator->Render();

	for (ModelMesh* mesh : model->Meshes())
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
	SafeDelete(transfromMap);
	transfromMap = new ClipTransformMap(model);

	ID3D11ShaderResourceView* srv = transfromMap->GetSRV();
	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->SetShader(shader);
		mesh->TransformsSRV(srv);
	}

	SafeDelete(animator);
	animator = new ClipAnimator(model->Clips().data(), model->ClipCount());
	animator->CreateBuffer(shader);
}

void ModelAnimator::Pass(UINT value)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(value);
}
