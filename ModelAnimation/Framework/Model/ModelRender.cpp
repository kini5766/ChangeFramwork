#include "Framework.h"
#include "ModelRender.h"

#include "ModelMesh.h"
#include "ModelClip.h"

using namespace ShaderEffctConstantName;

ModelRender::ModelRender(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
	perFrame = new PerFrame(shader);

	transform->CreateBuffer();
}

ModelRender::~ModelRender()
{
	SafeDelete(perFrame);
	SafeDelete(transform);
	SafeDelete(model);
}

void ModelRender::Update()
{
	perFrame->Update();
	transform->Update();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelRender::Render()
{
	perFrame->Render();
	transform->Render();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Render();
}

void ModelRender::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelRender::ReadMesh(wstring file)
{
	model->ReadMesh(file);

	ApplyModel();
}

void ModelRender::Pass(UINT value)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(value);
}

void ModelRender::ApplyModel()
{
	for (Material* material : model->Materials())
		material->SetShader(shader);

	for (ModelMesh* data : model->Meshes())
		data->SetMaterial(model->MaterialByName(data->Data()->PBind->MaterialName));
}
