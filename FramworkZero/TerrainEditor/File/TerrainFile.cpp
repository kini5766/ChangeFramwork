#include "stdafx.h"
#include "TerrainFile.h"

#include "Brush.h"
#include "TerrainConverter.h"

TerrainFile::TerrainFile(Brush* brush)
	: brush(brush)
{
	converter = new TerrainConverter();

	UpdateDataMapFileList();
}

TerrainFile::~TerrainFile()
{
	SafeDelete(converter);
}

void TerrainFile::RenderImGui()
{
	if (ImGui::CollapsingHeader("Height Map Convert", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Open HeightMap"))
		{
			// 다이얼로그
			Path::OpenFileDialog(heightMapFileName, Path::ImageFilter, heightMapFileDirectory, 
				bind(&TerrainFile::OpenComplete, this, placeholders::_1), WinDesc::GetHandle());

			SafeDelete(heightMapFile);
			heightMapFile = new Texture(heightMapFileDirectory + heightMapFileName);
		}

		// 콤보 채널 선택
		const char* items[] = { "Red", "Green", "Blue", "Alpha" };
		ImGui::Combo("Channel", &selectedChannel, items, 4);

		// 컨버트 버튼
		if (ImGui::Button("Convert Map File"))
		{
			converter->SetHeightMapTexture(heightMapFile, (TerrainConverter::Channel)selectedChannel);

			wstring fileName = Path::GetFileNameWithoutExtension(heightMapFile->GetFile());
			fileName = URI::Textures + heightMapFileDirectory + fileName + L".dds";
			converter->ExportFile(fileName);
			UpdateDataMapFileList();
		}

		// 임구이 이미지
		if (heightMapFile != nullptr)
			ImGui::Image(heightMapFile->SRV(), ImVec2(256, 256));
	}

	ImGui::Separator();

	// dds 리스트 -> 버튼을 누르면 -> 선택한 dds로 지형 생성
	if (ImGui::CollapsingHeader("Data Map List", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (UINT i = 0; i < dataMapFileList.size(); i++)
		{
			if (ImGui::Button(String::ToString(dataMapFileList[i]).c_str(), ImVec2(200, 0)))
			{
				dataFileName = dataMapFileList[i] + L".dds";
				wstring path = heightMapFileDirectory + dataFileName;
				funcOpenTerrain(path);
			}
		}
	}

	if (brush->GetTerrain() == nullptr)
		return;

	ImGui::Separator();

	if (ImGui::Button("Save Brush"))
	{
		Path::SaveFileDialog(dataFileName, L".dds", heightMapFileDirectory,
			bind(&TerrainFile::SaveBrushComplete, this, placeholders::_1), WinDesc::GetHandle());

	}
}

void TerrainFile::SetFuncOpen(const function<void(wstring)>& value)
{
	funcOpenTerrain = value;
}

void TerrainFile::OpenComplete(wstring fileName)
{
	heightMapFileName = Path::GetFileName(fileName);
}

void TerrainFile::SaveBrushComplete(wstring fileName)
{
	fileName  = heightMapFileDirectory + Path::GetFileName(fileName);

	TerrainConverter c;

	UINT width = brush->GetTerrain()->Width();
	UINT height = brush->GetTerrain()->Height();
	UINT* heights = new UINT[width * height];
	brush->GetTerrain()->GetHeights(&heights);
	c.SetHeights(heights, width, height);
	c.ExportFile(fileName);
	SafeDelete(heights);

	UpdateDataMapFileList();
}

void TerrainFile::UpdateDataMapFileList()
{
	dataMapFileList.clear();
	Path::GetFiles(&dataMapFileList, URI::Textures + heightMapFileDirectory, L"*.dds", false);

	for (wstring& file : dataMapFileList)
		file = Path::GetFileNameWithoutExtension(file);
}
