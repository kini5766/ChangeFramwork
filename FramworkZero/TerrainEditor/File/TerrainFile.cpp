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
			// ���̾�α�
			Path::OpenFileDialog(heightMapFileName, Path::ImageFilter, heightMapFileDirectory, 
				bind(&TerrainFile::OpenComplete, this, placeholders::_1), WinDesc::GetHandle());

			SafeDelete(heightMapFile);
			heightMapFile = new Texture(heightMapFileDirectory + heightMapFileName);
		}

		// �޺� ä�� ����
		const char* items[] = { "Red", "Green", "Blue", "Alpha" };
		ImGui::Combo("Channel", &selectedChannel, items, 4);

		// ����Ʈ ��ư
		if (ImGui::Button("Convert Map File"))
		{
			converter->SetHeightMapTexture(heightMapFile, (TerrainConverter::Channel)selectedChannel);

			wstring fileName = Path::GetFileNameWithoutExtension(heightMapFile->GetFile());
			fileName = URI::Textures + heightMapFileDirectory + fileName + L".dds";
			converter->ExportFile(fileName);
			UpdateDataMapFileList();
		}

		// �ӱ��� �̹���
		if (heightMapFile != nullptr)
			ImGui::Image(heightMapFile->SRV(), ImVec2(256, 256));
	}

	ImGui::Separator();

	// dds ����Ʈ -> ��ư�� ������ -> ������ dds�� ���� ����
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

	Terrain* terrain = brush->GetTerrain();

	UINT width = terrain->Width();
	UINT height = terrain->Height();
	UINT pixelCount = width * height;

	{
		UINT* heights = new UINT[pixelCount];
		Terrain::VertexTerrain* vertices = terrain->Vertices();
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = (width)* z + x;  // ���ý��ε���

				UINT pexel = width * (height - z - 1) + x;  // uv ��ǥ ü�� ������

				float heightPexel = vertices[index].Position.y / TERRAIN_TEXTURE_HEIGHT;

				// ���� �����ؼ� ���ġ ���� �� ����
				if (heightPexel < 0.0f)
					heightPexel = 0.0f;
				else if (heightPexel > 1.0f)
					heightPexel = 1.0f;

				heights[pexel] = (UINT)(heightPexel * 255.0f);
			}
		}
		c.SetHeights(heights, width, height);
		SafeDelete(heights);
	}

	// Layer1
	{
		UINT* alphas = new UINT[pixelCount];
		float* value = terrain->Layer1Data();
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = width * z + x;
				alphas[index] = (UINT)(value[index] * 255.0f);
			}
		}

		c.SetAlphaLayer1(alphas);
		SafeDelete(alphas);
	}

	// Layer2
	{
		UINT* alphas = new UINT[pixelCount];
		float* value = terrain->Layer2Data();
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = width * z + x;
				alphas[index] = (UINT)(value[index] * 255.0f);
			}
		}

		c.SetAlphaLayer2(alphas);
		SafeDelete(alphas);
	}

	// Layer3
	{
		UINT* alphas = new UINT[pixelCount];
		float* value = terrain->Layer3Data();
		for (UINT z = 0; z < height; z++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = width * z + x;
				alphas[index] = (UINT)(value[index] * 255.0f);
			}
		}

		c.SetAlphaLayer3(alphas);
		SafeDelete(alphas);
	}

	c.ExportFile(fileName);

	UpdateDataMapFileList();
}

void TerrainFile::UpdateDataMapFileList()
{
	dataMapFileList.clear();
	Path::GetFiles(&dataMapFileList, URI::Textures + heightMapFileDirectory, L"*.dds", false);

	for (wstring& file : dataMapFileList)
		file = Path::GetFileNameWithoutExtension(file);
}
