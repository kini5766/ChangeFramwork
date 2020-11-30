#include "Framework.h"
#include "Terrain.h"
#include "Viewer/Freedom.h"
#include "TerrainBrush.h"

using namespace ShaderEffctConstantName;

Terrain::Terrain(Shader * shader, wstring _heightMap)
	: Renderer(shader)
{
	sBaseMap = shader->AsSRV("BaseMap");
	sAlphaMap = shader->AsSRV("AlphaMap");
	sLayerMap = shader->AsSRV("LayerMap");


	brush = new TerrainBrushDemo(shader);

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	sLineBuffer = shader->AsConstantBuffer(CB_TerrainLine);

	heightMap = new Texture(_heightMap);

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(TerrainVertex), 0, true, false);
	indexBuffer = new IndexBuffer(indices, indexCount);

	brush->SetVertex(vertices, width, height);
}
#pragma region Create

void Terrain::CreateVertexData()
{
	vector<Color> heights;
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, &heights);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();

	vertexCount = (width) * (height);
	vertices = new TerrainVertex[vertexCount];

	int debug = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = (width)* z + x;  // 저차원 값
			UINT pexel = width * (height - z - 1) + x;  // uv 좌표 체계 뒤집기

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = heights[pexel].r * TEXTURE_HEIGHT; // 빨강
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = (float)x / (float)width;
			vertices[index].Uv.y = (float)(height - z - 1) / (float)height;
		}
	}

}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];
	UINT index = 0;
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = (width)* z + x;
			indices[index + 1] = (width) * (z + 1) + x;
			indices[index + 2] = (width)* z + (x + 1);
			indices[index + 3] = (width)* z + (x + 1);
			indices[index + 4] = (width) * (z + 1) + x;
			indices[index + 5] = (width) * (z + 1) + (x + 1);

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	UINT size = indexCount / 3;
	for (UINT i = 0; i < size; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		TerrainVertex v0 = vertices[index0];
		TerrainVertex v1 = vertices[index1];
		TerrainVertex v2 = vertices[index2];

		Vector3 d1 = v1.Position - v0.Position;
		Vector3 d2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].Normal = normal;
		vertices[index1].Normal = normal;
		vertices[index2].Normal = normal;
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}

#pragma endregion


Terrain::~Terrain()
{
	SafeDelete(brush);
	SafeDelete(lineBuffer);
	SafeDelete(indexBuffer);
	SafeDelete(vertexBuffer);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(heightMap);
	SafeDelete(baseMap);
	SafeDelete(layerMap);
	SafeDelete(alphaMap);
}


#pragma region Renderer

void Terrain::Update()
{
	Renderer::Update();

	UpdateBrush();

	ImGui::Separator();
	brush->RenderImGui();
	if (ImGui::Button("SaveBrush"))
	{
		SaveHeightMap();
	}

	// UpdateLine();
}

void Terrain::Render()
{
	Renderer::Render();

	if (baseMap != nullptr)
		sBaseMap->SetResource(baseMap->SRV());

	if (layerMap != nullptr && alphaMap != nullptr)
	{
		sAlphaMap->SetResource(alphaMap->SRV());
		sLayerMap->SetResource(layerMap->SRV());
	}

	brush->Render();

	lineBuffer->Render();
	sLineBuffer->SetConstantBuffer(lineBuffer->Buffer());

	shader->DrawIndexed(0, Pass(), indexCount);  // 인덱스 카운트 개수를 넘김
}

#pragma endregion


#pragma region Update

void Terrain::UpdateBrush()
{
	UINT shape = brush->Shape();
	if (shape > 0)
	{
		bool bMouseOver = ImGui::GetIO().WantCaptureMouse;
		if (Mouse::Get()->Down(0) && bMouseOver == false)
		{
			// world
			Vector3 mouse = Mouse::Get()->GetPosition();
			prevMousePos = mouse;
			Vector3 picked = GetPickedPosition();

			brush->StartBrush(picked);
		}
		else if (Mouse::Get()->Press(0) && bMouseOver == false)
		{
			Vector3 mouse = Mouse::Get()->GetPosition();
			if (mouse != prevMousePos)
			{
				prevMousePos = mouse;

				Vector3 picked;
				picked = GetPickedPosition();
				brush->SetLocation(picked);
			}

			brush->UpdateBrush(vertices);
			UpdateVertices();
		}
		else if (Mouse::Get()->Up(0) && bMouseOver == false)
		{
			brush->EndBrush();
		}
		else
		{
			Vector3 picked;
			picked = GetPickedPosition();
			brush->SetLocation(picked);
		}
	}
}

