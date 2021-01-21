#include "stdafx.h"
#include "TerrainPicker.h"

TerrainPicker::TerrainPicker()
{
	picks[0] = nullptr;
	picks[1] = nullptr;
}

TerrainPicker::~TerrainPicker()
{
	SafeDelete(picks[0]);
	SafeDelete(picks[1]);
}

bool TerrainPicker::RenderImGui()
{
	bool bTemp = bPickMode;
	ImGui::Checkbox("Pick Mode", &bTemp);
	if (bTemp != bPickMode)
	{
		bPickMode = bTemp;
		if (bPickMode == false)
		{
			SafeDelete(picks[0]);
			SafeDelete(picks[1]);
			Debug::Gizmo->SetTransform(nullptr);
			return true;
		}
	}

	if (picks[0] != nullptr)
	{
		if (ImGui::Button("Pick 1"))
		{
			Debug::Gizmo->SetTransform(picks[0]);
		}

		if (picks[1] != nullptr)
		{
			if (ImGui::Button("Pick 2"))
			{
				Debug::Gizmo->SetTransform(picks[1]);
			}
		}
	}
	return false;
}

void TerrainPicker::AddPicker(const Vector3 & position)
{
	if (picks[0] == nullptr)
	{
		picks[0] = new Transform();
		picks[0]->Position(position);
		Debug::Gizmo->SetTransform(picks[0]);
		return;
	}

	if (picks[1] != nullptr)
	{
		SafeDelete(picks[0]);
		picks[0] = picks[1];
	}

	picks[1] = new Transform();
	picks[1]->Position(position);
	Debug::Gizmo->SetTransform(picks[1]);
}
