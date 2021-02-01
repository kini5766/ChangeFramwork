#include "Framework.h"
#include "PostEffectTest.h"

#include "Objects/PostProcessing/Saturation.h"
#include "Objects/PostProcessing/Vignette.h"
#include "Objects/PostProcessing/Wiggle.h"
#include "Objects/PostProcessing/LensDistortion.h"
#include "Objects/PostProcessing/RaialBlur.h"

PostEffectTest::PostEffectTest()
{
	shaderPostEffect = Shader::Load(URI::Shaders + L"03_Panel.fxo");

	postEffect = new PostEffect(shaderPostEffect);
	saturation = new Saturation(shaderPostEffect);
	vigentte = new Vignette(shaderPostEffect);
	wiggle = new Wiggle(shaderPostEffect);
	lens = new LensDistortion(shaderPostEffect);
	raialBlur = new RaialBlur(shaderPostEffect);

	OnSaturation();
	OnRaialBlur();
	OnWiggle();
	OnLensDistortion();
}

PostEffectTest::~PostEffectTest()
{
	SafeDelete(raialBlur);
	SafeDelete(lens);
	SafeDelete(wiggle);
	SafeDelete(vigentte);
	SafeDelete(saturation);
	SafeDelete(postEffect);
	SafeRelease(shaderPostEffect);
}


#pragma region postEffect

void PostEffectTest::Update()
{
	postEffect->Update();
}

void PostEffectTest::BeginPreRender()
{
	postEffect->BeginPreRender();
}

void PostEffectTest::EndPreRender()
{
	postEffect->EndPreRender();
}

void PostEffectTest::Render()
{
	postEffect->Render();
}

#pragma endregion


void PostEffectTest::ImGuiRender()
{
	int select = mode;
	ImGui::Begin("PostEffectTest");

	ImGui::SliderInt("Mode", &select, 0, 5);

	if (select != mode)
	{
		mode = select;
		None();
		switch (mode)
		{
		case 1: OnSaturation(); break;
		case 2: OnVignette(); break;
		case 3: OnWiggle(); break;
		case 4: OnLensDistortion(); break;
		case 5: OnRaialBlur(); break;
		default: break;
		}
	}

	switch (mode)
	{
	case 1: saturation->ImGuiRender(); break;
	case 2: vigentte->ImGuiRender(); break;
	case 3: wiggle->ImGuiRender(); break;
	case 4: lens->ImGuiRender(); break;
	case 5: raialBlur->ImGuiRender(); break;
	default: break;
	}

	ImGui::End();
}


#pragma region On

void PostEffectTest::None()
{
	postEffect->ClearEffects();
}

void PostEffectTest::OnSaturation()
{
	saturation->On(postEffect);
}

void PostEffectTest::OnVignette()
{
	vigentte->On(postEffect);
}

void PostEffectTest::OnWiggle()
{
	wiggle->On(postEffect);
}

void PostEffectTest::OnLensDistortion()
{
	lens->On(postEffect);
}

void PostEffectTest::OnRaialBlur()
{
	raialBlur->On(postEffect);
}

#pragma endregion

