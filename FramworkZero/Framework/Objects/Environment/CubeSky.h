#pragma once

class CubeSky
{
public:
	CubeSky(wstring file_cubeMap);
	~CubeSky();

public:
	void Update();
	void Render();
	void PreRender_EnvCube();

public:
	Material* GetMaterial() { return material; }
	void Pass(UINT value) { pass = value; }

private:
	Material* material;
	Mesh* sphere;
	Transform* transform;

	ID3D11ShaderResourceView* srv = nullptr;  // 직접 생성함
	UINT pass = 0;

	EnvCubeCaster* preEnvCube;
};