#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(wstring imageFile)
	: imageFile(imageFile), meshData(new MeshData())
	, shader(Shader::Load(URI::Shaders + L"01_Terrain.fxo"))
{
	ReadHeightData(); // 높이맵, 이미지맵 (dds)에서 높이 정보를 읽어오는 함수
	CreateVertexData();  // y = heightMap.r
	CreateIndexData();
	RecalculateNormals();

	perTransform = new PerTransform(shader);
	vertexBuffer = new VertexBuffer(meshData->Vertices, meshData->VertexCount, meshData->Stride, 0, true);
	indexBuffer = new IndexBuffer(meshData->Indices, meshData->IndexCount);

	material = new Material(shader);
	material->Diffuse(1, 1, 1, 1);
	material->Specular(1, 1, 1, 20);

	material->SetTexture("BaseMap", baseMap);

	shadow = new ShadowTest(shader, Vector3(128.0f, 64.0f, 128.0f), 128.0f);
	//shadow = new ShadowCaster(shader);
	//shadow->SetShadow_Global();
	//shadow->SetFuncPreRender(bind(&Terrain::PreRender_Depth, this));
}

Terrain::~Terrain()
{
	SafeDelete(shadow);

	SafeDelete(baseMap);

	SafeDeleteArray(heights);

	SafeDelete(indexBuffer);
	SafeDelete(vertexBuffer);
	SafeDelete(perTransform);
	SafeDelete(material);

	meshData->SafeDeleteData();
	SafeDelete(meshData);

	SafeRelease(shader);
}

void Terrain::Update()
{
	perTransform->Update();
}

void Terrain::PreRender_Depth()
{
	shadow->PreRender();

	perTransform->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//material->Render();
	shader->DrawIndexed(0, 2, meshData->IndexCount);
}

void Terrain::Render()
{
	perTransform->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Render();
	shader->DrawIndexed(0, pass, meshData->IndexCount);
}

void Terrain::RenderVisibleNormal()
{
	static UINT interval = 5;
	ImGui::SliderInt("Vertex Interval", (int*)&interval, 1, 5);

	for (UINT z = 0; z < height; z += interval)
	{
		UINT lineStart = width * z;

		for (UINT x = 0; x < width; x += interval)
		{
			UINT index = lineStart + x;
			Vector3 start = vertices[index].Position;
			Vector3 end = vertices[index].Position + vertices[index].Normal * 3.0f;
			Debug::Line->RenderLine(start, end);
		}
	}
}


void Terrain::RecalculateNormals()
{
	UINT count = meshData->IndexCount / 3;
	for (UINT i = 0; i < count; i++)
	{
		// 3각형 면의 인덱스
		UINT index0 = meshData->Indices[i * 3 + 0];
		UINT index1 = meshData->Indices[i * 3 + 1];
		UINT index2 = meshData->Indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].Position;
		Vector3 v1 = vertices[index1].Position;
		Vector3 v2 = vertices[index2].Position;

		Vector3 e1 = v1 - v0;
		Vector3 e2 = v2 - v0;

		Vector3 normal;
		D3DXVec3Cross(&normal, &e1, &e2);
		D3DXVec3Normalize(&normal, &normal);

		// 누적 시키고 노멀라이즈
		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;
	}

	for (UINT i = 0; i < meshData->VertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}

void Terrain::ApplyVertex()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, meshData->Stride * meshData->VertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
	material->SetTexture("BaseMap", baseMap);
}


#pragma region Picker

