#pragma once

class ShaderLoader
{
	// ±¸Á¶Ã¼
public:
	struct ShaderDesc
	{
		unsigned int RefCount = 0;
		Shader* SRV = nullptr;
	};


	// ½ºÅÂÆ½
private:
	static map<wstring, ShaderDesc> shaderMap;

protected:
	static Shader* Load(wstring file);
	static void Remove(wstring file);

	// ¸É¹ö
public:
	ShaderLoader() {};
	virtual ~ShaderLoader() {}

	// Shader* GetShader() { return shader; }

};