void Terrain::UpdateVertices()
{
	CreateNormalData();

	//D3D::GetDC()->UpdateSubresource(
	//	vertexBuffer->Buffer(), 0, nullptr, vertices, sizeof(TerrainVertex) * vertexCount, 0
	//);

	D3D11_MAPPED_SUBRESOURCE subResoure;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResoure);
	{
		memcpy(subResoure.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::UpdateLine()
{
	ImGui::Separator();
	ImGui::ColorEdit3("Color", lineDesc.Color);

	ImGui::InputInt("Visible", (int*)&lineDesc.Visible);
	lineDesc.Visible %= 2;

	ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.001f);
	lineDesc.Thickness = Math::Clamp(lineDesc.Thickness, 0.01f, 0.9f);

	ImGui::InputFloat("Size", &lineDesc.Size);
}

#pragma endregion


#pragma region Map

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void Terrain::LayerMap(wstring _layerMap, wstring _alphaMap)
{
	SafeDelete(layerMap);
	SafeDelete(alphaMap);

	layerMap = new Texture(_layerMap);
	alphaMap = new Texture(_alphaMap);
}

void Terrain::SaveHeightMap()
{
	// 텍스쳐 저장코드
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;  // 1은 기본 값
	desc.ArraySize = 1;  // Texture2DArray의 개수
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  // 바이트의 크기
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	// desc.Usage = D3D11_USAGE_IMMUTABLE; // 수정불가 (저장이 안 됨)
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	UINT* colors = new UINT[vertexCount];

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = (width)* z + x;  // 버택스인덱스
			UINT pexel = width * (height - z - 1) + x;  // uv 좌표 체계 뒤집기

			float heightPexel = (vertices[index].Position.y) / TEXTURE_HEIGHT;
			
			// 높이 수정해서 허용치 밖일 때 조정
			if (heightPexel < 0.0f)
				heightPexel = 0.0f;
			else if (heightPexel > 1.0f)
				heightPexel = 1.0f;

			// colors[pexel] = (UINT)Color(0.0f, 0.0f, heightPexel, 1.0f);
			UINT a = 255;
			UINT b = (UINT)(heightPexel * 255.0f);
			UINT g = b;
			UINT r = b;

			colors[pexel] = 0;
			colors[pexel] |= a << 24;
			colors[pexel] |= b << 16;
			colors[pexel] |= g << 8;
			colors[pexel] |= r << 0;
		}
	}

	subResource.pSysMem = colors;  // 데이터
	subResource.SysMemPitch = desc.Width * sizeof(UINT);  // 가로(바이트)
	subResource.SysMemSlicePitch = desc.Width * desc.Height * sizeof(UINT);  // 가로 * 세로(바이트)

	ID3D11Texture2D* texture;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &texture));

	// 저장
	wstring file = heightMap->GetFile();
	file = URI::Textures + file;

	// 파일 입출력은 인터럭트 발생 -> 스레드로 빼야됨
	Check(D3DX11SaveTextureToFile(D3D::GetDC(), texture, D3DX11_IFF_PNG, file.c_str()));
	// FileA : 아스키코드모드
	// Check(hr = D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, file.c_str()));

	SafeRelease(texture);
	SafeDeleteArray(colors);
}

#pragma endregion


#pragma region Pick

// 기울기를 이용
float Terrain::GetHeight(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	// FLT_MIN;
	if (x < 0 || x > width)
		return position.y;

	if (z < 0 || z > height)
		return position.y;

	UINT index[4];
	index[0] = width * z + x;  // 좌하
	index[1] = width * (z + 1) + x; // 좌상
	index[2] = width * z + x + 1; // 우하
	index[3] = width * (z + 1) + x + 1;  // 우상

	Vector3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;

	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	// z의 y증가량 * z의 사이값 + x의 y증가량 * y의 사이값 + 시작y
	// (2.y - 0.y) * ddx + (1.y - 0.y) * ddz + 0.y

	Vector3 result;

	// (v[2] - v[0]) : x증가량
	// (v[1] - v[0]) : y증가량
	// ddx, ddy : 기준점 기준 상대 위치
	if (ddx + ddz <= 1)
		result = (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz + v[0];
	else
	{
		ddx = 1.0f - ddx;
		ddz = 1.0f - ddz;

		result = (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz + v[3];
	}

	return result.y;
}

float Terrain::GetHeightPick(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	// FLT_MIN;
	if (x < 0 || x > width)
		return position.y;

	if (z < 0 || z > height)
		return position.y;

	UINT index[4];
	index[0] = width * z + x;  // 좌하
	index[1] = width * (z + 1) + x; // 좌상
	index[2] = width * z + x + 1; // 우하
	index[3] = width * (z + 1) + x + 1;  // 우상

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	Vector3 start(position.x, 1000, position.z);
	Vector3 direction(0, -1, 0);

	Vector3 result(-1, FLT_MIN, -1);

	float u, v, distance;
	// 점점 순서 산관없음
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance))
		result = (p[1] - p[0]) * u + (p[2] - p[0]) * v + p[0];

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance))
		result = (p[1] - p[3]) * u + (p[2] - p[3]) * v + p[3];

	return result.y;
}

Vector3 Terrain::GetPickedPosition()
{
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();

	// world
	Vector3 mouse = Mouse::Get()->GetPosition();
	Viewport* vp = Context::Get()->GetViewport();

	Vector3 n, f;
	mouse.z = 0.0f;
	vp->UnProject(&n, mouse, transform->World(), V, P);

	mouse.z = 1.0f;
	vp->UnProject(&f, mouse, transform->World(), V, P);

	Vector3 start = n;
	Vector3 direction = f - n;

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < height - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;  // 좌하
			index[1] = width * (z + 1) + x; // 좌상
			index[2] = width * z + x + 1; // 우하
			index[3] = width * (z + 1) + x + 1;  // 우상

			Vector3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		}
	}
	return Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
}

#pragma endregion




// 조명 종류
// Ambient
// Diffuse
// Specular
// Emissive