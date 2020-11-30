#pragma once

namespace MaterialGroupValue { class IVariableEffects; }

class MaterialGroup
{
public:
	MaterialGroup();
	~MaterialGroup();

public:
	void Render();

	UINT AddMaterial(Material* value);
	Material* GetMaterial(UINT index) { return materials[index]; }
	UINT MaterialsSize() { return materials.size(); }

	void SetConstantBuffer(string name, ConstantBuffer* buffer);
	void SetSRV(string name, ID3D11ShaderResourceView* buffer);

private:
	void AddShader(Shader* value);

private:
	vector<Material*> materials;
	map<string, MaterialGroupValue::IVariableEffects*> effectsValues;

private:
	struct ShaderRef
	{
		Shader* Shader;
		int RefCount;
	};
	vector<ShaderRef*> shaders;
};