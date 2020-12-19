#pragma once

class Material : public ShaderSetter
{
public:
	Material();
	Material(Shader* shader);
	virtual ~Material();

public:
	virtual void Render() override;
	virtual void SetShader(Shader* value) override;

public:
	void Name(wstring value) { name = value; }
	wstring Name() { return name; }

	const Color& Ambient() { return colorDesc.Ambient; }
	void Ambient(const Color& color);
	void Ambient(float r, float g, float b, float a = 1.0f);

	const Color& Diffuse() { return colorDesc.Diffuse; }
	void Diffuse(const Color& color);
	void Diffuse(float r, float g, float b, float a = 1.0f);

	Color& Specular() { return colorDesc.Specular; }
	void Specular(const Color& color);
	void Specular(float r, float g, float b, float a = 1.0f);

	Color& Emissive() { return colorDesc.Emissive; }
	void Emissive(const Color& color);
	void Emissive(float r, float g, float b, float a = 1.0f);

	Texture* DiffuseMap() { return diffuseMap; }
	void DiffuseMap(string file);
	void DiffuseMap(wstring file);

	Texture* SpecularMap() { return specularMap; }
	void SpecularMap(string file);
	void SpecularMap(wstring file);

	Texture* NormalMap() { return normalMap; }
	void NormalMap(string file);
	void NormalMap(wstring file);

private:
	void Initialize();

private:
	wstring name;
	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	ConstantBuffer* buffer;

private:
	struct ColorDesc
	{
		Color Ambient = Color(1, 0, 1, 1);
		Color Diffuse = Color(1, 1, 1, 1);
		Color Specular = Color(0, 0, 0, 1);
		Color Emissive = Color(0, 0, 0, 1);
	}colorDesc;
};