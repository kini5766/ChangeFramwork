#include "Framework.h"
#include "ModelRender.h"

#include "ModelMesh.h"
#include "ModelClip.h"

using namespace ShaderEffctConstantName;

ModelRender::ModelRender(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform();
	perFrame = new PerFrameBuffer();
	materials = new MaterialGroup();

	transform->CreateBuffer();

	materials->SetConstantBuffer(CB_PerFrame, perFrame->BufferPerFrame());
	materials->SetConstantBuffer(CB_Light, perFrame->BufferLight());
	materials->SetConstantBuffer(CB_World, transform->Buffer());
}

ModelRender::~ModelRender()
{
	SafeDeleteArray(boneTransforms);

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(materials);
	SafeDelete(perFrame);
	SafeDelete(transform);
	SafeDelete(model);
}

void ModelRender::Update()
{
	perFrame->Update();
	transform->Update();

	for (ModelMesh* mesh : renderers)
		mesh->Update();
}

void ModelRender::Render()
{
	perFrame->Render();
	transform->Render();
	materials->Render();

	for (ModelMesh* mesh : renderers)
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
	for (ModelMesh* mesh : renderers)
		mesh->Pass(value);
}

void ModelRender::UpdateTransform(ModelBone * bone, const Matrix & matrix)
{
	if (bone != nullptr)
		UpdateBones(bone, matrix);

	SafeDeleteArray(boneTransforms);
	boneCount = model->BoneCount();
	boneTransforms = new Matrix[boneCount];

	for (UINT i = 0; i < boneCount; i++)
	{
		ModelBone* bone = model->BoneByIndex(i);
		memcpy(&boneTransforms[i], &bone->Transform(), sizeof(Matrix));
	}

	//for (ModelMesh* mesh : renderers)
	//	mesh->BoneTransform(&boneTransforms[mesh->BoneIndex()]);
}

void ModelRender::UpdateBones(ModelBone * bone, const Matrix & matrix)
{
	Matrix temp = bone->Transform();
	bone->Transform(temp * matrix);  // 메트릭스 부모 자식 관계

	// 본에 있는 자식들 재귀 넣기
	for (ModelBone* child : bone->Childs())
		UpdateBones(child, matrix);
}

void ModelRender::ApplyModel()
{
	for (Material* material : model->Materials())
	{
		material->SetShader(shader);
		materials->AddMaterial(material);
	}

	for (ModelMeshData* data : model->Meshes())
	{
		ModelMesh* renderer = new ModelMesh();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(model->MaterialByName(data->PBind->MaterialName));
		renderers.push_back(renderer);
	}

	UpdateTransform();
}
