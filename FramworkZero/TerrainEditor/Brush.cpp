#include "stdafx.h"
#include "Brush.h"

#include "BrushEditor/BrushEditor.h"
#include "BrushEditor/BrushInput.h"
#include "BrushEditor/TerrainPicker.h"


Brush::Brush()
{
	shader = new ShaderSetter();
	brush = new BrushEditor{ brushDesc };

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	this->shader->SetConstantBuffer("CB_TerrainBrush", brushBuffer->Buffer());

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	this->shader->SetConstantBuffer("CB_TerrainLine", lineBuffer->Buffer());

	input = new BrushInput();
	picker = new TerrainPicker();

	funcRaycast = [&](Vector3* value) { return ((TerrainCollider)*terrain).GetMouseRaycast(value, false); };
}

Brush::~Brush()
{
	SafeDelete(picker);
	SafeDelete(input);

	SafeDelete(lineBuffer);
	SafeDelete(brushBuffer);
	SafeDelete(brush);
	SafeDelete(shader);
}

void Brush::SetTerrain(Terrain * value)
{
	terrain = value;
	shader->SetShader(terrain->GetShader());

	brush->Width = terrain->Width();
	brush->Height = terrain->Height();
}

void Brush::Update()
{
	if (terrain == nullptr)
		return;

	UpdateBrush();
}

void Brush::Render()
{
	if (terrain == nullptr)
		return;

	brushBuffer->Render();
	lineBuffer->Render();

	shader->Render();

	RenderImGui();
}

void Brush::RenderImGui()
{
	ImGui::Begin("TerrainBrush");
	if (ImGui::CollapsingHeader("Guideline"))
	{
		static bool bVisible = true;
		ImGui::Checkbox("Line Visible", &bVisible);
		lineDesc.Visible = bVisible ? 1 : 0;

		ImGui::ColorEdit3("Line Color", lineDesc.Color);
		ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.01f);
		ImGui::InputFloat("LineSize", &lineDesc.Size, 1.0f);
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Edit"))
	{
		brush->RenderImGui();
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Picker"))
	{
		picker->RenderImGui();
	}

	ImGui::Separator();

	ImGui::SliderInt("Select Layer", &targetLayer, -1, 2);

	ImGui::End();
}

void Brush::UpdateBrush()
{
	input->Update();

	if (picker->IsPickMode())
	{
		UpdatePickMode();
		return;
	}

	if (brushDesc.Shape == 0)
		return;

	switch (input->GetState())
	{
	case BrushInput::MouseState::NONE:
	{
		Vector3 curr;
		((TerrainCollider)*terrain).GetMouseRaycast(&curr);
		brushDesc.Location = curr;
	}break;
	case BrushInput::MouseState::DOWN:
	{
		Vector3 curr;
		if (((TerrainCollider)*terrain).GetMouseRaycast(&curr) == false)
			return;

		// world
		brush->DragStartPoint = curr;
		brushDesc.Location = curr;
	}break;
	case BrushInput::MouseState::PRESS:
	{
		if (input->IsMouseMove())
		{
			Vector3 curr;
			((TerrainCollider)*terrain).GetMouseRaycast(&curr);
			brushDesc.Location = curr;

			if (brushDesc.Shape == 3)
			{
				((DragPlane)*brush).Set();
			}
		}
		ApplyBrush();

	}break;
	case BrushInput::MouseState::UP:
	{
		brushDesc.DragLength = 0.0f;
	}break;
	case BrushInput::MouseState::PICK:
	{
		UINT flatIndex
			= terrain->Width() * (UINT)brushDesc.Location.z
			+ (UINT)brushDesc.Location.x;
		brush->FlatHeight = terrain->Vertices()[flatIndex].Position.y;
	}break;
	}

}

void Brush::UpdatePickMode()
{
	if (picker->Pick1() != nullptr)
	{
		picker->Pick1()->Position(&brushDesc.Location);
		brush->FlatHeight = brushDesc.Location.y;

		if (picker->Pick2() != nullptr)
		{
			brush->DragStartPoint = brushDesc.Location;  // 이 전 위치
			picker->Pick2()->Position(&brushDesc.Location);

			brush->FlatHeight = brushDesc.Location.y;
			((DragPlane)*brush).Set();
		}

		if (Input::Keyboard()->Press(VK_SPACE))
		{
			ApplyBrush();
		}
	}

	if (input->GetState() != BrushInput::MouseState::PICK)
		return;

	Vector3 curr;
	if (((TerrainCollider)*terrain).GetMouseRaycast(&curr) == false)
		return;

	brushDesc.Location = curr;
	picker->AddPicker(curr);
}

void Brush::ApplyBrush()
{
	if (targetLayer == -1)
	{
		brush->Target = 0;
		brush->Origin = brush->Result = terrain->Vertices();
		brush->Set();
		terrain->RecalculateNormals();
		terrain->ApplyVertex();
	}
	else if (targetLayer == 0)
	{
		brush->Target = 1;
		brush->OriginAlpha = brush->ResultAlpha = terrain->Layer1Data();
		brush->Set();
		terrain->ApplyAlphasLayer1();
	}
	else if (targetLayer == 1)
	{
		brush->Target = 1;
		brush->OriginAlpha = brush->ResultAlpha = terrain->Layer2Data();
		brush->Set();
		terrain->ApplyAlphasLayer2();
	}
	else if (targetLayer == 2)
	{
		brush->Target = 1;
		brush->OriginAlpha = brush->ResultAlpha = terrain->Layer3Data();
		brush->Set();
		terrain->ApplyAlphasLayer3();
	}

}
