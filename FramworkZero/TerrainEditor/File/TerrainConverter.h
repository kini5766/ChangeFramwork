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

public:
	void ExportFile(wstring file);
	void ClearData();

private:
	UINT* heights = nullptr;
	UINT width = 0;
	UINT height = 0;
};