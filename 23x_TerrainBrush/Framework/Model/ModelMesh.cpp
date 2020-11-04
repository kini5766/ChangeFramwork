#include "Framework.h"
#include "ModelMesh.h"

// ------------------------------------------------------------------------------------------------
// ModelBone
// ------------------------------------------------------------------------------------------------

ModelBone::ModelBone()
{
}

ModelBone::~ModelBone()
{
}


// ------------------------------------------------------------------------------------------------
// ModelMesh
// ------------------------------------------------------------------------------------------------

ModelMesh::ModelMesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMesh::~ModelMesh()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
	
	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);
	SafeDelete(material);
}

void ModelMesh::Binding(Model * model)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::ModelVertex));
	indexBuffer = new IndexBuffer(indices, indexCount);

	Material* srcMaterial = model->MaterialByName(materialName);
	material = new Material();
	material->Ambient(srcMaterial->Ambient());
	material->Diffuse(srcMaterial->Diffuse());
	material->Specular(srcMaterial->Specular());

	if (srcMaterial->DiffuseMap() != nullptr)
		material->DiffuseMap(srcMaterial->DiffuseMap()->GetFile());

	if (srcMaterial->SpecularMap() != nullptr)
		material->SpecularMap(srcMaterial->SpecularMap()->GetFile());

	if (srcMaterial->NormalMap() != nullptr)
		material->NormalMap(srcMaterial->NormalMap()->GetFile());
}

void ModelMesh::SetShader(Shader * value)
{
	shader = value;

	SafeDelete(transform);
	transform = new Transform(value);

	SafeDelete(perFrame);
	perFrame = new PerFrame(value);

	material->SetShader(value);

	sBoneBuffer = value->AsConstantBuffer(ShaderEffctConstantName::CB_Bone);

	sTransformsSRV = value->AsSRV("TransformsMap");
}


void ModelMesh::Update()
{
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	transform->Update();
}

void ModelMesh::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();
	transform->Render();
	material->Render();

	if (nullptr != sTransformsSRV)
		sTransformsSRV->SetResource(transformsSRV);

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, indexCount);
}

void ModelMesh::BoneTransform(Matrix * value)
{
	memcpy(boneDesc.Transform, value, sizeof(Matrix));
}

void ModelMesh::SetTransform(Transform * value)
{
	transform->Set(value);
}

void ModelMesh::TransformsSRV(ID3D11ShaderResourceView * value)
{
	transformsSRV = value;
}
