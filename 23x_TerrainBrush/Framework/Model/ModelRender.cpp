#include "Framework.h"
#include "ModelRender.h"

#include "ModelMesh.h"

ModelRender::ModelRender(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform();
}

ModelRender::~ModelRender()
{
	SafeDeleteArray(boneTransforms);

	for (auto d : renderers)
		SafeDelete(d);

	SafeDelete(transform);
	SafeDelete(model);
}

void ModelRender::Update()
{
	for (ModelMesh* mesh : renderers)
		mesh->Update();
}

void ModelRender::Render()
{
	for (ModelMesh* mesh : renderers)
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void ModelRender::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);

	for (Material* material : model->Materials())
		material->SetShader(shader);
}

void ModelRender::ReadMesh(wstring file)
{
	model->ReadMesh(file);

	for (ModelMeshData* data : model->Meshes())
	{
		ModelMesh* renderer = new ModelMesh();
		renderer->CreateBuffer(data);
		renderer->SetMaterial(model->MaterialByName(data->PBind->MaterialName));
		renderers.push_back(renderer);
	}

	UpdateTransform();
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

	for (ModelMesh* mesh : renderers)
		mesh->BoneTransform(&boneTransforms[mesh->BoneIndex()]);
}

void ModelRender::UpdateBones(ModelBone * bone, const Matrix & matrix)
{
	Matrix temp = bone->Transform();
	bone->Transform(temp * matrix);  // 메트릭스 부모 자식 관계

	// 본에 있는 자식들 재귀 넣기
	for (ModelBone* child : bone->Childs())
		UpdateBones(child, matrix);
}
