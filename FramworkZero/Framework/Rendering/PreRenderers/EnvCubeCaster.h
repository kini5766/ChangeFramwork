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
	void Release() { isRelease = false; funcPreRender = []() {}; }

public:
	void PreRender(PreEnvCubeDesc* input);
	bool IsRelease() const { return isRelease; }

private:
	ShaderSetter* material;
	function<void()> funcPreRender;
	bool isRelease = false;

private:
	ConstantBuffer* buffer;
	PreEnvCubeDesc* desc;

};