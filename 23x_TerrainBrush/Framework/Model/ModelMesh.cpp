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

	//SafeDeleteArray(vertices);
	//SafeDeleteArray(indices);
	
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
	vertexBuffer = new VertexBuffer(mesh->Vertices, mesh->VertexCount, sizeof(Model::ModelVertex));

	SafeDelete(indexBuffer);
	indexBuffer = new IndexBuffer(mesh->Indices, mesh->IndexCount);
}

void ModelMesh::SetMaterial(Material * mat)
{
	material = mat;

	shader = material->GetShader();

	SafeDelete(transform);
	transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	material->SetShader(shader);

	sBoneBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_Bone);
}

void ModelMesh::BindBone(ModelBone * bone)
{
	boneDesc.BoneIndex = bone->Index();
	BoneTransform(&bone->Transform());
}

void ModelMesh::Update()
{
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

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, mesh->IndexCount);
}

void ModelMesh::BoneTransform(Matrix * value)
{
	memcpy(boneDesc.Transform, value, sizeof(Matrix));
}

void ModelMesh::SetTransform(Transform * value)
{
	transform->Set(value);
}


// ------------------------------------------------------------------------------------------------
// ModelMeshBaked
// ------------------------------------------------------------------------------------------------

ModelMeshBoneMap::ModelMeshBoneMap()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMeshBoneMap::~ModelMeshBoneMap()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	//SafeDeleteArray(vertices);
	//SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);
}

/*
void ModelMeshBoneMap::CreateBuffer(MeshData * mesh)
{
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

void ModelMeshBoneMap::CreateBuffer(ModelMeshData * _mesh)
{
	mesh = _mesh;

	SafeDelete(vertexBuffer);
	vertexBuffer = new VertexBuffer(mesh->Vertices, mesh->VertexCount, sizeof(Model::ModelVertex));

	SafeDelete(indexBuffer);
	indexBuffer = new IndexBuffer(mesh->Indices, mesh->IndexCount);
}

void ModelMeshBoneMap::SetMaterial(Material * mat)
{
	material = mat;

	shader = material->GetShader();

	SafeDelete(transform);
	transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	material->SetShader(shader);

	sBoneBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_Bone);

	sTransformsSRV = shader->AsSRV("TransformsMap");
}

void ModelMeshBoneMap::Update()
{
	perFrame->Update();
	transform->Update();
}

void ModelMeshBoneMap::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();
	transform->Render();
	material->Render();

	sTransformsSRV->SetResource(transformsSRV);

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, mesh->IndexCount);
}

void ModelMeshBoneMap::SetTransform(Transform * value)
{
	transform->Set(value);
}

void ModelMeshBoneMap::TransformsSRV(ID3D11ShaderResourceView * value, UINT boneIndex)
{
	boneDesc.BoneIndex = boneIndex;
	transformsSRV = value;
}
