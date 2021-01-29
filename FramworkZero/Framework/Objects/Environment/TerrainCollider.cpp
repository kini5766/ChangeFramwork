#include "Framework.h"
#include "TerrainCollider.h"


#pragma region TerrainLodCollider

bool TerrainLodCollider::GetRaycastPosition_Old(Vector3 * outPoint)
{
	Matrix W;
	Transform->GlobalWorld(&W);
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();
	Vector3 mouse = Input::Mouse()->GetPosition();

	//mouse = Vector3(Screen::Width() * 0.5f, Screen::Height() * 0.5f, 0);

	mouse.z = 0.0f;
	Vector3 n = vp->Unproject(mouse, W, V, P);

	mouse.z = 1.0f;
	Vector3 f = vp->Unproject(mouse, W, V, P);

	Vector3 start = n;
	Vector3 direction = f - n;

	float yScale = HeightScale * 255.0f;

	UINT maxWidth = MapWidth - 1;
	UINT maxHeight = MapHeight - 1;

	float xScale = (Width / ((float)maxWidth));
	float zScale = (Height / ((float)maxHeight));
	float xStart = Width * 0.5f;
	float zStart = Height * 0.5f;

	struct UINTOffset {
		UINT x, z;
	}offsets[4]{
		{0, 0}, {1, 0},
		{0, 1}, {1, 1}
	};
	for (UINT z = 0; z < maxHeight; z++)
	{
		for (UINT x = 0; x < maxWidth; x++)
		{
			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
			{
				UINT currZ = (z + offsets[i].z);
				UINT currX = (x + offsets[i].x);
				UINT index = MapWidth * currZ + currX;

				float h = Heights[index] * yScale;
				float hz = (float)currZ * -zScale + zStart;
				float hx = (float)currX * +xScale - xStart;

				p[i] = Vector3(hx, h, hz);
			}

			float u, v, distance;
			if (D3DXIntersectTri(p + 0, p + 1, p + 2, &start, &direction, /* out */ &u, /* out */ &v, /* out */ &distance))
			{
				(*outPoint) = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				return true;
			}
			else if (D3DXIntersectTri(p + 3, p + 1, p + 2, &start, &direction, /* out */ &u, /* out */ &v, /* out */ &distance))
			{
				(*outPoint) = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				return true;
			}
		}
	}

	return false;
}

TerrainLodCollider::TerrainLodCollider()
{
}

TerrainLodCollider::TerrainLodCollider(TerrainLOD & value)
	: Heights(value.HeightMapData())
	, HeightScale(value.HeightScale())
	, Transform(value.GetTransform())
	, MapWidth(value.MapWidth())
	, MapHeight(value.MapHeight())
	, Width(value.GetWidth())
	, Height(value.GetHeight())
{
}

#pragma endregion


#pragma region TerrainCollider

typedef Terrain::VertexTerrain Vertex;

float TerrainCollider::GetHeight(const Vector3 & position)
{
	Vector3 p[4];
	{
		UINT x = (UINT)position.x;
		UINT z = (UINT)position.z;

		if (x < 0 || x > Width - 2) return FLT_MIN;
		if (z < 0 || z > Height - 2) return FLT_MIN;

		UINT index[4];
		index[0] = Width * (z + 0) + x + 0;  // 왼아
		index[1] = Width * (z + 1) + x + 0;  // 왼위
		index[2] = Width * (z + 0) + x + 1;  // 오아
		index[3] = Width * (z + 1) + x + 1;  // 오위

		for (UINT i = 0; i < 4; i++)
			p[i] = Vertices[index[i]].Position;
	}

	Vector3 result;
	{
		float ddx = (position.x - p[0].x) / 1.0f;
		float ddz = (position.z - p[0].z) / 1.0f;

		if (ddx + ddz <= 1.0f)
		{
			result = p[0] + (p[2] - p[0]) * ddx + (p[1] - p[0]) * ddz;
		}
		else
		{
			ddx = 1.0f - ddx;
			ddz = 1.0f - ddz;
			result = p[3] + (p[1] - p[3]) * ddx + (p[2] - p[3]) * ddz;
		}
	}

	return result.y;
}

