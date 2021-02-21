#pragma once

struct EnvCubeCasterInput
{
	Shader* Shader;
	function<void()> FuncPreRender;
};


struct PreEnvCubeDesc
{
	Matrix Views[6];
	Matrix Projection;
};

class EnvCubeCaster
{
public:
	EnvCubeCaster(const EnvCubeCasterInput& input);
	~EnvCubeCaster();

public:
	void Release() { bRelease = false; bActive = false; funcPreRender = []() {}; }
	void SetActive(bool value) { bActive = value; }

public:
	void PreRender(PreEnvCubeDesc* input);
	bool IsRelease() const { return bRelease; }

private:
	ShaderSetter* material;
	function<void()> funcPreRender;
	bool bRelease = false;
	bool bActive = true;

private:
	ConstantBuffer* buffer;
	PreEnvCubeDesc* desc;

};