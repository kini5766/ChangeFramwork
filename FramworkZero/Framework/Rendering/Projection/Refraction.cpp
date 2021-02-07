#include "Framework.h"
#include "Refraction.h"

using namespace ShaderEffectName;

Refraction::Refraction(Shader* shader, Transform * transform, float width, float height)
	: material(new ShaderSetter(shader))
	, transform(transform)
{
	refractionCamera = new PreCamera(width, height);
}

Refraction::~Refraction()
{
	SafeDelete(refractionCamera);
	SafeDelete(material);
}

void Refraction::PreRender()
{
	Vector3 position;
	transform->Position(&position);
	// 0.5f´Â ¿ï··ÀÓ Â©¸² Çö»ó
	Plane plane(0, -1, 0, position.y + 1.5f);
	//Plane plane(0, -1, 0, position.y);

	refractionCamera->PreRender();
	Context::Get()->Clipping() = plane;
}

void Refraction::Render()
{
	material->SetSRV(REFRACTION_MAP, refractionCamera->GetRenderTarget()->SRV());
	material->SetSRV(REFRACTION_DEPTH_MAP, refractionCamera->GetDepthStencil()->SRV());

	material->Render();
}
