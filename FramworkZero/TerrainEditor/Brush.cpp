#include "stdafx.h"
#include "Brush.h"

Brush::Brush()
{
	shader = new ShaderSetter();

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	this->shader->SetConstantBuffer("CB_TerrainBrush", brushBuffer->Buffer());

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	this->shader->SetConstantBuffer("CB_TerrainLine", lineBuffer->Buffer());

}

Brush::~Brush()
{
	SafeDelete(shader);
	SafeDelete(lineBuffer);
	SafeDelete(brushBuffer);
}

void Brush::Update()
{
	if (terrain == nullptr)
		return;

	Vector3 mouse = Input::Mouse()->GetPosition();

	if (
		ImGui::GetIO().WantCaptureMouse
		&& ImGui::IsAnyItemHovered()
		//&& ImGui::IsAnyWindowHovered()
		)
	{
		brushDesc.Location = terrain->GetRaycastPosition();
		return;
	}

	if (brushDesc.Shape == 0)
		return;


	if (Input::Mouse()->Down(0))
	{
		Vector3 curr = terrain->GetRaycastPosition();
		if (curr == Vector3(-1, -1, -1))
			return;

		// world
		prevMousePos = mouse;
		startDragPoint = curr;
		brushDesc.Location = curr;

		UINT flatIndex
			= terrain->Width() * (UINT)brushDesc.Location.z 
			+ (UINT)brushDesc.Location.x;
		flatHeight = terrain->Vertices()[flatIndex].Position.y;
	}
	else if (Input::Mouse()->Press(0))
	{
		if (mouse != prevMousePos)
		{
			prevMousePos = mouse;
			brushDesc.Location = terrain->GetRaycastPosition();
		}

		origin = result = terrain->Vertices();

		UpdateBrush();

		terrain->RecalculateNormals();
		terrain->ApplyVertex();
	}
	else if (Input::Mouse()->Up(0))
	{
		SafeDelete(planeSlope);
		brushDesc.DragLength = 0.0f;
	}
	else
	{
		brushDesc.Location = terrain->GetRaycastPosition();
	}

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
	if (ImGui::CollapsingHeader("Terrain Guideline"))
	{
		static bool bVisible = true;
		ImGui::Checkbox("Line Visible", &bVisible);
		lineDesc.Visible = bVisible ? 1 : 0;

		ImGui::ColorEdit3("Line Color", lineDesc.Color);
		ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.01f);
		ImGui::InputFloat("LineSize", &lineDesc.Size, 1.0f);

		ImGui::Separator();
	}

	ImGui::ColorEdit3("BrushColor", brushDesc.Color);
	switch (brushDesc.Shape)
	{
	case 0: ImGui::LabelText("##Shape2", "None"); break;
	case 1: ImGui::LabelText("##Shape2", "Rect"); break;
	case 2: ImGui::LabelText("##Shape2", "Circle"); break;
	case 3: ImGui::LabelText("##Shape2", "Drag"); break;
	default:ImGui::LabelText("##Shape2", "None"); break;
	}
	ImGui::SliderInt("Shape", (int*)&brushDesc.Shape, 0, 3);
	switch (type)
	{
	case 0: ImGui::LabelText("##Type2", "Up"); break;
	case 1: ImGui::LabelText("##Type2", "Down"); break;
	case 2: ImGui::LabelText("##Type2", "Noise"); break;
	case 3: ImGui::LabelText("##Type2", "Gaussian"); break;
	case 4: ImGui::LabelText("##Type2", "Flat"); break;
	case 5: ImGui::LabelText("##Type2", "Slope"); break;
	default:ImGui::LabelText("##Type2", "None"); break;
	}
	ImGui::SliderInt("BrushType", (int*)&type, 0, 5);
	ImGui::SliderInt("BrushRange", (int*)&brushDesc.Range, 1, 30);

	float floatValue;
	floatValue = brushDesc.Rad * Math::Rad2Deg;
	ImGui::DragFloat("Rad", &floatValue, 1.0f);
	brushDesc.Rad = floatValue * Math::Deg2Rad;

	ImGui::End();


	if (brushDesc.Shape > 0)
	{
		string str = "";
		str += to_string(brushDesc.Location.x);
		str += ", ";
		str += to_string(brushDesc.Location.y);
		str += ", ";
		str += to_string(brushDesc.Location.z);
		Gui::Get()->RenderText(10, 50, 1, 0, 0, str);
	}
}

void Brush::SetTerrain(Terrain * value)
{
	terrain = value;
	shader->SetShader(terrain->GetShader());
}

void Brush::UpdateBrush()
{
	if (brushDesc.Shape != 3)
		return;

	Vector3 position = brushDesc.Location;

	// RaiseDrag
	if (startDragPoint == position) return;


	Vector2 v2Rad;
	v2Rad.x = position.x - startDragPoint.x;
	v2Rad.y = position.z - startDragPoint.z;
	
	Debug::Log->Show(to_string(v2Rad.x));
	Debug::Log->Show(to_string(v2Rad.y));

	float length = D3DXVec2Length(&v2Rad);

	brushDesc.Rad = atan2(v2Rad.y, v2Rad.x);
	brushDesc.DragLength = length;
}


#pragma region BrushType

void Brush::RaiseUp(UINT x, UINT z, float intensity)
{
	UINT index = terrain->Width() * z + x;
	result[index].Position.y += intensity;
}

void Brush::RaiseDown(UINT x, UINT z, float intensity)
{
	UINT index = terrain->Width() * z + x;
	result[index].Position.y -= intensity;
}

void Brush::RaiseNoise(UINT x, UINT z, float intensity)
{
	UINT index = terrain->Width() * z + x;
	result[index].Position.y += Math::Random(-5.0f, 5.0f) * intensity;
}

void Brush::RaiseSmoothing(UINT x, UINT z, float intensity)
{
	UINT index = terrain->Width() * z + x;

	// intensity = 0.01fÀÌ Àû´ç
	float intensit2 = intensity * 2.0f;
	float intensit1 = 1.0f - intensity * 12.0f;

	// gaussianFilter
	float filter[9] =
	{
		intensity, intensit2, intensity,
		intensit2, intensit1, intensit2,
		intensity, intensit2, intensity
	};

	int width = (int)terrain->Width();
	int height = (int)terrain->Height();
	float count = 0.0f;
	float total = 0.0f;

	for (int z2 = -1; z2 <= +1; z2++)
	{
		int z3 = z2 + (int)z;
		if (z3 < 0) continue;
		if (z3 >= height) continue;

		for (int x2 = -1; x2 <= +1; x2++)
		{
			int x3 = x2 + (int)x;
			if (x3 < 0) continue;
			if (x3 >= width) continue;

			UINT index = width * (z3 + 0) + x3 + 0;
			int index2 = 3 * (z2 + 1) + x2 + 1;

			count += filter[index2];
			total += origin[index].Position.y * filter[index2];
		}
	}

	if (count == 0.0f)
		return;
	total = total / count;

	result[index].Position.y = total;
}

void Brush::RaiseFlat(UINT x, UINT z, float intensity)
{
	UINT index = terrain->Width() * z + x;
	result[index].Position.y = flatHeight;
}

void Brush::RaiseSlope(UINT x, UINT z, float intensity)
{
	if (planeSlope == nullptr)
		return;

	const Plane& plane = *planeSlope;

	UINT index = terrain->Width() * z + x;

	// y = (ax + cz + d) / -b
	result[index].Position.y = (plane.a * x + plane.c * z + plane.d) / -plane.b;
}

#pragma endregion
