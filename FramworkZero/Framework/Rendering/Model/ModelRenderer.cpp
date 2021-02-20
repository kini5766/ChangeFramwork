#include "Framework.h"
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(const ModelDesc & desc)
{
	defaultShader = Shader::Load(L"01_Model.fxo");

	perframe = new PerFrame();
	data = new ModelData();

	data->ReadMaterial(desc.MaterialFile);
	data->ReadMesh(desc.MeshFile);
	for (auto& file : desc.ClipFiles)
		data->ReadClip(file);

	vector<Material*>& materials = data->GetMaterials();
	for (Material* material : materials)
	{
		material->SetShader(defaultShader);
		perframe->SetAtMaterial(material);
	}

	renderer = new SkinnedMeshRenderer(data->GetRawData());
}

ModelRenderer::~ModelRenderer()
{
	SafeDelete(renderer);

	SafeDelete(data);
	SafeDelete(perframe);
}


void ModelRenderer::Update()
{
	perframe->Update();
}

void ModelRenderer::Render()
{
	perframe->Render();
	renderer->Render();
}

void ModelRenderer::RenderInstance(UINT drawCount)
{
	perframe->Render();
	renderer->RenderInstance(drawCount);
}

void ModelRenderer::SetShader(Shader * shader, wstring name)
{
	SetShader(shader, data->MaterialIndexByName(name));
}

void ModelRenderer::SetShader(Shader * shader, UINT index)
{
	data->GetMaterials()[index]->SetShader(shader);
}
