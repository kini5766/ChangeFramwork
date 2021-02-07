#include "Framework.h"
#include "Shadow.h"


#pragma region Shadow

Shadow* Shadow::globalShadow = nullptr;

ShadowCastDesc * Shadow::AddCaster_Global()
{
	if (globalShadow == nullptr) return nullptr;
	return globalShadow->AddCaster();
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
	Context::Get()->PreRenderMain();
	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Lighting::Get()->DirectionalDesc()->Direction;
	Vector3 eye = direction * radius * -2.0f;
	eye += at;
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

		(*iter)->FuncPreRender(&desc);

		++iter;
	}
}

void Shadow::ImGuiRender()
{
	static UINT quality = 0;
	ImGui::InputInt("Quality", (int*)&quality);
	Quality(quality);

	static float bias = -0.001f;
	ImGui::DragFloat("Bias", &bias, 0.001f);
	Bias(bias);

	ImGui::DragFloat3("At", at, 0.1f);

	ImGui::Begin("Shadow Depth");
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 v;
		v.x = vMax.x - vMin.x;
		v.y = vMax.y - vMin.y;

		//ImGui::Image((void*)renderTarget->SRV(), v);
		ImGui::Image((void*)depthStencil->SRV(), v);
	}
	ImGui::End();
}

ShadowCastDesc * Shadow::AddCaster()
{
	ShadowCastDesc * casterDesc = new ShadowCastDesc();

	casterDesc->DepthStencil = depthStencil;
	casterDesc->ShadowSampler = shadowSampler;

	casterDesc->IsDestroy = false;
	casterDesc->ThisPointer = nullptr;
	casterDesc->FuncPreRender = [](ShadowDesc*) {};

	shadowCasters.push_back(casterDesc);
	return casterDesc;
}

#pragma endregion


#pragma region ShadowCaster

ShadowCaster::ShadowCaster(Shader* s)
	: material(new ShaderSetter(s))
{
	buffer = new ConstantBuffer(&shadowDesc, sizeof(ShadowDesc));
	sShadowSampler = material->GetShader()->AsSampler("ShadowSampler");

	material->SetConstantBuffer("CB_Shadow", buffer->Buffer());
}

ShadowCaster::~ShadowCaster()
{
	if (desc != nullptr)
	{
		desc->FuncPreRender = [](ShadowDesc*) {};
		desc->IsDestroy = true;
	}

	SafeDelete(buffer);
	SafeDelete(material);
}

void ShadowCaster::SetShadow_Global()
{
	SafeDeleteDesc();
	desc = Shadow::AddCaster_Global();
	SetDesc();
}

void ShadowCaster::SetShadow(Shadow * value)
{
	SafeDeleteDesc();
	desc = value->AddCaster();
	SetDesc();
}

void ShadowCaster::PreRender(ShadowDesc * shadow)
{
	memcpy(&shadowDesc, shadow, sizeof(ShadowDesc));
	buffer->Render();
	
	material->SetSRV("ShadowMap", desc->DepthStencil->SRV());
	material->Render();

	sShadowSampler->SetSampler(0, desc->ShadowSampler);

	funcPreRender();
}

void ShadowCaster::SafeDeleteDesc()
{
	if (desc == nullptr)
		return;

	desc->FuncPreRender = [](ShadowDesc*) {};
	desc->IsDestroy = true;
}

void ShadowCaster::SetDesc()
{
	if (desc == nullptr)
		return;

	desc->FuncPreRender = bind(&ShadowCaster::PreRender, this, placeholders::_1);
	depth = desc->DepthStencil;
	desc->ThisPointer = &desc;
}

#pragma endregion

