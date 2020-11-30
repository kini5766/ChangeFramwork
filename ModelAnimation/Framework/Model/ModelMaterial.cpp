#include "Framework.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial()
{
	Initialize();
}

ModelMaterial::ModelMaterial(Shader * _shader)
	: Material(_shader)
{
	Initialize();
}

void ModelMaterial::Initialize()
{
	name = L"";

	buffer = new ConstantBuffer(&colorDesc, sizeof(ColorDesc));

	SetTexture("DiffuseMap", diffuseMap);
	SetTexture("SpecularMap", specularMap);
	SetTexture("NormalMap", normalMap);
}


ModelMaterial::~ModelMaterial()
{
	SafeDelete(buffer);

	SafeDelete(diffuseMap);
	SafeDelete(specularMap);
	SafeDelete(normalMap);
}

void ModelMaterial::Render()
{
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	Material::Render();
}

void ModelMaterial::SetShader(Shader * value)
{
	Material::SetShader(value);

	sBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_Material);
}


void ModelMaterial::Ambient(const Color & value)
{
	colorDesc.Ambient = value;
}

void ModelMaterial::Ambient(float r, float g, float b, float a)
{
	Ambient(Color(r, g, b, a));
}

void ModelMaterial::Diffuse(const Color & value)
{
	colorDesc.Diffuse = value;
}

void ModelMaterial::Diffuse(float r, float g, float b, float a)
{
	Diffuse(Color(r, g, b, a));
}

void ModelMaterial::Specular(const Color & value)
{
	colorDesc.Specular = value;
}

void ModelMaterial::Specular(float r, float g, float b, float a)
{
	Specular(Color(r, g, b, a));
}

void ModelMaterial::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);
	diffuseMap = new Texture(file);
	SetTexture("DiffuseMap", diffuseMap);
}

void ModelMaterial::SpecularMap(wstring file)
{
	SafeDelete(specularMap);
	SetTexture("SpecularMap", specularMap);
}

void ModelMaterial::NormalMap(wstring file)
{
	SafeDelete(normalMap);
	diffuseMap = new Texture(file);
	SetTexture("NormalMap", normalMap);
}
