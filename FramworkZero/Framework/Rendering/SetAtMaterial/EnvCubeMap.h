#pragma once

class EnvCubeMap
{
public:
	EnvCubeMap(float width = 256.0f, float height = 256.0f);
	~EnvCubeMap();

public:
	void PreRender();
	void Render();

	void SetAtMaterial(ShaderSetter* target);
	Transform* GetTransform();
	void SetProjection(float width, float height, float zNear, float zFar, float fov);

	void ImGuiRender();

private:
	class EnvCamera* camera;
	ConstantBuffer* buffer;

	struct PreEnvCubeDesc* output;

private:
	struct Desc
	{
		UINT Type = 3;
		float Alpha = 0.75f;
		float RefractAmount = 0.52f;
		float Padding;

		// ÇÁ¸®³Ú
		float FresnelAmount = 1.0f;
		float FresnelBias = 1.0f;
		float FresnelScale = 1.0f;
		float Padding2;
	} desc;
public:
	Desc& GetDesc() { return desc; }
};
