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
	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);
}

/*
void ModelMesh::CreateBuffer(MeshData * mesh)
{
	BindBone(mesh->PBind->PBone);

	SafeDeleteArray(vertexBuffer);
	vertexCount = mesh->VertexCount;
	vertices = new Model::ModelVertex[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		memcpy(&vertices[i].Position, &mesh->Positions[i], sizeof(Vector3));
		memcpy(&vertices[i].Uv, &mesh->Uvs[i], sizeof(Vector2));
		memcpy(&vertices[i].Normal, &mesh->Normals[i], sizeof(Vector3));
		memcpy(&vertices[i].Tangent, &mesh->Tangents[i], sizeof(Vector3));
		memcpy(&vertices[i].BlendIndices, &mesh->BoneWeights[i].BlendIndices, sizeof(Vector4));
		memcpy(&vertices[i].BlendWeights, &mesh->BoneWeights[i].BlendWeights, sizeof(Vector4));
	}

	SafeDelete(vertexBuffer);
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::ModelVertex));


	SafeDeleteArray(indices);
	indexCount = mesh->IndexCount;
	indices = new UINT[indexCount];
	memcpy(indices, mesh->Indices, indexCount * sizeof(UINT));

	SafeDelete(indexBuffer);
	indexBuffer = new IndexBuffer(indices, indexCount);
}
*/

void ModelMesh::CreateBuffer(ModelMeshData * _mesh)
{
	mesh = _mesh;
	BindBone(mesh->PBind->PBone);

	SafeDelete(vertexBuffer);
	vertexBuffer = new VertexBuffer(mesh->Vertices, mesh->VertexCount, mesh->Stride);

	SafeDelete(indexBuffer);
	indexBuffer = new IndexBuffer(mesh->Indices, mesh->IndexCount);
}

void ModelMesh::SetMaterial(ModelMaterial * mat)
{
	material = mat;
	shader = material->GetShader();
	sBoneBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_Bone);
}

void ModelMesh::BindBone(ModelBone * bone)
{
	SetBoneIndex(bone->Index());
	BoneTransform(&bone->Transform());
}

void ModelMesh::Update()
{
}

void ModelMesh::Render()
{
	vertexBuffer->Render();
	indexBuffer->Render();

	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	material->Render();
	shader->DrawIndexed(0, pass, mesh->IndexCount);
}

void ModelMesh::BoneTransform(Matrix * value)
{
	memcpy(boneDesc.Transform, value, sizeof(Matrix));
}

void ModelMesh::SetBoneIndex(UINT boneIndex)
{
	boneDesc.BoneIndex = boneIndex;
}
