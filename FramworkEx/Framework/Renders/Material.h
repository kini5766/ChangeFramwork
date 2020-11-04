#pragma once

class Material
{
private:
	struct ColorDesc;

public:
	Material();
	Material(Shader* shader);
	~Material();

public:
	void SetShader(Shader* value);
	Shader* GetShader() { return shader; }

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
	void DiffuseMap(string file);
	void DiffuseMap(wstring file);

	Texture* SpecularMap() { return diffuseMap; }
	void SpecularMap(string file);
	void SpecularMap(wstring file);

	Texture* NormalMap() { return diffuseMap; }
	void NormalMap(string file);
	void NormalMap(wstring file);

	void Render();

private:
	void Initialize();

private:
	Shader* shader;
	wstring name;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	struct ColorDesc
	{
		Color Ambient = Color(0, 0, 0, 1);
		Color Diffuse = Color(1, 1, 1, 1);
		Color Specular = Color(0, 0, 0, 1);
	}colorDesc;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
	ID3DX11EffectShaderResourceVariable* sSpecularMap;
	ID3DX11EffectShaderResourceVariable* sNormalMap;
};