Vector3 TerrainCollider::GetNormal(float _x, float _z)
{
	Vector3 result(0, 1, 0);

	Vertex p[4];
	{
		UINT x = (UINT)_x;
		UINT z = (UINT)_z;

		if (x < 0 || x > Width - 2) return result;
		if (z < 0 || z > Height - 2) return result;

		UINT index[4];
		index[0] = Width * (z + 0) + x + 0;  // 왼아
		index[1] = Width * (z + 1) + x + 0;  // 왼위
		index[2] = Width * (z + 0) + x + 1;  // 오아
		index[3] = Width * (z + 1) + x + 1;  // 오위

		for (UINT i = 0; i < 4; i++)
			p[i] = Vertices[index[i]];
	}

	{
		float ddx = (_x - p[0].Position.x) / 1.0f;
		float ddz = (_z - p[0].Position.z) / 1.0f;

		if (ddx + ddz <= 1.0f)
		{
			result = p[0].Normal + (p[2].Normal - p[0].Normal) * ddx + (p[1].Normal - p[0].Normal) * ddz;
			D3DXVec3Normalize(&result, &result);
		}
		else
		{
			ddx = 1.0f - ddx;
			ddz = 1.0f - ddz;
			result = p[3].Normal + (p[1].Normal - p[3].Normal) * ddx + (p[2].Normal - p[3].Normal) * ddz;
			D3DXVec3Normalize(&result, &result);
		}
	}

	return result;
}

float TerrainCollider::GetVerticalRaycast(const Vector3 & position)
{
	Vector3 p[4];
	{
		UINT x = (UINT)position.x;
		UINT z = (UINT)position.z;

		if (x < 0 || x > Width - 2) return FLT_MIN;
		if (z < 0 || z > Height - 2) return FLT_MIN;

		UINT index[4];
		index[0] = Width * (z + 0) + x + 0;  // 왼아
		index[1] = Width * (z + 1) + x + 0;  // 왼위
		index[2] = Width * (z + 0) + x + 1;  // 오아
		index[3] = Width * (z + 1) + x + 1;  // 오위

		for (UINT i = 0; i < 4; i++)
			p[i] = Vertices[index[i]].Position;
	}

	Vector3 start(position.x, 100.0f, position.z);
	Vector3 direction(0, -1, 0);
	Vector3 result(-1, FLT_MIN, -1);

	float u, v, distance;
	if (D3DXIntersectTri(p + 0, p + 1, p + 2, &start, &direction, /* out */ &u, /* out */ &v, /* out */ &distance))
	{
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
	}
	else if (D3DXIntersectTri(p + 3, p + 1, p + 2, &start, &direction, /* out */ &u, /* out */ &v, /* out */ &distance))
	{
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
	}

	return result.y;
}

bool TerrainCollider::GetMouseRaycast(Vector3 * outPoint)
{
	Matrix W;
	Transform->GlobalWorld(&W);
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();
	Vector3 mouse = Input::Mouse()->GetPosition();

	// 마우스 시작 지점
	mouse.z = 0.0f;
	Vector3 n = vp->Unproject(mouse, W, V, P);

	// 마우스 끝 지점
	mouse.z = 1.0f;
	Vector3 f = vp->Unproject(mouse, W, V, P);

	// 방향
	Vector3 dist = f - n;

	int sx, sz, ex, ez, z1, z2;
	int xdir, zdir;
	float slope = dist.z / dist.x;

	sx = (int)n.x;
	sz = (int)n.z;
	ex = (int)f.x;
	ez = (int)f.z;
	z1 = sz;

	int xMax = (int)Width - 2;
	int zMax = (int)Height - 2;

	if (dist.x == 0) return false;
	else if (dist.x > 0) {
		xdir = 1;
		if (n.x > xMax) return false;
		if (n.x < 0) {
			sz = (int)(slope * (0 - n.x) + n.z);
			sx = 0;
		}
	}
	else {
		xdir = -1;
		if (n.x < 0) return false;
		if (n.x > xMax) {
			sz = (int)(slope * (xMax - n.x) + n.z);
			sx = xMax;
		}
	}

	if (dist.z == 0) return false;
	else if (dist.z > 0) {
		zdir = 1;
		if (n.z > zMax) return false;
		if (n.z < 0 &&
			(sx != 0 || sz < 0) &&
			(sx != xMax || sz < 0))
		{
			sx = (int)(1.0f / slope * (0 - n.z) + n.x);
			sz = 0;
		}
	}
	else {
		zdir = -1;
		if (n.z < 0) return false;
		if (n.z > zMax &&
			(sx != 0 || sz > zMax) &&
			(sx != xMax || sz > zMax)) {
			sx = (int)(1.0f / slope * (zMax - n.z) + n.x);
			sz = zMax;
		}
	}

	int nRight = (int)Width - (xdir + 1) / 2;
	int nTop = (int)Height - (zdir + 1) / 2;
	int nLeft = 0 - (xdir - 1) / 2;
	int nBottom = 0 - (zdir - 1) / 2;

	for (int x = sx; x < nRight; x += xdir)
	{
		if (x * xdir > ex * xdir ||
			x < nLeft)
			return false;

		z2 = (int)(slope * (x + xdir - sx)) + sz + zdir;

		// z 최대 최소 체크
		if (zdir == -1) swap(z1, z2);
		{
			if (z1 * zdir > ez * zdir ||
				z2 < nBottom ||
				z1 >= nTop)
				return false;

			if (z1 < nBottom) z1 = nBottom;
			if (z2 >= nTop) z2 = nTop - 1;
		}
		if (zdir == -1) swap(z1, z2);

		for (int z = z1; z * zdir <= z2 * zdir; z += zdir)
		{
			UINT index[4];
			index[0] = Width * (z)+(x);  // 왼아
			index[1] = Width * (z + zdir) + (x);  // 왼위
			index[2] = Width * (z)+(x + xdir);  // 오아
			index[3] = Width * (z + zdir) + (x + xdir);  // 오위

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
			{
				p[i] = Vertices[index[i]].Position;
			}

			float u, v, distance;
			if (D3DXIntersectTri(p + 0, p + 1, p + 2, &n, &dist, &u, &v, &distance))
			{
				(*outPoint) = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				return true;
			}
			else if (D3DXIntersectTri(p + 3, p + 1, p + 2, &n, &dist, &u, &v, &distance))
			{
				(*outPoint) = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				return true;
			}

			for (UINT i = 0; i < 4; i++)
				index[i] += (int)Width * (-zdir + 1) / 2 + 1;  // 왼아

			for (UINT i = 0; i < 4; i++)
			{
				if (index[i] >= MeshData->VertexCount)
					break;

				p[i] = Vertices[index[i]].Position;
				//vertices[index[i]]._Color = Color(0, 0, 1, 0); // test
			}
			if (D3DXIntersectTri(p + 0, p + 1, p + 2, &n, &dist, &u, &v, &distance))
			{
				(*outPoint) = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				return true;
			}
			else if (D3DXIntersectTri(p + 3, p + 1, p + 2, &n, &dist, &u, &v, &distance))
			{
				(*outPoint) = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				return true;
			}

		}

		z1 = z2 - zdir;
	}

	return false;
}

