#include "Framework.h"
#include "TerrainBrush.h"
#include "Terrain.h"

using namespace ShaderEffctConstantName;

TerrainBrushDemo::TerrainBrushDemo(Shader * shader)
{
	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	sBrushBuffer = shader->AsConstantBuffer(CB_Brush);
}

inline TerrainBrushDemo::~TerrainBrushDemo()
{
	SafeDelete(brushBuffer);
	SafeDeleteArray(terrainVertex);
}

void TerrainBrushDemo::SetVertex(const TerrainVertex * vertices, UINT _width, UINT _height)
{
	original = vertices;
	width = _width;
	height = _height;

	SafeDeleteArray(terrainVertex);
	terrainVertex = new TerrainVertex[width * height];

	memcpy(terrainVertex, vertices, sizeof(TerrainVertex) * width * height);
	//original = terrainVertex;
}

void TerrainBrushDemo::StartBrush(const Vector3 & position)
{
	startDragPoint = position;

	if (original == nullptr)
		return;

	brushDesc.Location = position;
	UINT flatIndex = width * ((UINT)position.z + 0) + (UINT)position.x + 0;
	flatHeight = original[flatIndex].Position.y;
}

void TerrainBrushDemo::UpdateBrush(TerrainVertex * outVertices)
{
	if (original == nullptr)
		return;

	switch (brushDesc.Shape)
	{
	case 1: RaiseRect(brushDesc.Location, brushDesc.Range); break;
	case 2: RaiseCircle(brushDesc.Location, brushDesc.Range); break;
	case 3: RaiseDrag(brushDesc.Location, brushDesc.Range); break;
	default: return;
	}

	// 적용
	memcpy(outVertices, terrainVertex, sizeof(TerrainVertex) * width * height);
}

void TerrainBrushDemo::EndBrush()
{
	SafeDelete(planeSlope);
	brushDesc.DragLength = 0.0f;
}

void TerrainBrushDemo::Render()
{
	brushBuffer->Render();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());
}

void TerrainBrushDemo::RenderImGui()
{
	UINT uintValue;

	uintValue = Shape();
	switch (uintValue)
	{
	case 0: ImGui::LabelText("##Shape2", "None"); break;
	case 1: ImGui::LabelText("##Shape2", "Rect"); break;
	case 2: ImGui::LabelText("##Shape2", "Circle"); break;
	case 3: ImGui::LabelText("##Shape2", "Drag"); break;
	default:ImGui::LabelText("##Shape2", "None"); break;
	}
	ImGui::InputInt("Shape", (int*)&uintValue);
	uintValue %= 4;
	Shape(uintValue);

	uintValue = Type();
	switch (uintValue)
	{
	case 0: ImGui::LabelText("##Type2", "Up"); break;
	case 1: ImGui::LabelText("##Type2", "Down"); break;
	case 2: ImGui::LabelText("##Type2", "Noise"); break;
	case 3: ImGui::LabelText("##Type2", "Gaussian"); break;
	case 4: ImGui::LabelText("##Type2", "Flat"); break;
	case 5: ImGui::LabelText("##Type2", "Slope"); break;
	default:ImGui::LabelText("##Type2", "None"); break;
	}
	ImGui::InputInt("Type", (int*)&uintValue);
	uintValue %= 6;
	Type(uintValue);

	uintValue = Range();
	ImGui::InputInt("Range", (int*)&uintValue);
	uintValue %= 30;
	Range(uintValue);

	float floatValue;
	floatValue = Rad() * Math::Rad2Deg();
	ImGui::DragFloat("Rad", &floatValue, 1.0f);
	Rad(floatValue * Math::Deg2Rad());

}

