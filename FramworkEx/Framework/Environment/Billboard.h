#pragma once

class Billboard
{
public:
	Billboard(Shader* shader, wstring file);
	~Billboard();

	void Pass(UINT value) { quad->Pass(value); }
	Transform* GetTransfrom() { return quad->GetTransform(); }

	void Update();
	void Render();

private:
	Shader* shader;
	MeshQuad* quad;

	Texture* texture;
	ID3DX11EffectShaderResourceVariable* sMap;
};