#include "Framework.h"
#include "Material.h"

using namespace ShaderEffectName;

Material::Material()
{
	Initialize();
}

Material::Material(Shader * shader)
{
	Initialize();

	SetShader(shader);
}

Material::~Material()
{
	SafeDelete(diffuseMap);
	SafeDelete(specularMap);
	SafeDelete(normalMap);

	SafeDelete(buffer);
}


void Material::Render()
{
	buffer->Render();
	ShaderSetter::Render();
}

void Material::SetShader(Shader * value)
{
	ShaderSetter::SetShader(value);
}


void Material::Ambient(const Color & color)
{
	colorDesc.Ambient = color;
}

void Material::Ambient(float r, float g, float b, float a)
{
	Ambient(Color(r, g, b, a));
}

void Material::Diffuse(const Color & color)
{
	colorDesc.Diffuse = color;
}

void Material::Diffuse(float r, float g, float b, float a)
{
	Diffuse(Color(r, g, b, a));
}

void Material::Specular(const Color & color)
{
	colorDesc.Specular = color;
}

void Material::Specular(float r, float g, float b, float a)
{
	Specular(Color(r, g, b, a));
}

void Material::Emissive(const Color & color)
{
	colorDesc.Emissive = color;
}

void Material::Emissive(float r, float g, float b, float a)
{
	Emissive(Color(r, g, b, a));
}

void Material::DiffuseMap(string file)
{
	DiffuseMap(String::ToWString(file));
}

void Material::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);

	diffuseMap = new Texture(file);
	SetSRV(DIFFUSEMAP, diffuseMap->SRV());
}

void Material::SpecularMap(string file)
{
	SpecularMap(String::ToWString(file));
}

void Material::SpecularMap(wstring file)
{
	SafeDelete(specularMap);

	specularMap = new Texture(file);
	SetSRV(SPECULARMAP, specularMap->SRV());
}

void Material::NormalMap(string file)
{
	NormalMap(String::ToWString(file));
}

void Material::NormalMap(wstring file)
{
	SafeDelete(normalMap);

	normalMap = new Texture(file);
	SetSRV(NORMALMAP, normalMap->SRV());
}

void Material::Initialize()
{
	diffuseMap = nullptr;
	specularMap = nullptr;
	normalMap = nullptr;

	buffer = new ConstantBuffer(&colorDesc, sizeof(ColorDesc));

	SetSRV(DIFFUSEMAP, nullptr);
	SetSRV(SPECULARMAP, nullptr);
	SetSRV(NORMALMAP, nullptr);
	SetConstantBuffer(CB_MATERIAL, buffer->Buffer());
}
