#pragma once

class Reflection
{
public:
	Reflection(Shader* shader, Transform* transform, float width = 0, float height = 0);
	~Reflection();

public:
	void Update();
	void PreRender();
	void Render();

private:
	ShaderSetter* material;
	Transform* transform;
	SubCamera* reflectionCamera;
};