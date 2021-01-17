#pragma once

class CubeSky
{
public:
	CubeSky(wstring file_cubeMap);
	~CubeSky();

public:
	void Update();
	void Render();

public:
	Material* GetMaterial() { return material; }
	void Pass(UINT value) { pass = value; }

private:
	Material* material;
	Mesh* sphere;
	Transform* transform;

	ID3D11ShaderResourceView* srv = nullptr;  // 流立 积己窃
	UINT pass = 0;

};