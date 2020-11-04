#include "Framework.h"
#include "ShaderLoader.h"

map<wstring, ShaderLoader::ShaderDesc> ShaderLoader::shaderMap;

Shader * ShaderLoader::Load(wstring file)
{
	if (shaderMap.count(file) > 0)
	{
		shaderMap[file].RefCount++;
		return shaderMap[file].SRV;
	}

	Shader* srv = new Shader(file);
	ShaderDesc desc;
	{
		desc.RefCount++;
		desc.SRV = srv;
	}

	shaderMap[file] = desc;
	return srv;
}

void ShaderLoader::Remove(wstring file)
{
	UINT count = shaderMap.count(file);
	assert(count > 0);

	shaderMap[file].RefCount--;

	if (1 > shaderMap[file].RefCount)
	{
		SafeDelete(shaderMap[file].SRV);
		shaderMap.erase(file);
	}
}
