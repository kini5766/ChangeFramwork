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
		switch (mode)
		{
		case 0: None(); break;
		case 1: None(); OnSaturation(); break;
		case 2: None(); OnVignette(); break;
		case 3: None(); OnWiggle(); break;
		case 4: None(); OnLensDistortion(); break;
		case 5: None(); OnRaialBlur(); break;
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
	postEffect->EffectEvents().clear();
}

void PostEffectTest::OnSaturation()
{
	postEffect->EffectEvents().push_back(
		bind(&Saturation::PreRender, saturation, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

void PostEffectTest::OnVignette()
{
	postEffect->EffectEvents().push_back(
		bind(&Vignette::PreRender, vigentte, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

void PostEffectTest::OnWiggle()
{
	postEffect->EffectEvents().push_back(
		bind(&Wiggle::PreRender, wiggle, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

void PostEffectTest::OnLensDistortion()
{
	postEffect->EffectEvents().push_back(
		bind(&LensDistortion::PreRender, lens, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

void PostEffectTest::OnRaialBlur()
{
	postEffect->EffectEvents().push_back(
		bind(&RaialBlur::PreRender, raialBlur, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

#pragma endregion

