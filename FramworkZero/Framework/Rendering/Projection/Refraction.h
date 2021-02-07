#pragma once

class Refraction
{
public:
	Refraction(Shader * shader, Transform* transform, float width = 0, float height = 0);
	~Refraction();

	void PreRender();
	void Render();

private:
	ShaderSetter* material;
	Transform* transform;

	PreCamera* refractionCamera;
};