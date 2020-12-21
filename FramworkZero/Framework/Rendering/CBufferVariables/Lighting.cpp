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
	light = new DirectionalLight();
}

Lighting::~Lighting()
{
	SafeDelete(light);
}
