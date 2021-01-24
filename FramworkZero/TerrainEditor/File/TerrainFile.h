#pragma once

class TerrainFile
{
public:
	TerrainFile(class Brush* brush);
	~TerrainFile();

public:
	void RenderImGui();
	void SetFuncOpen(const function<void(wstring)>& value);

private:
	void UpdateDataMapFileList();
	void OpenComplete(wstring fileName);
	void SaveBrushComplete(wstring fileName);

private:
	function<void(wstring)> funcOpenTerrain;

private:
	wstring dataFileName;
	class Brush* brush;

private:
	class TerrainConverter* converter;
	wstring heightMapFileDirectory = L"Terrain/";
	wstring heightMapFileName = L"Gray256.png";
	Texture* heightMapFile = nullptr;
	int selectedChannel = 0;

private:
	vector<wstring> dataMapFileList;
};