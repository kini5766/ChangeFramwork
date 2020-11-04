#pragma once

class ShaderLoader
{
	// ����ü
public:
	struct ShaderDesc
	{
		unsigned int RefCount = 0;
		Shader* SRV = nullptr;
	};


	// ����ƽ
private:
	static map<wstring, ShaderDesc> shaderMap;

protected:
	static Shader* Load(wstring file);
	static void Remove(wstring file);

	// �ɹ�
public:
	ShaderLoader() {};
	virtual ~ShaderLoader() {}

	// Shader* GetShader() { return shader; }

};