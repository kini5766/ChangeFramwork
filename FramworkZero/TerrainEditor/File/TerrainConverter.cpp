#include "stdafx.h"
#include "TerrainConverter.h"

TerrainConverter::TerrainConverter()
{
}

TerrainConverter::~TerrainConverter()
{
	ClearData();
}

void TerrainConverter::SetHeightMapTexture(Texture* texture, Channel channel)
{
	// heightMapFile -> heights;
	Texture2DDesc dstDesc;
	ID3D11Texture2D* readTexture = dstDesc.CopyResourceTexture(texture->GetTexture());
	width = dstDesc.Desc().Width;
	height = dstDesc.Desc().Height;

	UINT pixelCount = width * height;
	UINT* pixels = new UINT[pixelCount];

	dstDesc.MapCopyFromOutput(pixels, sizeof(UINT) * pixelCount);

	SafeDeleteArray(heights);
	heights = new UINT[pixelCount];
	for (UINT i = 0; i < pixelCount; i++)
	{
		// 0xAABBGGRR -> 0xXXXXXX00
		UINT pixel = pixels[i];
		UINT result = 0;

		switch (channel)
		{
		case Channel::Alpha: result = (pixel & 0xFF000000) >> 24; break;
		case Channel::Blue:  result = (pixel & 0x00FF0000) >> 16; break;
		case Channel::Green: result = (pixel & 0x0000FF00) >> 8;  break;
		case Channel::Red:   result = (pixel & 0x000000FF) >> 0;  break;
		}

		heights[i] = result;
	}
	SafeDeleteArray(pixels);
	SafeRelease(readTexture);
}

void TerrainConverter::SetHeights(const UINT * heights, UINT width, UINT height)
{
	this->width = width;
	this->height = height;

	UINT pixelCount = width * height;

	SafeDeleteArray(this->heights);
	this->heights = new UINT[pixelCount];
	memcpy(this->heights, heights, pixelCount * sizeof(UINT));
}

void TerrainConverter::ExportFile(wstring file)
{
	if (heights == nullptr)
		return;

	Texture2DDesc saveDesc;

	D3D11_TEXTURE2D_DESC& desc = saveDesc.Desc();
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	UINT pixelCount = width * height;
	UINT* colors = new UINT[pixelCount];
	for (UINT i = 0; i < pixelCount; i++)
	{
		// Set heights
		colors[i] = 0u;
		colors[i] |= 0u << 8;
		colors[i] |= 0u << 16;
		colors[i] |= heights[i] << 24;
	}

	// save data Texture
	saveDesc.SetColors(colors);
	ID3D11Texture2D* saveTexture = saveDesc.CreateTexture(sizeof(UINT));
	SafeDeleteArray(colors);

	D3DX11SaveTextureToFile(D3D::GetDC(), saveTexture, D3DX11_IFF_DDS, (URI::Textures + file).c_str());
	SafeRelease(saveTexture);
}

void TerrainConverter::ClearData()
{
	SafeDeleteArray(heights);
}

