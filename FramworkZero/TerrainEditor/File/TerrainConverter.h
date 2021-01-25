#pragma once

class TerrainConverter
{
public:
	enum class Channel { Red = 0, Green, Blue, Alpha, Max };

public:
	TerrainConverter();
	~TerrainConverter();

public:
	void SetHeightMapTexture(Texture* texture, Channel channel);
	void SetHeights(const UINT* heights, UINT width, UINT height);

	void SetAlphaLayer1(const UINT* value);
	void SetAlphaLayer2(const UINT* value);
	void SetAlphaLayer3(const UINT* value);

public:
	void ExportFile(wstring file);
	void ClearData();

private:
	UINT pixelCount;
	UINT* heights = nullptr;
	UINT* alphaLayer1;
	UINT* alphaLayer2;
	UINT* alphaLayer3;
	UINT width = 0;
	UINT height = 0;
};