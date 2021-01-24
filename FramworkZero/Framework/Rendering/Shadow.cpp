#include "Framework.h"
#include "Shadow.h"


#pragma region ShadowCaster

ShadowCaster::ShadowCaster(Shader* s)
	: shader(new ShaderSetter(s))
{
	sShadowSampler = shader->GetShader()->AsSampler("ShadowSampler");
}

ShadowCaster::~ShadowCaster()
{
	if (desc != nullptr)
	{
		desc->FuncPreRender = []() {};
		desc->IsDestroy = true;
	}
	SafeDelete(buffer);

	SafeDelete(shader);
}

void ShadowCaster::SetShadow_Global()
{
	if (desc != nullptr) {
		desc->FuncPreRender = []() {};
		desc->IsDestroy = true;
	}
	SafeDelete(buffer);

	Shadow::AddCaster_Global(&desc);

	if (desc == nullptr)
		return;

	desc->FuncPreRender = bind(&ShadowCaster::PreRender, this);
	buffer = new ConstantBuffer(&desc->ShadowDesc, sizeof(Shadow::ShadowDesc));
	shader->SetConstantBuffer("CB_Shadow", buffer->Buffer());
	shader->SetSRV("ShadowMap", desc->DepthStencil->SRV());
}

void ShadowCaster::SetShadow(Shadow * value)
{
	if (desc != nullptr) {
		desc->FuncPreRender = []() {};
		desc->IsDestroy = true;
	}
	SafeDelete(buffer);

	value->AddCaster(&desc);

	desc->FuncPreRender = bind(&ShadowCaster::PreRender, this);
	buffer = new ConstantBuffer(&desc->ShadowDesc, sizeof(Shadow::ShadowDesc));
	shader->SetConstantBuffer("CB_Shadow", buffer->Buffer());
	shader->SetSRV("ShadowMap", desc->DepthStencil->SRV());
}

void ShadowCaster::PreRender()
{
	buffer->Render();
	shader->Render();
	sShadowSampler->SetSampler(0, desc->ShadowSampler);

	funcPreRender();
}

#pragma endregion


#pragma region Shadow

Shadow* Shadow::globalShadow = nullptr;

void Shadow::AddCaster_Global(ShadowCastDesc ** shadowcast)
{
	if (globalShadow == nullptr) return;

	globalShadow->AddCaster(shadowcast);
}

Shadow::Shadow(const Vector3 & at, float radius, float width, float height)
	: at(at)
	, radius(radius)
	, width(width)
	, height(height)
{
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

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
}

Shadow::~Shadow()
{
	for (auto d : shadowCasters)
	{
		*d->ThisPointer = nullptr;
		SafeDelete(d);
	}

	SafeDelete(viewport);
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);

	SafeRelease(shadowSampler);
}

void Shadow::PreRender()
{
	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Lighting::Get()->DirectionalDesc()->Direction;
	Vector3 eye = direction * radius * -2.0f - at;
	D3DXMatrixLookAtLH(&desc.View, &eye, &at, &up);

	Vector3 center;
	D3DXVec3TransformCoord(&center, &at, &desc.View);
	float l = center.x - radius;
	float r = center.x + radius;
	float t = center.y + radius;
	float b = center.y - radius;
	float f = center.z + radius;
	float n = center.z - radius;
	D3DXMatrixOrthoLH(&desc.Projection, r - l, t - b, n, f);

	Context::Get()->MainCamera()->GetMatrix(&desc.View);
	Context::Get()->MainCamera()->GetProjection()->GetMatrix(&desc.Projection);

	desc.MapSize = Vector2(width, height);

	list<ShadowCastDesc*>::iterator iter = shadowCasters.begin();
	while (iter != shadowCasters.end())
	{
		if ((*iter)->IsDestroy)
		{
			SafeDelete(*iter);
			shadowCasters.erase(iter++);
			continue;
		}

		(*iter)->FuncPreRender();

		++iter;
	}
}

void Shadow::ImGuiRender()
{
	//static UINT quality = 0;
	//ImGui::InputInt("Quality", (int*)&quality);
	//Quality(quality);

	//static float bias = -0.001f;
	//ImGui::DragFloat("Bias", &bias, 0.001f);
	//Bias(bias);

	ImGui::DragFloat3("At", at, 0.1f);

	ImGui::Begin("Shadow Depth");
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 v;
		v.x = vMax.x - vMin.x;
		v.y = vMax.y - vMin.y;

		ImGui::Image((void*)renderTarget->SRV(), v);
		//ImGui::Image((void*)depthStencil->SRV(), v);
	}
	ImGui::End();
}

void Shadow::AddCaster(ShadowCastDesc ** outDesc)
{
	ShadowCastDesc * casterDesc = new ShadowCastDesc();

	casterDesc->IsDestroy = false;
	casterDesc->DepthStencil = depthStencil;
	casterDesc->ShadowSampler = shadowSampler;
	casterDesc->ShadowDesc = &desc;
	casterDesc->ThisPointer = outDesc;

	*outDesc = casterDesc;
	shadowCasters.push_back(casterDesc);
}

#pragma endregion



ShadowTest::ShadowTest(Shader * s, const Vector3 & at, float radius, float width, float height)
	: shader(new ShaderSetter(s))
	, at(at)
	, radius(radius)
	, width(width)
	, height(height)
{
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	shader->SetConstantBuffer("CB_Shadow", buffer->Buffer());

	// Create Sampler State
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS;

		Check(D3D::GetDevice()->CreateSamplerState(&desc, &shadowSampler));
		sShadowSampler = shader->GetShader()->AsSampler("ShadowSampler");
	}
}

ShadowTest::~ShadowTest()
{
	SafeDelete(buffer);

	SafeDelete(viewport);
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);

	SafeRelease(shadowSampler);
}

void ShadowTest::PreRender()
{
	//Context::Get()->GetCamera()->Position(&at);
	//D3D11_DEPTH_STENCIL_DESC;

	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Lighting::Get()->DirectionalDesc()->Direction;
	Vector3 eye = direction * radius * -2.0f + at;
	D3DXMatrixLookAtLH(&desc.View, &eye, &at, &up);

	Vector3 center;
	D3DXVec3TransformCoord(&center, &at, &desc.View);
	float l = center.x - radius;
	float r = center.x + radius;
	float t = center.y + radius;
	float b = center.y - radius;
	float f = center.z + radius;
	float n = center.z - radius;
	D3DXMatrixOrthoLH(&desc.Projection, r - l, t - b, n, f);

	buffer->Render();
	shader->Render();
	sShadowSampler->SetSampler(0, shadowSampler);

	shader->SetSRV("ShadowMap", depthStencil->SRV());

	desc.MapSize = Vector2(width, height);
}

void ShadowTest::RenderImGui()
{
	ImGui::DragFloat3("At", at, 0.1f);
	ImGui::Begin("Shadow Depth");
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 v;
		v.x = vMax.x - vMin.x;
		v.y = vMax.y - vMin.y;

		ImGui::Image((void*)depthStencil->SRV(), v);
	}
	ImGui::End();
}
