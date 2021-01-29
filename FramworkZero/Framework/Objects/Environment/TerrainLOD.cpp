#include "Framework.h"
#include "TerrainLOD.h"

TerrainLOD::TerrainLOD(wstring ddsFile)
	: ddsFile(ddsFile)
{
	shader = Shader::Load(L"01_TerrainLOD.fxo");
	meshData = new MeshData();

	ReadDataMap();  // out width, height

	CreatePatchMeshData();

	renderer = new MeshRenderer(shader, meshData);
	perTransform = new PerTransform(shader);
	buffer = new ConstantBuffer(&desc, sizeof(Desc));

	material = renderer->GetDefaultMaterial();
	material->SetConstantBuffer("CB_TerrainLOD", buffer->Buffer());
	material->SetSRV("HeightMap", heightMapSRV);
	material->SetSRV("BaseMap", nullptr);
	material->Emissive() = Color(0.8f, 0.8f, 0.8f, 0.5f);
	material->Specular() = Color(0.8f, 0.8f, 0.8f, 20);

	meshData->SubMeshes[0].topology =
		D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

	desc.CellSpacingU = 1.0f / width;
	desc.CellSpacingV = 1.0f / height;
	shadow = new ShadowTest(shader, Vector3(-64.0f, 64.0f, 64.0f), 512.0f);

	layer1.sSRV = shader->AsSRV("Layer1AlphaMap");
	layer1.sMap = shader->AsSRV("Layer1ColorMap");
	layer2.sSRV = shader->AsSRV("Layer2AlphaMap");
	layer2.sMap = shader->AsSRV("Layer2ColorMap");
	layer3.sSRV = shader->AsSRV("Layer3AlphaMap");
	layer3.sMap = shader->AsSRV("Layer3ColorMap");
}

TerrainLOD::~TerrainLOD()
{
	SafeDelete(shadow);

	SafeRelease(heightMap);
	SafeRelease(heightMapSRV);
	SafeDeleteArray(heights);

	SafeDelete(buffer);
	SafeDelete(perTransform);
	SafeDelete(renderer);
	meshData->SafeDeleteData();
	SafeDelete(meshData);

	SafeRelease(shader);
}

void TerrainLOD::Update()
{
	perTransform->Update();
}

void TerrainLOD::Render()
{
	perTransform->Render();
	buffer->Render();

	if (layer1.Map != nullptr)
	{
		layer1.sSRV->SetResource(layer1.SRV);
		layer1.sMap->SetResource(layer1.Map->SRV());
	}
	if (layer2.Map != nullptr)
	{
		layer2.sSRV->SetResource(layer2.SRV);
		layer2.sMap->SetResource(layer2.Map->SRV());
	}
	if (layer3.Map != nullptr)
	{
		layer3.sSRV->SetResource(layer3.SRV);
		layer3.sMap->SetResource(layer3.Map->SRV());
	}

	renderer->Pass(0);
	renderer->Render();

	shadow->RenderImGui();
}

void TerrainLOD::PreRender_Depth()
{
	shadow->PreRender();
	buffer->Render();

	perTransform->Render();

	renderer->Pass(1);
	renderer->Render();
}

void TerrainLOD::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
	material->SetSRV("BaseMap", baseMap->SRV());
}

void TerrainLOD::Layer1(wstring file)
{
	SafeDelete(layer1.Map);

	layer1.Map = new Texture(file);
}

void TerrainLOD::Layer2(wstring file)
{
	SafeDelete(layer2.Map);

	layer2.Map = new Texture(file);
}

void TerrainLOD::Layer3(wstring file)
{
	SafeDelete(layer3.Map);

	layer3.Map = new Texture(file);
}

float TerrainLOD::GetWidth() const
{
	return width * desc.CellSpacing;
}

float TerrainLOD::GetHeight() const
{
	return height * desc.CellSpacing;
}

void TerrainLOD::ApplyHeightMap()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(heightMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, heights, sizeof(float) * width * height);
	}
	D3D::GetDC()->Unmap(heightMap, 0);
}

void TerrainLOD::ApplyAlphasLayer1()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(layer1.Texture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, layer1.Data, sizeof(float) * width * height);
	}
	D3D::GetDC()->Unmap(layer1.Texture2D, 0);
}

void TerrainLOD::ApplyAlphasLayer2()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(layer2.Texture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, layer2.Data, sizeof(float) * width * height);
	}
	D3D::GetDC()->Unmap(layer2.Texture2D, 0);
}

void TerrainLOD::ApplyAlphasLayer3()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(layer3.Texture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, layer3.Data, sizeof(float) * width * height);
	}
	D3D::GetDC()->Unmap(layer3.Texture2D, 0);
}