float Terrain::GetHeight(const Vector3 & position)
{
	Vector3 p[4];
	{
		UINT x = (UINT)position.x;
		UINT z = (UINT)position.z;

		if (x < 0 || x > width - 2) return FLT_MIN;
		if (z < 0 || z > height - 2) return FLT_MIN;

		UINT index[4];
		index[0] = width * (z + 0) + x + 0;  // 왼아
		index[1] = width * (z + 1) + x + 0;  // 왼위
		index[2] = width * (z + 0) + x + 1;  // 오아
		index[3] = width * (z + 1) + x + 1;  // 오위

		for (UINT i = 0; i < 4; i++)
			p[i] = vertices[index[i]].Position;
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
Vector3 Terrain::GetNormal(float _x, float _z)
{
	Vector3 result(0, 1, 0);

	VertexTerrain p[4];
	{
		UINT x = (UINT)_x;
		UINT z = (UINT)_z;

		if (x < 0 || x > width - 2) return result;
		if (z < 0 || z > height - 2) return result;

		UINT index[4];
		index[0] = width * (z + 0) + x + 0;  // 왼아
		index[1] = width * (z + 1) + x + 0;  // 왼위
		index[2] = width * (z + 0) + x + 1;  // 오아
		index[3] = width * (z + 1) + x + 1;  // 오위

		for (UINT i = 0; i < 4; i++)
			p[i] = vertices[index[i]];
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
float Terrain::GetVerticalRaycast(const Vector3 & position)
{
	Vector3 p[4];
	{
		UINT x = (UINT)position.x;
		UINT z = (UINT)position.z;

		if (x < 0 || x > width - 2) return FLT_MIN;
		if (z < 0 || z > height - 2) return FLT_MIN;

		UINT index[4];
		index[0] = width * (z + 0) + x + 0;  // 왼아
		index[1] = width * (z + 1) + x + 0;  // 왼위
		index[2] = width * (z + 0) + x + 1;  // 오아
		index[3] = width * (z + 1) + x + 1;  // 오위

		for (UINT i = 0; i < 4; i++)
			p[i] = vertices[index[i]].Position;
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
Vector3 Terrain::GetRaycastPosition(Vector3* start)
{
	Matrix W;
	GetTransform()->GlobalWorld(&W);
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* vp = Context::Get()->GetViewport();
	Vector3 mouse = Input::Mouse()->GetPosition();

	mouse.z = 0.0f;
	Vector3 n = vp->Unproject(mouse, W, V, P);

	mouse.z = 1.0f;
	Vector3 f = vp->Unproject(mouse, W, V, P);

	if (start == nullptr)
		start = &n;
	else (*start) = n;
	Vector3 direction = f - n;

	UINT* indices = meshData->Indices;
	UINT maxWidth = this->width - 1;
	UINT maxHeight = this->height - 1;
	for (UINT z = 0; z < maxHeight; z++)
	{
		for (UINT x = 0; x < maxWidth; x++)
		{
			UINT index[4];
			index[0] = width * (z + 0) + x + 0;  // 왼아
			index[1] = width * (z + 1) + x + 0;  // 왼위
			index[2] = width * (z + 0) + x + 1;  // 오아
			index[3] = width * (z + 1) + x + 1;  // 오위

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;
			if (D3DXIntersectTri(p + 0, p + 1, p + 2, start, &direction, /* out */ &u, /* out */ &v, /* out */ &distance))
			{
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
			}
			else if (D3DXIntersectTri(p + 3, p + 1, p + 2, start, &direction, /* out */ &u, /* out */ &v, /* out */ &distance))
			{
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
			}
		}
	}

	return Vector3(-1, -1, -1);
}

#pragma endregion


#pragma region Initalize

void Terrain::ReadHeightData()
{
	wstring ext = Path::GetExtension(imageFile);
	std::transform(ext.begin(), ext.end(), ext.begin(), toupper);

	if (ext == L"DDS")
	{
		Texture* texture = new Texture(imageFile);

		Texture2DDesc dstDesc;
		ID3D11Texture2D* readTexture = dstDesc.CopyResourceTexture(texture->GetTexture());
		width = dstDesc.Desc().Width;
		height = dstDesc.Desc().Height;

		UINT pixelCount = width * height;
		UINT* pixels = new UINT[pixelCount];

		dstDesc.MapCopyFromOutput(pixels, sizeof(UINT) * pixelCount);

		heights = new float[pixelCount];
		for (UINT i = 0; i < pixelCount; i++)
		{
			UINT temp = (pixels[i] & 0xFF000000) >> 24;
			heights[i] = (float)temp / 255.0f;
		}

		SafeRelease(readTexture);
		return;
	} // End DDS

	Texture* heightMap = new Texture(imageFile);

	vector<Color> pixels;
	heightMap->ReadPixel(&pixels);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();

	UINT pixelCount = width * height;
	heights = new float[pixelCount];
	for (UINT i = 0; i < pixelCount; i++)
		heights[i] = pixels[i].r;

	SafeDelete(heightMap);
}

void Terrain::CreateVertexData()
{
	vertices = meshData->NewVertices<VertexTerrain>(width * height);

	UINT wh = meshData->VertexCount;
	for (UINT z = 0; z < height; z++)
	{
		UINT lineStart = width * z;
		//UINT lineStartPixel = width * (height - z - 1);
		UINT lineStartPixel = wh - lineStart - width;

		for (UINT x = 0; x < width; x++)
		{
			UINT index = lineStart + x;
			UINT pixel = lineStartPixel + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = heights[pixel] * 255.0f / 10.0f;
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = x / (float)(width - 1);
			vertices[index].Uv.y = 1.0f - (z / (float)(height - 1));
		}
	}
}

void Terrain::CreateIndexData()
{
	UINT maxWidth = this->width - 1;
	UINT maxHeight = this->height - 1;

	meshData->NewIndices(maxWidth * maxHeight * 6);
	UINT* indices = meshData->Indices;

	UINT index = 0;
	for (UINT z = 0; z < maxHeight; z++)
	{
		for (UINT x = 0; x < maxWidth; x++)
		{
			indices[index + 0] = width * (z + 0) + (x + 0);
			indices[index + 1] = width * (z + 1) + (x + 0);
			indices[index + 2] = width * (z + 0) + (x + 1);
			indices[index + 3] = width * (z + 0) + (x + 1);
			indices[index + 4] = width * (z + 1) + (x + 0);
			indices[index + 5] = width * (z + 1) + (x + 1);
			index += 6;
		}
	}
}

#pragma endregion
