#include "Framework.h"
#include "Material.h"

Material::Material()
{
	Initialize();
}

Material::Material(Shader * _shader)
{
	Initialize();

	SetShader(_shader);
}

void Material::Initialize()
{
	name = L"";

	diffuseMap = nullptr;
	specularMap = nullptr;
	normalMap = nullptr;

	buffer = new ConstantBuffer(&colorDesc, sizeof(ColorDesc));
}


Material::~Material()
{
	SafeDelete(buffer);

	SafeDelete(diffuseMap);
	SafeDelete(specularMap);
	SafeDelete(normalMap);
}

void Material::SetShader(Shader * value)
{
	shader = value;

	sBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_Material);

	sDiffuseMap = shader->AsSRV("DiffuseMap");
	sSpecularMap = shader->AsSRV("SpecularMap");
	sNormalMap = shader->AsSRV("NormalMap");
}

void Material::Ambient(const Color & value)
{
	colorDesc.Ambient = value;
}

void Material::Ambient(float r, float g, float b, float a)
{
	Ambient(Color(r, g, b, a));
}

void Material::Diffuse(const Color & value)
{
	colorDesc.Diffuse = value;
}

void Material::Diffuse(float r, float g, float b, float a)
{
	Diffuse(Color(r, g, b, a));
}

void Material::Specular(const Color & value)
{
	colorDesc.Specular = value;
}

void Material::Specular(float r, float g, float b, float a)
{
	Specular(Color(r, g, b, a));
}

void Material::DiffuseMap(string file)
{
	DiffuseMap(String::ToWString(file));
}

void Material::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);
	diffuseMap = new Texture(file);
}

void Material::SpecularMap(string file)
{
	SpecularMap(String::ToWString(file));
}

void Material::SpecularMap(wstring file)
{
	SafeDelete(specularMap);
	specularMap = new Texture(file);
}

void Material::NormalMap(string file)
{
	NormalMap(String::ToWString(file));
}

void Material::NormalMap(wstring file)
{
	SafeDelete(normalMap);
	normalMap = new Texture(file);
}

void Material::Render()
{
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	if (diffuseMap != nullptr)
		sDiffuseMap->SetResource(diffuseMap->SRV());
	else
		sDiffuseMap->SetResource(nullptr);

	if (specularMap != nullptr)
		sSpecularMap->SetResource(specularMap->SRV());
	else
		sSpecularMap->SetResource(nullptr);

	if (normalMap != nullptr)
		sNormalMap->SetResource(normalMap->SRV());
	else
		sNormalMap->SetResource(nullptr);
}