bool TerrainCollider::GetMouseRaycast2(Vector3 * outPoint)
{
	Matrix W;
	Transform->GlobalWorld(&W);
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();
	Vector3 mouse = Input::Mouse()->GetPosition();

	//mouse = Vector3(Screen::Width() * 0.5f, Screen::Height() * 0.5f, 0); // test
	for (UINT i = 0; i < MeshData->VertexCount; i++)
		Vertices[i]._Color = Color(0, 0, 0, 0); // test

	// 마우스 시작 지점
	mouse.z = 0.0f;
	Vector3 n = vp->Unproject(mouse, W, V, P);

	// 마우스 끝 지점
	mouse.z = 1.0f;
	Vector3 f = vp->Unproject(mouse, W, V, P);

	// 방향
	Vector3 dist = f - n;

	int xdir, zdir;
	if (dist.x == 0) return false;
	else if (dist.x > 0) { xdir = 1; }
	else { xdir = -1; }

	if (dist.z == 0) return false;
	else if (dist.z > 0) { zdir = 1; }
	else { zdir = -1; }

	int nRight, nTop, nLeft, nBottom;
	if (xdir == 1)
	{
		nRight = (int)Width - 2;
		nLeft = 0;
	}
	else
	{
		nRight = 1;
		nLeft = (int)Width - 1;
	}
	if (zdir == 1)
	{
		nTop = (int)Height - 2;
		nBottom = 0;
	}
	else
	{
		nTop = 1;
		nBottom = (int)Height - 1;
	}

	for (int x = nLeft; x * xdir < nRight * xdir; x += xdir)
	{
		for (int z = nBottom; z * zdir < nTop * zdir; z += zdir)
		{
			UINT index[4];
			index[0] = Width * (z)+(x);  // 왼아
			index[1] = Width * (z + zdir) + (x);  // 왼위
			index[2] = Width * (z)+(x + xdir);  // 오아
			index[3] = Width * (z + zdir) + (x + xdir);  // 오위

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
			{
				p[i] = Vertices[index[i]].Position;
				Vertices[index[i]]._Color = Color(0, 0, 1, 0); // test
			}

			float u, v, distance;
			if (D3DXIntersectTri(p + 0, p + 1, p + 2, &n, &dist, &u, &v, &distance))
			{
				(*outPoint) = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				return true;
			}
			else if (D3DXIntersectTri(p + 3, p + 1, p + 2, &n, &dist, &u, &v, &distance))
			{
				(*outPoint) = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				return true;
			}
		}
	}

	return false;
}

TerrainCollider::TerrainCollider(Terrain & value)
	: Vertices(value.Vertices())
	, Transform(value.GetTransform())
	, Width(value.Width())
	, Height(value.Height())
	, MeshData(value.GetMeshData())
{
}

#pragma endregion