void TerrainLOD::ReadDataMap()
{
	wstring ext = Path::GetExtension(ddsFile);
	std::transform(ext.begin(), ext.end(), ext.begin(), toupper);

	assert(ext == L"DDS");

	Texture* texture = new Texture(ddsFile);

	Texture2DDesc srcDesc;

	ID3D11Texture2D* readTexture =
		srcDesc.CopyResourceTexture(texture->GetTexture());

	width = srcDesc.Desc().Width;
	height = srcDesc.Desc().Height;
	UINT pixelCount = width * height;

	UINT* pixels = new UINT[pixelCount];
	srcDesc.MapCopyFromOutput(pixels, sizeof(UINT) * pixelCount);

	heights = new float[pixelCount];
	layer1.Data = new float[pixelCount];
	layer2.Data = new float[pixelCount];
	layer3.Data = new float[pixelCount];

	float charToPer = 1 / 255.0f;
	for (UINT i = 0; i < pixelCount; i++)
	{
		UINT temp;
		temp = (pixels[i] & 0xFF000000) >> 24;
		heights[i] = (float)temp * charToPer;
		//heights[i] = (float)(i % width) / (float)width;

		temp = (pixels[i] & 0x000000FF) >> 0;
		layer1.Data[i] = (float)temp * charToPer;

		temp = (pixels[i] & 0x0000FF00) >> 8;
		layer2.Data[i] = (float)temp * charToPer;

		temp = (pixels[i] & 0x00FF0000) >> 16;
		layer3.Data[i] = (float)temp * charToPer;
	}

	SafeDelete(pixels);
	SafeRelease(readTexture);
	SafeDelete(texture);


	Texture2DDesc dstDesc(srcDesc);
	D3D11_TEXTURE2D_DESC& desc = dstDesc.Desc();
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	dstDesc.SetColors(heights);
	heightMap = dstDesc.CreateTexture(4u);
	heightMapSRV = dstDesc.CreateSRV();

	dstDesc.SetColors(layer1.Data);
	layer1.Texture2D = dstDesc.CreateTexture(4u);
	layer1.SRV = dstDesc.CreateSRV();

	dstDesc.SetColors(layer2.Data);
	layer2.Texture2D = dstDesc.CreateTexture(4u);
	layer2.SRV = dstDesc.CreateSRV();

	dstDesc.SetColors(layer3.Data);
	layer3.Texture2D = dstDesc.CreateTexture(4u);
	layer3.SRV = dstDesc.CreateSRV();
}

void TerrainLOD::CreatePatchMeshData()
{
	UINT patchWidth, patchHeight;

	// 256Ä­ -> 8Ä­
	patchWidth = ((width - 1) / cellsPerPatch) + 1;
	patchHeight = ((height - 1) / cellsPerPatch) + 1;

	TerrainVertex* vertices =
		meshData->NewVertices<TerrainVertex>((patchWidth) * (patchHeight));

	float halfWidth = GetWidth() * 0.5f;
	float halfHeight = GetHeight() * 0.5f;

	float xScale = GetWidth() / (patchWidth - 1);
	float zScale = GetHeight() / (patchHeight - 1);

	float du = 1.0f / (patchWidth - 1);
	float dv = 1.0f / (patchHeight - 1);

	for (UINT z = 0; z < patchHeight; z++)
	{
		float tempZ =  z * -zScale + halfHeight;

		for (UINT x = 0; x < patchWidth; x++)
		{
			float tempX = x * xScale - halfWidth;

			vertices[(patchWidth) * z + x].Position = Vector3(tempX, 0.0f, tempZ);
			vertices[(patchWidth) * z + x].Uv.x = x * du;
			vertices[(patchWidth) * z + x].Uv.y = z * dv;
		}
	}

	UINT patchCount = (patchWidth - 1) * (patchHeight - 1);
	meshData->NewIndices(patchCount * 4);  // * 6Àº ¾È ÇÔ

	UINT* indices = meshData->Indices;

	UINT index = 0;
	for (UINT z = 0; z < patchHeight - 1; z++)
	{
		for (UINT x = 0; x < patchWidth - 1; x++)
		{
			indices[index + 0] = (patchWidth) * (z + 0) + (x + 0);
			indices[index + 1] = (patchWidth) * (z + 0) + (x + 1);
			indices[index + 2] = (patchWidth) * (z + 1) + (x + 0);
			indices[index + 3] = (patchWidth) * (z + 1) + (x + 1);

			index += 4;
		}
	}
}
