#include "Framework.h"
#include "ModelMesh.h"

// ------------------------------------------------------------------------------------------------
// ModelBone
// ------------------------------------------------------------------------------------------------

ModelBone::ModelBone()
{
	data = new ModelBoneData();
	transform = new TransformData(&boneDesc.Transform);
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelBone::~ModelBone()
{
	SafeDelete(boneBuffer);
	SafeDelete(transform);
	SafeDelete(data);
}

void ModelBone::Render()
{
	transform->UpdateWorld();
	boneBuffer->Render();
}

void ModelBone::BindBone(ModelBone * newParent)
{
	boneDesc.BoneIndex = data->Index;

	ModelBone * oldParent = parent;
	if (oldParent != nullptr)
	{
		vector<ModelBone*>::iterator iter;
		for (iter = oldParent->childs.begin(); iter != oldParent->childs.end(); iter++)
		{
			if ((*iter) == this)
			{
				oldParent->childs.erase(iter);
				break;
			}
		}

		transform->SetParent(nullptr);
	}

	if (newParent != nullptr)
	{
		newParent->childs.push_back(this);
		transform->SetParent(newParent->transform);
	}

	parent = newParent;
	transform->LossyWorld(data->Transform);
}


// ------------------------------------------------------------------------------------------------
// ModelMesh
// ------------------------------------------------------------------------------------------------

using namespace ShaderEffctConstantName;

ModelMesh::ModelMesh()
{
	data = new ModelMeshData();
}

ModelMesh::~ModelMesh()
{
	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);
	data->SafeDeleteData();
	SafeDelete(data);
}

void ModelMesh::Data(const ModelMeshData * value)
{
	ModelMeshData::Copy<Model::ModelVertex>(data, value);

	SafeDelete(indexBuffer);
	indexBuffer = new IndexBuffer(data->Indices, data->IndexCount);

	SafeDelete(vertexBuffer);
	vertexBuffer = new VertexBuffer(data->Vertices, data->VertexCount, data->Stride);
}

void ModelMesh::SetMaterial(ModelMaterial * mat)
{
	material = mat;
	shader = material->GetShader();
	sBoneBuffer = shader->AsConstantBuffer(CB_Bone);
}

void ModelMesh::Update()
{
}

void ModelMesh::Render()
{
	vertexBuffer->Render();
	indexBuffer->Render();

	data->PBind->PBone->Render();
	sBoneBuffer->SetConstantBuffer(data->PBind->PBone->Buffer());

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	material->Render();
	shader->DrawIndexed(0, pass, data->IndexCount);
}
