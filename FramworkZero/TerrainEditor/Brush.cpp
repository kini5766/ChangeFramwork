#include "stdafx.h"
#include "Brush.h"

Brush::Brush(Terrain * terrain)
	: shader(new ShaderSetter())
	, terrain(terrain)
{
	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	this->shader->SetConstantBuffer("CB_TerrainBrush", brushBuffer->Buffer());

	shader->SetShader(terrain->GetShader());
}

Brush::~Brush()
{
	SafeDelete(brushBuffer);
}

void Brush::Update()
{
	ImGui::ColorEdit3("BrushColor", brushDesc.Color);
	ImGui::SliderInt("BrushType", (int*)&brushDesc.Type, 0, 3);
	ImGui::InputInt("BrushRange", (int*)&brushDesc.Range, 1, 10);
	brushDesc.Range = Math::Clamp<UINT>(brushDesc.Range, 1, 10);

	if (brushDesc.Type > 0)
	{
		brushDesc.Location = terrain->GetRaycastPosition();

		if (brushDesc.Type == 1)
		{
			if (Input::Mouse()->Press(0))
				RaiseHeight(5);
		}
	}
}

void Brush::Render()
{
	if (brushDesc.Type > 0)
	{
		string str = "";
		str += to_string(brushDesc.Location.x);
		str += ", ";
		str += to_string(brushDesc.Location.y);
		str += ", ";
		str += to_string(brushDesc.Location.z);
		Gui::Get()->RenderText(10, 50, 1, 0, 0, str);
	}

	brushBuffer->Render();
	shader->Render();
}

void Brush::RaiseHeight(float intensity)
{
	UINT left = (UINT)brushDesc.Location.x - brushDesc.Range;
	UINT right = (UINT)brushDesc.Location.x + brushDesc.Range;
	UINT bottom = (UINT)brushDesc.Location.z - brushDesc.Range;
	UINT top = (UINT)brushDesc.Location.z + brushDesc.Range;

	UINT width = terrain->Width();
	UINT height = terrain->Height();

	if (left < 0) left = 0;
	if (right >= width) right = width;
	if (bottom < 0) bottom = 0;
	if (top >= height) top = height;

	for (UINT z = bottom; z <= top; z++)
	{
		for (UINT x = left; x <= right; x++)
		{
			UINT index;
			index = width * (z + 0) + x + 0;  // ¿Þ¾Æ
			//index[1] = width * (z + 1) + x + 0;  // ¿ÞÀ§
			//index[2] = width * (z + 0) + x + 1;  // ¿À¾Æ
			//index[3] = width * (z + 1) + x + 1;  // ¿ÀÀ§

			terrain->Vertices()[index].Position.y += intensity * Time::Delta();
		}
	}

	terrain->RecalculateNormals();
	terrain->ApplyVertex();
}
