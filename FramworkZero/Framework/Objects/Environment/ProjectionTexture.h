#pragma once

class ProjectionTexture
{
public:
	ProjectionTexture(Shader* s, wstring mapFile, float width, float height);
	~ProjectionTexture();

public:
	void Update();
	void Render();

public:
	Camera* GetCamera() { return (Camera*)camera; }

private:
	ShaderSetter* shader;
	Fixity* camera;
	Texture* map;
	ConstantBuffer* buffer;

private:
	struct Desc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		Matrix View;
		Matrix Projection;
	} desc;
};
