#include "stdafx.h"
#include "DragPlane.h"

#include "Brush.h"
#include "BrushEditor.h"


void DragPlane::Set()
{
	(*Vec2Rad).x = EndPoint.x - StartPoint.x;
	(*Vec2Rad).y = EndPoint.z - StartPoint.z;

	(*Length) = D3DXVec2Length(Vec2Rad);

	if ((*Length) == 0)
	{
		return;
	}

	(*Rad) = atan2((*Vec2Rad).y, (*Vec2Rad).x);
	D3DXVec2Normalize(Vec2Rad, Vec2Rad);

	// Make plane
	Vector3 p1, p2, p3;
	{
		p1 = p2 = StartPoint;
		p2.x -= (*Vec2Rad).y;
		p2.z += (*Vec2Rad).x;

		p3 = EndPoint;

		D3DXPlaneFromPoints(PlaneSlope, &p1, &p2, &p3);
	}
}

DragPlane::DragPlane(BrushEditor & value)
	: PlaneSlope(&value.PlaneSlope)
	, Vec2Rad(&value.Vec2Rad)
	, Rad(&value.Desc.Rad)
	, Length(&value.Desc.DragLength)
{
	StartPoint = value.DragStartPoint;
	EndPoint = value.Desc.Location;

	//if (value.Target != 0)
	//{
	//	StartPoint.y = 1;
	//	EndPoint.y = 0;
	//}
}
