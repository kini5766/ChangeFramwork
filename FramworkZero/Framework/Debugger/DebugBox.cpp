#include "Framework.h"
#include "DebugBox.h"

DebugBox::DebugBox()
{
	vertex[0] = Vector3(-0.5f, -0.5f, -0.5f);
	vertex[1] = Vector3(-0.5f, +0.5f, -0.5f);
	vertex[2] = Vector3(+0.5f, -0.5f, -0.5f);
	vertex[3] = Vector3(+0.5f, +0.5f, -0.5f);

	vertex[4] = Vector3(-0.5f, -0.5f, +0.5f);
	vertex[5] = Vector3(-0.5f, +0.5f, +0.5f);
	vertex[6] = Vector3(+0.5f, -0.5f, +0.5f);
	vertex[7] = Vector3(+0.5f, +0.5f, +0.5f);
}

DebugBox::~DebugBox()
{
}

void DebugBox::RenderBox(Transform* box, const Color& color)
{
	Matrix world;
	box->GlobalWorld(&world);
	boxes.push_back({ world, color });
}

void DebugBox::Render()
{
	Vector3 line[8];

	for (BoxDesc& box : boxes)
	{
		const Color& color = box.Color;

		for (UINT i = 0; i < 8; i++)
			D3DXVec3TransformCoord(&line[i], &vertex[i], &box.Transform);

		Debug::Line->RenderLine(line[0], line[1], color);
		Debug::Line->RenderLine(line[1], line[3], color);
		Debug::Line->RenderLine(line[3], line[2], color);
		Debug::Line->RenderLine(line[2], line[0], color);

		Debug::Line->RenderLine(line[4], line[5], color);
		Debug::Line->RenderLine(line[5], line[7], color);
		Debug::Line->RenderLine(line[7], line[6], color);
		Debug::Line->RenderLine(line[6], line[4], color);

		Debug::Line->RenderLine(line[0], line[4], color);
		Debug::Line->RenderLine(line[1], line[5], color);
		Debug::Line->RenderLine(line[2], line[6], color);
		Debug::Line->RenderLine(line[3], line[7], color);
	}
	boxes.clear();
}
