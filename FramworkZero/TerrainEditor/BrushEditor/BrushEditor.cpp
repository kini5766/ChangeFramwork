#include "stdafx.h"
#include "BrushEditor.h"

#include "Brush.h"


void BrushEditor::Set()
{
	RaiseShape();
}

void BrushEditor::RenderImGui()
{
	ImGui::ColorEdit3("BrushColor", Desc.Color);
	switch (Desc.Shape)
	{
	case 0: ImGui::LabelText("##Shape2", "None"); break;
	case 1: ImGui::LabelText("##Shape2", "Rect"); break;
	case 2: ImGui::LabelText("##Shape2", "Circle"); break;
	case 3: ImGui::LabelText("##Shape2", "Drag"); break;
	default:ImGui::LabelText("##Shape2", "None"); break;
	}
	ImGui::SliderInt("Shape", (int*)&Desc.Shape, 0, 3);
	switch (Type)
	{
	case 0: ImGui::LabelText("##Type2", "Up"); break;
	case 1: ImGui::LabelText("##Type2", "Down"); break;
	case 2: ImGui::LabelText("##Type2", "Noise"); break;
	case 3: ImGui::LabelText("##Type2", "Gaussian"); break;
	case 4: ImGui::LabelText("##Type2", "Flat"); break;
	case 5: ImGui::LabelText("##Type2", "Slope"); break;
	default:ImGui::LabelText("##Type2", "None"); break;
	}
	ImGui::SliderInt("BrushType", (int*)&Type, 0, 5);
	ImGui::SliderInt("BrushRange", (int*)&Desc.Range, 1, 30);

	float floatValue;
	floatValue = Desc.Rad * Math::Rad2Deg;
	ImGui::DragFloat("Rad", &floatValue, 1.0f);
	Desc.Rad = floatValue * Math::Deg2Rad;

	ImGui::SliderFloat("Intensity", &Intensity, 0.1f, 30.0f);
	ImGui::SliderFloat("Flat Height", &FlatHeight, 0.0f, 25.5f);


	if (Desc.Shape > 0)
	{
		string str = "";
		str += to_string(Desc.Location.x);
		str += ", ";
		str += to_string(Desc.Location.y);
		str += ", ";
		str += to_string(Desc.Location.z);
		Gui::Get()->RenderText(10, 50, 1, 0, 0, str);
	}
}


void BrushEditor::RaiseShape()
{
	//UpdateDragPlane();

	switch (Desc.Shape)
	{
	case 1: RaiseRect(); break;
	case 2: RaiseCircle(); break;
	case 3: RaiseDrag(); break;
	default: return;
	}
}

void BrushEditor::RaiseType(UINT x, UINT z, float intensity)
{
	switch (Type)
	{
	case 0: RaiseUp(x, z, intensity); break;
	case 1: RaiseDown(x, z, intensity); break;
	case 2: RaiseNoise(x, z, intensity); break;
	case 3: RaiseSmoothing(x, z, intensity); break;
	case 4: RaiseFlat(x, z, intensity); break;
	case 5: RaiseSlope(x, z, intensity); break;
	default: return;
	}
}


#pragma region Shape

void BrushEditor::RaiseRect()
{
	Vector3 position = Desc.Location;
	UINT range = Desc.Range;

	float rad = Desc.Rad;  // 사각형의 회전량 (라디안)
	float deg = rad * 57.295779513f;
	int nDeg = (int)deg;
	float fracDeg = deg - nDeg;
	nDeg %= 90;

	deg = fracDeg + nDeg;
	rad = deg * 0.01745329252f;

	float radCos = cos(rad);
	float radSin = sin(rad);

	// 표시할 Rect의 범위
	float right = position.x + range * (radCos - radSin);
	float top = position.z + range * (radCos + radSin);
	float left = position.x - range * (radCos - radSin);
	float bottom = position.z - range * (radCos + radSin);


	// 가로축의 경사
	float HSlope = tan(rad);
	// 가로축 시작점의 높이
	float HBottom = bottom - HSlope * left;
	// 가로축 끝점의 높이
	float HTop = top - HSlope * right;

	// 세로축의 경사
	float VSlope = tan(-rad);
	// 세로축 시작점의 높이
	float VLeft = left - VSlope * bottom;
	// 세로축 끝점의 높이
	float VRight = right - VSlope * top;

	float range2 = (float)range + (float)range;
	range2 = sqrtf(range2 * range2);
	UINT range2U = (UINT)range2;
	UINT leftR = static_cast<UINT>(position.x) - range2U;
	UINT topR = static_cast<UINT>(position.z) + range2U;
	UINT rightR = static_cast<UINT>(position.x) + range2U;
	UINT bottomR = static_cast<UINT>(position.z) - range2U;

	// UINT는 음수 비교 불가
	if (leftR >= (0u - range2)) leftR = 0;
	if (rightR >= Width) rightR = Width - 1;
	if (bottomR >= (0u - range2)) bottomR = 0;
	if (topR >= Height) topR = Height - 1;

	for (UINT z = bottomR; z <= topR; z++)
	{
		for (UINT x = leftR; x <= rightR; x++)
		{
			if (z <= (HSlope * x + HTop) &&
				z >= (HSlope * x + HBottom) &&
				x <= (VSlope * z + VRight) &&
				x >= (VSlope * z + VLeft))
			{
				// 사각형 반경 안에 들어감
				RaiseType(x, z, Intensity * Time::Delta());
			}
		}
	}
}

