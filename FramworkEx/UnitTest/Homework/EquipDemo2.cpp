#include "stdafx.h"
#include "EquipDemo2.h"
#include "Viewer/Freedom.h"
#include "EquippableKachujin.h"

void EquipDemo2::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -50);
	//((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	kachujin = new EquippableKachujin();
}

void EquipDemo2::Destroy()
{
	SafeDelete(kachujin);
}

void EquipDemo2::Update()
{
	if (Keyboard::Get()->Down(VK_SPACE))
	{
		++clip;
		clip %= 4;
		kachujin->PlayClip(clip);
	}

	if (Keyboard::Get()->Down('1'))
	{
		kachujin->Armed(item);
	}
	if (Keyboard::Get()->Down('2'))
	{
		kachujin->Unarmed();
	}
	if (Keyboard::Get()->Down('3'))
	{
		item++;
		item %= 10;
		kachujin->Armed(item);
	}
	kachujin->Update();
}

void EquipDemo2::Render()
{
	kachujin->Render();

	if (ImGui::SliderInt("Item Num", (int*)&item, 0, 9))
	{
		kachujin->SetEquipItem(item);
	}
}
