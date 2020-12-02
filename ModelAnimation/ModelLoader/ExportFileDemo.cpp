#include "stdafx.h"
#include "ExportFileDemo.h"

#include "Converter.h"

void ExportFileDemo::Initialize()
{
	//Airplane();
	//Tower();
	//Tank();
	//Kachujin();
	//Paladin();

	//Weapon();
}

void ExportFileDemo::Airplane()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"B787/Airplane.fbx");
	conv->ExportMaterial(L"B787/Airplane");
	conv->ExportMesh(L"B787/Airplane");

	SafeDelete(conv);
}

void ExportFileDemo::Tower()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tower/Tower.fbx");
	conv->ExportMaterial(L"Tower/Tower");
	conv->ExportMesh(L"Tower/Tower");

	SafeDelete(conv);
}

void ExportFileDemo::Tank()
{
	// 수동으로 텍스쳐 파일 저장, 적어주기
	// 다시 저장하면 고쳐써짐 -> false로 export

	Converter* conv = new Converter();
	conv->ReadFile(L"Tank/tank.fbx");
	conv->ExportMaterial(L"Tank/Tank", false);
	conv->ExportMesh(L"Tank/Tank");

	SafeDelete(conv);
}

void ExportFileDemo::Kachujin()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Kachujin/Mesh.fbx");
	conv->ExportMaterial(L"Kachujin/Mesh");
	conv->ExportMesh(L"Kachujin/Mesh");

	// 에니메이션
	// 씬이 바껴서 다시 생성
	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Kachujin/Idle.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Idle");

	SafeDelete(conv);
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Walk.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Walk");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Run.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Run");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Slash.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Slash");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Uprock.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Uprock");
	}
	conv = new Converter();
	conv->ReadFile(L"Kachujin/Running.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Running");

	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Kachujin/Jump.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Jump");

	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Kachujin/Hip_Hop_Dancing.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Hip_Hop_Dancing");

	SafeDelete(conv);
}

void ExportFileDemo::Paladin()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Paladin/Mesh.fbx");
	conv->ExportMaterial(L"Paladin/Mesh");
	conv->ExportMesh(L"Paladin/Mesh");

	// 에니메이션
	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Paladin/Taunt.fbx");
	conv->ExportAnimClip(0, L"Paladin/Taunt");

	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Paladin/Dancing.fbx");
	conv->ExportAnimClip(0, L"Paladin/Dancing");

	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Paladin/SwingDancing.fbx");
	conv->ExportAnimClip(0, L"Paladin/SwingDancing");

	SafeDelete(conv);
}

void ExportFileDemo::Weapon()
{
	wstring names[] =
	{
		L"Cutter.FBX",
		L"Cutter2.FBX",
		L"Dagger_epic.FBX",
		L"Dagger_small.FBX",
		L"Katana.FBX",
		L"LongArrow.obj",
		L"LongBow.obj",
		L"Rapier.FBX",
		L"Sword.FBX",
		L"Sword_epic.FBX",
		L"Sword2.FBX",
	};

	for (int i = 0; i < 11; i++)
	{
		Converter* conv = new Converter();
		conv->ReadFile(L"Weapon/" + names[i]);

		String::Replace(&names[i], L".FBX", L"");
		String::Replace(&names[i], L".obj", L"");

		conv->ExportMaterial(L"Weapon/" + names[i], false);
		conv->ExportMesh(L"Weapon/" + names[i]);

		SafeDelete(conv);
	}

}
