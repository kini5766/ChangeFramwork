#pragma once

class ModelMaterial : public Material
{
private:
	struct ColorDesc;

public:
	ModelMaterial();
	ModelMaterial(Shader* shader);
	~ModelMaterial();

public:
	void Render() override;
	void SetShader(Shader* value) override;

public:
	void Name(wstring value) { name = value; }
	wstring Name() { return name; }

	Color Ambient() { return colorDesc.Ambient; }
	void Ambient(const Color& value);
	void Ambient(float r, float g, float b, float a = 1.0f);

	Color Diffuse() { return colorDesc.Diffuse; }
	void Diffuse(const Color& value);
	void Diffuse(float r, float g, float b, float a = 1.0f);

	Color Specular() { return colorDesc.Specular; }
	void Specular(const Color& value);
	void Specular(float r, float g, float b, float a = 1.0f);

	Texture* DiffuseMap() { return diffuseMap; }
	void DiffuseMap(wstring file);
	Texture* SpecularMap() { return specularMap; } 
	void SpecularMap(wstring file);
	Texture* NormalMap() { return normalMap; }
	void NormalMap(wstring file);

private:
	void Initialize();

private:
	wstring name;

	struct ColorDesc
	{
		Color Ambient = Color(0, 0, 0, 1);
		Color Diffuse = Color(1, 1, 1, 1);
		Color Specular = Color(0, 0, 0, 1);
	}colorDesc;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;
};