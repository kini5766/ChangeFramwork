#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(13, 70, 0);
	Context::Get()->MainCamera()->Position(-20, 1, -3);

	shader = Shader::Load(L"01_Instance.fxo");

	box = new MeshInstancing(shader, new MeshCube());
	box->GetRenderer()->GetDefaultMaterial()->DiffuseMap(L"Box.png");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = box->AddInstance()->GetTransform();
		transform->Scale(0.25f, 0.25f, 0.25f);
		transform->Position(Vector3(x, 0.25f * 0.5f, 0.0f));
		transform->Rotation(0, Math::Random(-(float)D3DX_PI, (float)D3DX_PI), 0);
	}
	box->UpdateTransforms();
	box->UpdateColors();

	Kachujin();
}

void WorldDemo::Destroy()
{
	SafeDelete(box);
	SafeDelete(gizmo);
	SafeDeleteArray(attachBones);
	SafeDelete(kachujin);
	SafeRelease(shader);
}

void WorldDemo::Update()
{
	static int bone = 0;
	ImGui::SliderInt("Bone", &bone, 0, kachujin->BoneCount() - 1);

	box->Update();
	kachujin->Update();

	//kachujin->GetAttachBones(0, attachBones);
	gizmo->LocalWorld(attachBones[bone]);
}

void WorldDemo::Render()
{
	box->Render();
	kachujin->Render();
}

void WorldDemo::Kachujin()
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
	kachujin->UpdateColors();
	kachujin->Pass(1);

	attachBones = new Matrix[kachujin->BoneCount()];
	gizmo = new Transform();
	gizmo->SetParent(instance->GetTransform());
	Debug::Gizmo->SetTransform(gizmo);
}
