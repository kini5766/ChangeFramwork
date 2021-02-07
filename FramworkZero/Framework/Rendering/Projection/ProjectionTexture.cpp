#include "Framework.h"
#include "ProjectionTexture.h"

using namespace ShaderEffectName;

ProjectionTexture::ProjectionTexture(Shader * s, wstring mapFile, float width, float height)
	: shader(new ShaderSetter(s))
	, map(new Texture(mapFile))
{
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	camera = new Fixity();
	camera->ResizeScreen(width, height);

	camera->GetTransform()->Position(0, 100, 0);
	camera->GetTransform()->RotationEuler(EulerAngle::Degree(90, 0, 0));

	shader->SetSRV(PROJECTION_MAP, map->SRV());
	shader->SetConstantBuffer(CB_PROJECTIONTEXTURE, buffer->Buffer());
}

ProjectionTexture::~ProjectionTexture()
{
	SafeDelete(camera);
	SafeDelete(buffer);
	SafeDelete(shader);
	SafeDelete(map);
}

void ProjectionTexture::Update()
{
	CameraTransform* view = camera->GetTransform();

	Vector3 position;
	view->Position(&position);

	ImGui::SliderFloat3("Position", position, -512, 512);
	view->Position(position);

	ImGui::ColorEdit3("Color", desc.Color);

	view->GetView(&desc.View);
	camera->GetProjection()->GetMatrix(&desc.Projection);
}

void ProjectionTexture::Render()
{
	buffer->Render();
	shader->Render();
}