void TerrainBrushDemo::RaiseRect(const Vector3 & position, UINT range)
{
	/*
	D3D11_BOX rect;
	rect.left = (UINT)position.x - range;
	rect.top = (UINT)position.z + range;
	rect.right = (UINT)position.x + range;
	rect.bottom = (UINT)position.z - range;

	if (rect.left >= (UINT)(0 - range)) rect.left = 0;
	if (rect.right >= width) rect.right = width - 1;
	if (rect.top >= height) rect.top = height - 1;
	if (rect.bottom >= (UINT)(0 - range)) rect.bottom = 0;

	for (UINT z = rect.bottom; z <= rect.top; z++)
	{
		for (UINT x = rect.left; x <= rect.right; x++)
		{
			RaiseType(x, z);
		}
	}
	*/

	float rad = Rad();  // 사각형의 회전량 (라디안)
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
	/*
	D3D11_BOX rect;
	float rightB = position.x + range * (radCos + radSin);
	if (rightB > (float)(width - 1)) rect.right = width - 1;
	else rect.right = rightB;

	if (top > (float)(height - 1)) rect.top = height - 1;
	else rect.top = (UINT)top;

	float leftB = position.x - range * (radCos + radSin);
	if (leftB < 0.0f) rect.left = 0;
	else rect.left = (UINT)leftB;

	if (bottom < 0.0f) rect.bottom = 0;
	else rect.bottom = (UINT)bottom;
	*/
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

	for (UINT z = 0; z < height; z++)
	//for (UINT z = rect.bottom; z <= rect.top; z++)
	{
		for (UINT x = 0; x < width; x++)
		//for (UINT x = rect.left; x <= rect.right; x++)
		{
			if (z <= (HSlope * x + HTop) &&
				z >= (HSlope * x + HBottom) &&
				x <= (VSlope * z + VRight) &&
				x >= (VSlope * z + VLeft))
			{
				// 사각형 반경 안에 들어감
				RaiseType(x, z);
			}
		}
	}
}

void TerrainBrushDemo::RaiseCircle(const Vector3 & position, UINT range)
{
	D3D11_BOX rect;
	rect.left = (UINT)position.x - range;
	rect.top = (UINT)position.z + range;
	rect.right = (UINT)position.x + range;
	rect.bottom = (UINT)position.z - range;

	if (rect.left >= (UINT)(0 - range)) rect.left = 0;
	if (rect.right >= width) rect.right = width - 1;
	if (rect.top >= height) rect.top = height - 1;
	if (rect.bottom >= (UINT)(0 - range)) rect.bottom = 0;

	for (UINT z = rect.bottom; z <= rect.top; z++)
	{
		for (UINT x = rect.left; x <= rect.right; x++)
		{
			float dx = position.x - (float)x;
			float dz = position.z - (float)z;
			float dist = dx * dx + dz * dz;

			if (dist <= range * range)
				RaiseType(x, z);
		}
	}
}

