#include "Framework.h"
#include "ShadowCaster.h"

using namespace ShaderEffectName;

ShadowCaster::ShadowCaster(const ShadowCasterInput & input)
	: funcPreRender(input.FuncPreRender)
{
	shadowDesc = new ShadowDesc();
	material = new ShaderSetter(input.Shader);
	buffer = new ConstantBuffer(shadowDesc, sizeof(ShadowDesc));

	material->SetConstantBuffer(CB_SHADOW, buffer->Buffer());
}

ShadowCaster::~ShadowCaster()
{
	SafeDelete(buffer);
	SafeDelete(material);
	SafeDelete(shadowDesc);
}

void ShadowCaster::PreRender(PreShadowValue * value)
{
	memcpy(shadowDesc, value->Shadow, sizeof(ShadowDesc));
	buffer->Render();

	material->SetSampler(SHADOW_SAMPLER, 0, value->ShadowSampler);
	material->SetSRV(SHADOW_MAP, value->DepthStencil->SRV());
	material->Render();

	funcPreRender();
}
