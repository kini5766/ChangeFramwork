#include "Framework.h"
#include "Shadow.h"

#include "Tools/ImGui/ImGuiSrvViewer.h"
#include "Rendering/Camera/PreRender/ShadowCamera.h"


Shadow::Shadow(const Vector3 & at, float radius, float width, float height)
{
	// Create Sampler State
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1.0f;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS;

		Check(D3D::GetDevice()->CreateSamplerState(&desc, &shadowSampler));
	}

	camera = new ShadowCamera(at, radius, width, height);
	output = new PreShadowValue();

	desc.MapSize = Vector2(width, height);

	output->Shadow = &desc;
	output->ShadowSampler = shadowSampler;
	output->DepthStencil = camera->GetDepth();
}

Shadow::~Shadow()
{
	SafeDelete(output);
	SafeDelete(camera);
	SafeRelease(shadowSampler);
}

void Shadow::Update()
{
	camera->Update();
	camera->GetView(&desc.View);
	camera->GetProjection(&desc.Projection);
}

void Shadow::PreRender()
{
	camera->PreRender();
	Context::Get()->PreRenderShadow(output);
}


void Shadow::ImGuiRender()
{
	static UINT quality = 2;
	ImGui::InputInt("Quality", (int*)&quality);
	desc.Quality = quality;

	static float bias = -0.001f;
	ImGui::DragFloat("Bias", &bias, 0.001f);
	desc.Bias = bias;

	Vector3 at = camera->At();
	ImGui::DragFloat3("At", at, 0.1f);
	camera->At(at);

	ImGuiSrvViewer::Render("Shadow Depth", camera->GetDepth()->SRV());
}

void Shadow::At(const Vector3 & value)
{
	camera->At(value);
}
