#include "stdafx.h"
#include "EquippableKachujin.h"

EquippableKachujin::EquippableKachujin()
{
	shader = new Shader(L"18_Animation.fx");
	Kachujin();

	Sword();  // 1
	Sword_epic();  // 2
	Sword2();  // 3
	Rapier();  // 4
	LongBow();  // 5
	Katana();  // 6
	Dagger_small();  // 7
	Dagger_epic();  // 8
	Cutter2();  // 9
	Cutter();  // 10

	kachujin->Pass(2);
}

EquippableKachujin::~EquippableKachujin()
{
	SafeDelete(kachujin);
	SafeDelete(shader);
}

void EquippableKachujin::Update()
{
	/*
	float animTime = kachujin->GetClipRunTime();
	switch (state)
	{
	case EquippableKachujin::State::ArmedBefore:
		if (animTime > 25.0f)
		{
			equipedItemNum = chageItem;
			chageItem = -1;

			kachujin->ChangeModel(equipedItemNum);
			bGetWeapon = true;
			state = State::ArmedAfter;
		}
		break;
	case EquippableKachujin::State::ArmedAfter:
		if (animTime > kachujin->GetClipLength(4))
		{
			kachujin->PlayClip(clipNum);
			state = State::None;
		}
		break;
	case EquippableKachujin::State::UnarmedBefore:
		if (animTime > 25.0f)
		{
			kachujin->OnlyOrigin();
			bGetWeapon = false;
			state = State::UnarmedAfter;
		}
		break;
	case EquippableKachujin::State::UnarmedAfter:
		if (animTime > kachujin->GetClipLength(5))
		{
			kachujin->PlayClip(clipNum);
			state = State::None;
		}
		break;
	}
	*/
	kachujin->Update();
}

void EquippableKachujin::Render()
{
	kachujin->Render();

	Vector3 v3;
	kachujin->GetTransform()->Position(&v3);
	ImGui::DragFloat3("position", v3);
	kachujin->GetTransform()->Position(v3);
}

void EquippableKachujin::Unarmed()
{
	if (bGetWeapon == false)
		return;

	if (state == State::UnarmedBefore)
		return;

	kachujin->PlayClip(5);
	state = State::UnarmedBefore;
}

void EquippableKachujin::Armed(int num)
{
	if (chageItem != -1)
		return;

	if (equipedItemNum == num && bGetWeapon)
		return;

	chageItem = num;
	kachujin->PlayClip(4);
	state = State::ArmedBefore;
}

void EquippableKachujin::SetEquipItem(int num)
{

	equipedItemNum = num;

	kachujin->OnlyOrigin();
	kachujin->SetActiveItem(equipedItemNum, true);

	//if (bGetWeapon)
	//	kachujin->SetActiveItem(equipedItemNum, true);
	
	
}

void EquippableKachujin::PlayClip(UINT clip, float speed, float takeTime)
{
	clipNum = clip;
	if (state == State::None)
		kachujin->PlayClip(clip, speed, takeTime);
}

void EquippableKachujin::Kachujin()
{
	kachujin = new ModelAttacher(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");

	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");
	kachujin->ReadClip(L"Kachujin/Equip");
	kachujin->ReadClip(L"Kachujin/Disarm");
	kachujin->ApplyOriginModel();

	kachujin->GetTransform()->Position(3, 0, -20);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

void EquippableKachujin::Sword()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");

	Transform offset = Transform();
	//offset.Position(-13, -3, -5);
	offset.Position(0, 100, 0);
	//offset.RotationDegree(0, 0, 90);
	//offset.Scale(0.8f, 0.8f, 5.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}


void EquippableKachujin::Sword_epic()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword_epic");
	weapon->ReadMesh(L"Weapon/Sword_epic");

	Transform offset = Transform();
	offset.Position(-13.0f, -3.0f, -5.0f);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.8f, 0.8f, 0.8f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Sword2()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword2");
	weapon->ReadMesh(L"Weapon/Sword2");

	Transform offset = Transform();
	offset.Position(-13.0f, -3.0f, -5.0f);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.8f, 0.8f, 0.8f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Rapier()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Rapier");
	weapon->ReadMesh(L"Weapon/Rapier");

	Transform offset = Transform();
	offset.Position(-13.0f, -3.0f, -5.0f);
	offset.RotationDegree(0, 0, -90);
	offset.Scale(0.5f, 0.5f, 0.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::LongBow()
{
	
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/LongBow");
	weapon->ReadMesh(L"Weapon/LongBow");

	Transform offset = Transform();
	offset.Position(10.0f, -5.0f, 0.0f);
	offset.RotationDegree(-90, 90, 0);
	offset.Scale(5.8f, 5.8f, 5.8f);
	kachujin->AttachItem(weapon, 11, &offset);

	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/LongArrow");
	weapon->ReadMesh(L"Weapon/LongArrow");

	offset.Position(-10, 3, -15);
	offset.RotationDegree(90, 0, 0);
	offset.Scale(5.5f, 5.5f, 5.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Katana()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Katana");
	weapon->ReadMesh(L"Weapon/Katana");

	Transform offset = Transform();
	offset.Position(-10, -3, -5);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.8f, 0.8f, 0.8f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Dagger_small()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Dagger_small");
	weapon->ReadMesh(L"Weapon/Dagger_small");

	Transform offset = Transform();
	offset.Position(-12, -3, -5);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.5f, 0.5f, 0.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Dagger_epic()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Dagger_epic");
	weapon->ReadMesh(L"Weapon/Dagger_epic");

	Transform offset = Transform();
	offset.Position(-10, -3, -5);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.5f, 0.5f, 0.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Cutter2()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Cutter2");
	weapon->ReadMesh(L"Weapon/Cutter2");

	Transform offset = Transform();
	offset.Position(-13, -3, -5);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.5f, 0.5f, 0.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}

void EquippableKachujin::Cutter()
{
	Model* weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Cutter");
	weapon->ReadMesh(L"Weapon/Cutter");

	Transform offset = Transform();
	offset.Position(-10, -3, -5);
	offset.RotationDegree(0, 0, 90);
	offset.Scale(0.5f, 0.5f, 0.5f);
	kachujin->AttachItem(weapon, 35, &offset);
}