void TerrainBrushDemo::RaiseDrag(const Vector3 & position, UINT range)
{
	//if (type != 5) return;
	if (startDragPoint == position) return;

	Vector2 v2Rad;
	v2Rad.x = position.x - startDragPoint.x;
	v2Rad.y = position.z - startDragPoint.z;

	float length = D3DXVec2Length(&v2Rad);

	brushDesc.Rad = atan2(v2Rad.y, v2Rad.x);
	brushDesc.DragLength = length;

	D3DXVec2Normalize(&v2Rad, &v2Rad);
	//v2Rad *= range;

	// Make plane
	Vector3 p1, p2, p3;
	{
		p1 = p2 = startDragPoint;
		p2.x -= v2Rad.y;
		p2.z += v2Rad.x;

		p3 = position;

		if (planeSlope == nullptr)
			planeSlope = new Plane();
		D3DXPlaneFromPoints(planeSlope, &p1, &p2, &p3);
	}

	
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			// 점과 사각형 중점과의 사이 벡터
			Vector2 destance;
			destance.x = x - p3.x;
			destance.y = z - p3.z;

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
				RaiseType(x, z);
			}
		}
	}
	

	/*
	// 표시할 Rect의 범위
	float right = p2.x;
	float top = p2.z;
	float left = p3.x + v2Rad.y;
	float bottom = p3.z - v2Rad.x;

	// 가로축의 경사
	float HSlope = v2Rad.y / v2Rad.x;
	// 가로축 시작점의 높이
	float HBottom;
	// 가로축 끝점의 높이
	float HTop;

	// 세로축의 경사
	float VSlope = -HSlope;
	// 세로축 시작점의 높이
	float VLeft;
	// 세로축 끝점의 높이
	float VRight;

	if (v2Rad.x >= 0)
	{
		HBottom = bottom - HSlope * left;
		HTop = top - HSlope * right;
		VLeft = right - VSlope * top;
		VRight = left - VSlope * bottom;
	}
	else
	{
		HBottom = top - HSlope * right;
		HTop = bottom - HSlope * left;
		VLeft = left - VSlope * bottom;
		VRight = right - VSlope * top;
	}

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			if (z <= (HSlope * x + HTop) &&
				z >= (HSlope * x + HBottom) &&
				x <= (VSlope * z + VRight) &&
				x >= (VSlope * z + VLeft)
				)
			{
				// 사각형 반경 안에 들어감
				RaiseType(x, z);
			}
		}
	}
	*/
}

void TerrainBrushDemo::RaiseType(UINT x, UINT z)
{
	UINT index = width * (z + 0) + x + 0;

	switch (type)
	{
	case 0: RaiseUp(&terrainVertex[index].Position.y); break;
	case 1: RaiseDown(&terrainVertex[index].Position.y); break;
	case 2: RaiseNoise(&terrainVertex[index].Position.y); break;
	case 3: RaiseSmoothing(&terrainVertex[index].Position.y, x, z); break;
	case 4: RaiseFlat(&terrainVertex[index].Position.y); break;
	case 5: RaiseSlope(&terrainVertex[index].Position.y, x, z); break;
	default: return;
	}
}

void TerrainBrushDemo::RaiseUp(float * inout)
{
	*(inout) += 5.0f * Time::Delta();
}

void TerrainBrushDemo::RaiseDown(float * inout)
{
	*(inout) -= 5.0f * Time::Delta();
}

void TerrainBrushDemo::RaiseNoise(float * inout)
{
	*(inout) += Math::Random(-5.0f, 5.0f) * Time::Delta();
}

void TerrainBrushDemo::RaiseSmoothing(float * inout, UINT x, UINT z)
{
	// gaussian blur
	float weights[9] =
	{
		0.01f, 0.02f, 0.01f,
		0.02f, 0.88f, 0.02f,
		0.01f, 0.02f, 0.01f
	};

	float count = 0.0f;
	float total = 0.0f;

	for (int z2 = -1; z2 <= +1; z2++)
	{
		int z3 = z2 + (int)z;
		if (z3 < 0) continue;
		if (z3 >= (int)height) continue;

		for (int x2 = -1; x2 <= +1; x2++)
		{
			int x3 = x2 + (int)x;
			if (x3 < 0) continue;
			if (x3 >= (int)width) continue;

			UINT index = width * (z3 + 0) + x3 + 0;
			int index2 = 3 * (z2 + 1) + x2 + 1;

			count += weights[index2];
			total += original[index].Position.y * weights[index2];
		}
	}

	if (count == 0.0f)
		return;
	total = total / count;

	*(inout) = total;
}

void TerrainBrushDemo::RaiseFlat(float * inout)
{
	*(inout) = flatHeight;
}

void TerrainBrushDemo::RaiseSlope(float * inout, UINT x, UINT z)
{
	if (planeSlope == nullptr)
		return;

	const Plane& plane = *planeSlope;
	
	// y = (ax + cz + d) / -b
	*(inout) = (plane.a * x + plane.c * z + plane.d) / -plane.b;
}
