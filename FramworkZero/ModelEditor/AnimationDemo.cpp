#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(5, 8, 0);
	Context::Get()->GetCamera()->Position(0, 1, -6);
	((Freedom*)Context::Get()->GetCamera())->Speed(10);

	shader = Shader::Load(L"20x_ExAnimation.fxo");

	Kachujin();

	sky = new CubeSky(L"Environment/SnowCube1024.dds");
	planeShader = Shader::Load(L"15_Mesh.fxo");

	plane = new Mesh(planeShader, MeshPlane(6, 6));
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}

void AnimationDemo::Destroy()
{
	SafeRelease(shader);
	SafeDelete(sky);

	SafeRelease(planeShader);
	SafeDelete(plane);

	SafeDelete(kachujin);
}

void AnimationDemo::Update()
{
	// Tween Test
	{
		static UINT clip;
		static float speed = 1.0f;
		static float takeTime = 1.0f;

		static bool bBlendMode = false;
		static float blendAlpha = 0.0f;

		ImGui::Checkbox("Use Blend Mode", &bBlendMode);

		if (bBlendMode)
		{
			ImGui::SliderFloat("Alpha", &blendAlpha, 0.0f, 2.0f);
			kachujin->SetBlendAlpha(blendAlpha);

			if (ImGui::Button("Apply"))
				kachujin->PlayBlendMode(0, 1, 2);
		}
		else
		{
			const char* clipName[] = { "Idle", "Walk", "Run", "Slash", "Uprock" };
			ImGui::Text("%s", clipName[clip]);
			ImGui::InputInt("Clip", (int*)&clip);
			clip %= 5;
			ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
			ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);
			if (ImGui::Button("Apply"))
			{
				kachujin->PlayTweenMode(clip, speed, takeTime);
			}
		}
	}

	// Light
	{
		ImGui::Text("Light");
		static Vector3 LightDirection = Vector3(-1, -1, +1);
		ImGui::SliderFloat3("LightDirection", LightDirection, -1, 1);
		shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
		planeShader->AsVector("LightDirection")->SetFloatVector(LightDirection);
	}

	static UINT Pass = 0;
	ImGui::InputInt("Pass", (int*)&Pass);
	Pass %= 2;

	sky->Update();
	plane->Update();

	if (kachujin != nullptr)
	{
		kachujin->Pass(Pass);
		kachujin->Update();
	}
}

void AnimationDemo::Render()
{
	sky->Render();
	plane->Render();

	if (kachujin != nullptr)
		kachujin->Render();
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->SetModel({
		/**/ L"Kachujin/Mesh",
		/**/ L"Kachujin/Mesh",
		/*클립*/ {
			L"Kachujin/Idle",
			L"Kachujin/Walk",
			L"Kachujin/Run",
			L"Kachujin/Slash",
			L"Kachujin/Uprock",
		} }
	);
	kachujin->GetTransform()->Position(0, 0, 0);
	kachujin->GetTransform()->Scale(0.01f, 0.01f , 0.01f);

	//// Clip
	//{
	//	KeyframeDesc& desc = tweenDesc.Curr;
	//	ModelClip* clip = clips[desc.Clip];
	//	ImGui::Spacing();

	//	// 클립 번호 선택
	//	ImGui::Text("Clip");
	//	ImGui::Separator();
	//	ImGui::InputInt("ClipIndex", &desc.Clip);
	//	desc.Clip %= clipCount;

	//	// 프레임 번호 선택
	//	ImGui::Text("Frame");
	//	ImGui::Separator();
	//	ImGui::SliderInt("CurrFrame", (int*)&desc.CurrFrame, 0, clip->FrameCount());
	//	ImGui::SliderFloat("Speed", &desc.Speed, 0.1f, 4);
	//}
}

