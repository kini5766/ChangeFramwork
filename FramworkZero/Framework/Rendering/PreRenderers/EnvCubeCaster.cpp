#include "Framework.h"
#include "EnvCubeCaster.h"

using namespace ShaderEffectName;

EnvCubeCaster::EnvCubeCaster(const EnvCubeCasterInput & input)
	: funcPreRender(input.FuncPreRender)
{
	desc = new PreEnvCubeDesc();
	material = new ShaderSetter(input.Shader);
	buffer = new ConstantBuffer(desc, sizeof(PreEnvCubeDesc));

	material->SetConstantBuffer(CB_PRE_ENVCUBE, buffer->Buffer());
}

EnvCubeCaster::~EnvCubeCaster()
{
	SafeDelete(buffer);
	SafeDelete(material);
	SafeDelete(desc);
}

void EnvCubeCaster::PreRender(PreEnvCubeDesc * input)
{
	memcpy(desc, input, sizeof(PreEnvCubeDesc));
	buffer->Render();
	material->Render();

	funcPreRender();
}
