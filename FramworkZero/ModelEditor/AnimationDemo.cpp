#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(13, 70, 0);
	Context::Get()->MainCamera()->Position(-20, 1, -3);

	shader = Shader::Load(L"01_Material.fxo");

	Kachujin();
}

void AnimationDemo::Destroy()
{
	SafeDelete(gizmo);
	SafeDeleteArray(attachBones);
	SafeDelete(kachujin);
	SafeRelease(shader);
}

void AnimationDemo::Update()
{
	// Tween Test
	//{
	//	static UINT instance = 20;
	//	static UINT clip;
	//	static float speed = 1.0f;
	//	static float takeTime = 1.0f;

	//	static bool bBlendMode = false;
	//	static float blendAlpha = 0.0f;

	//	ImGui::InputInt("Instance", (int*)&instance);
	//	ImGui::Checkbox("Use Blend Mode", &bBlendMode);

	//	if (bBlendMode)
	//	{
	//		ImGui::SliderFloat("Alpha", &blendAlpha, 0.0f, 2.0f);
	//		kachujin->Animation()->SetBlendAlpha(instance, blendAlpha);

	//		if (ImGui::Button("Apply"))
	//			kachujin->Animation()->PlayBlendMode(instance, 0, 1, 2);
	//	}
	//	else
	//	{
	//		const char* clipName[] = { "Idle", "Walk", "Run", "Slash", "Uprock" };
	//		ImGui::Text("%s", clipName[clip]);
	//		ImGui::InputInt("Clip", (int*)&clip);
	//		clip %= 5;
	//		ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
	//		ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);
	//		if (ImGui::Button("Apply"))
	//		{
	//			kachujin->Animation()->PlayTweenMode(instance, clip, speed, takeTime);
	//		}
	//	}
	//}

	static int bone = 0;
	ImGui::SliderInt("Bone", &bone, 0, kachujin->GetModel()->BoneCount() - 1);
	
	if (kachujin != nullptr)
	{
		kachujin->Update();

		//kachujin->GetAttachBones(0, attachBones);
		//gizmo->LocalWorld(attachBones[bone]);
	}
}

void AnimationDemo::Render()
{
	if (kachujin != nullptr)
		kachujin->Render();
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelSkinnedInstancing(shader,
		{
			/*매쉬*/ L"Kachujin/Mesh",
			/*매터리얼*/ L"Kachujin/Mesh",
			/*클립*/ {
				L"Kachujin/Idle",
				L"Kachujin/Walk",
				L"Kachujin/Run",
				L"Kachujin/Slash",
				L"Kachujin/Uprock",
			}
		});

	ModelSkinnedInstance* instance = kachujin->AddInstance();
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* t = kachujin->AddInstance()->GetTransform();
		t->Position(x, 0, -5);
		t->Scale(0.01f, 0.01f, 0.01f);
	}

	kachujin->UpdateTransforms();
	kachujin->Pass(0);

	attachBones = new Matrix[kachujin->GetModel()->BoneCount()];
	gizmo = new Transform();
	gizmo->SetParent(instance->GetTransform());
	Debug::Gizmo->SetTransform(gizmo);
}

