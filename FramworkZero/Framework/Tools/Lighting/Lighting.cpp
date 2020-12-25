#include "Framework.h"
#include "Lighting.h"

Lighting* Lighting::instance = nullptr;

Lighting * Lighting::Get()
{
	return instance;
}

void Lighting::Create()
{
	SafeDelete(instance);
	instance = new Lighting();
}

void Lighting::Delete()
{
	SafeDelete(instance);
}

Lighting::Lighting()
{
	directional = new DirectionalLight(&directionalDesc);
}

Lighting::~Lighting()
{
	for (auto d : pointLights)
		SafeDelete(d);
	for (auto d : spotLights)
		SafeDelete(d);
	SafeDelete(directional);
}


// --
// PointLight
// --

void Lighting::AddPointLight(const PointLightDesc & light)
{
	pointsDesc.Lights[pointsDesc.Count++] = light;

	PointLight* result = new PointLight(pointsDesc.Lights + pointsDesc.Count - 1);
	pointLights.push_back(result);
}

PointLight * Lighting::GetPointLight(UINT index)
{
	return pointLights[index];
}


// --
// SpotLight
// --

void Lighting::AddSpotLight(const SpotLightDesc & light)
{
	spotsDesc.Lights[spotsDesc.Count++] = light;

	SpotLight* result = new SpotLight(spotsDesc.Lights + spotsDesc.Count - 1);
	spotLights.push_back(result);
}

SpotLight * Lighting::GetSpotLight(UINT index)
{
	return spotLights[index];
}

