#include "Framework.h"
#include "ModelAnimator.h"

using namespace ShaderEffctConstantName;

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
	perFrame = new PerFrame(shader);

	transform->CreateBuffer();
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(animation);
	SafeDelete(perFrame);
	SafeDelete(transform);
	SafeDelete(model);
}


void ModelAnimator::Update()
{
	if (animation == nullptr)
		Apply();

	perFrame->Update();
	transform->Update();
	animation->Update();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::Render()
{
	perFrame->Render();
	transform->Render();
	animation->Render();

	for (ModelMesh* mesh : model->Meshes())
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
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(value);
}


void ModelAnimator::Apply()
{
	SafeDelete(animation);
	animation = new ModelAnimationEx(model->Bones(), model->Clips());
	animation->SetShader(shader);

	for (ModelMaterial* material : model->Materials())
		material->SetShader(shader);

	for (ModelMesh* data : model->Meshes())
		data->SetMaterial(model->MaterialByName(data->Data()->PBind->MaterialName));

	PlayClip(0);
}
