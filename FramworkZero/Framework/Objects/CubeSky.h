#pragma once

class CubeSky
{
public:
	CubeSky(wstring file_cubeMap, Shader* shader = nullptr);
	~CubeSky();

public:
	void Update();
	void Render();

public:
	ShaderSetter* GetMaterial() { return shader; }
	void Pass(UINT value) { pass = value; }

private:
	ShaderSetter* shader;
	MeshInstancing* sphere;
	Transform* transform;

	ID3D11ShaderResourceView* srv = nullptr;  // 직접 생성함
	UINT pass = 0;

};