void BrushEditor::RaiseCircle()
{
	Vector3 position = Desc.Location;
	UINT range = Desc.Range;

	UINT leftR = static_cast<UINT>(position.x) - range;
	UINT topR = static_cast<UINT>(position.z) + range;
	UINT rightR = static_cast<UINT>(position.x) + range;
	UINT bottomR = static_cast<UINT>(position.z) - range;

	// UINT는 음수 비교 불가
	if (leftR >= (0u - range)) leftR = 0;
	if (rightR >= Width) rightR = Width - 1;
	if (bottomR >= (0u - range)) bottomR = 0;
	if (topR >= Height) topR = Height - 1;

	for (UINT z = bottomR; z <= topR; z++)
	{
		for (UINT x = leftR; x <= rightR; x++)
		{
			float dx = position.x - (float)x;
			float dz = position.z - (float)z;
			float dist = dx * dx + dz * dz;

			dist /= range * range;
			if (dist <= 1.0f)
			{
				dist = 1.0f - dist;
				RaiseType(x, z, dist * Intensity * Time::Delta());
			}
		}
	}
}

void BrushEditor::RaiseDrag()
{
	if (Desc.DragLength == 0.0f) return;

	Vector3 position = Desc.Location;
	Vector2 v2Rad = Vec2Rad;

	UINT range = Desc.Range;
	float length = Desc.DragLength;

	for (UINT z = 0; z < Height; z++)
	{
		for (UINT x = 0; x < Width; x++)
		{
			// 점과 사각형 중점과의 사이 벡터
			Vector2 destance;
			destance.x = x - position.x;
			destance.y = z - position.z;

			// 벡터 회전 
			// cos(A-B) = cosA * cosB + sinA * sinB
			float dx = destance.x * v2Rad.x + destance.y * v2Rad.y;
			// sin(A-B) = sinA * cosB - cosA * sinB
			float dy = destance.y * v2Rad.x - destance.x * v2Rad.y;

			if ((dx >= length * -1.0f) &&
				(dx <= 0.0f) &&
				(dy >= range * -1.0f) &&
				(dy <= range))
			{
				// 사각형 반경 안에 들어감
				RaiseType(x, z, Intensity * Time::Delta());
			}
		}
	}
}

#pragma endregion


#pragma region BrushType

void BrushEditor::RaiseUp(UINT x, UINT z, float intensity)
{
	UINT index = Width * z + x;
	Result[index].Position.y += intensity;
}

void BrushEditor::RaiseDown(UINT x, UINT z, float intensity)
{
	UINT index = Width * z + x;
	Result[index].Position.y -= intensity;
}

void BrushEditor::RaiseNoise(UINT x, UINT z, float intensity)
{
	UINT index = Width * z + x;
	Result[index].Position.y += Math::Random(-5.0f, 5.0f) * intensity;
}

void BrushEditor::RaiseSmoothing(UINT x, UINT z, float intensity)
{
	UINT index = Width * z + x;

	// intensity = 0.01f이 적당
	intensity *= 0.1f;
	float intensit2 = intensity * 2.0f;
	float intensit1 = 1.0f - intensity * 12.0f;

	// gaussianFilter
	float filter[9] =
	{
		intensity, intensit2, intensity,
		intensit2, intensit1, intensit2,
		intensity, intensit2, intensity
	};

	int width = (int)Width;
	int height = (int)Height;
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
			total += Origin[index].Position.y * filter[index2];
		}
	}

	if (count == 0.0f)
		return;
	total = total / count;

	Result[index].Position.y = total;
}

void BrushEditor::RaiseFlat(UINT x, UINT z, float intensity)
{
	UINT index = Width * z + x;
	Result[index].Position.y = FlatHeight;
}

void BrushEditor::RaiseSlope(UINT x, UINT z, float intensity)
{
	if (Desc.DragLength == 0.0f)
		return;

	Plane& p = PlaneSlope;

	UINT index = Width * z + x;

	// y = (ax + cz + d) / -b
	Result[index].Position.y = (p.a * x + p.c * z + p.d) / -p.b;
}

#pragma endregion

