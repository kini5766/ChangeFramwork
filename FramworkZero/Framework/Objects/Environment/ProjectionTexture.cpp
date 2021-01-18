#include "Framework.h"
#include "ProjectionTexture.h"

ProjectionTexture::ProjectionTexture(Shader * s, wstring mapFile, float width, float height)
	: shader(new ShaderSetter(s))
	, map(new Texture(mapFile))
{
	camera = new Fixity();
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	camera->ResizeScreen(width, height);

	camera->Position(0, 30, 0);
	camera->RotationDegree(90, 0, 0);
	shader->SetSRV("ProjectionMap", map->SRV());
	shader->SetConstantBuffer("CB_ProjectionTexture", buffer->Buffer());
}

ProjectionTexture::~ProjectionTexture()
{
	SafeDelete(buffer);
	SafeDelete(camera);
	SafeDelete(map);
	SafeDelete(shader);
}

void ProjectionTexture::Update()
{
	Vector3 position;
	camera->Position(&position);

	ImGui::SliderFloat3("Position", position, -100, 100);
	camera->Position(position);

	ImGui::ColorEdit3("Color", desc.Color);

	camera->GetMatrix(&desc.View);
	camera->GetProjection()->GetMatrix(&desc.Projection);
}

void ProjectionTexture::Render()
{
	buffer->Render();
	shader->Render();
}
