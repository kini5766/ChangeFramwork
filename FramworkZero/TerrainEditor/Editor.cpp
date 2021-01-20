#include "stdafx.h"
#include "Editor.h"

#include "Brush.h"

void Editor::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(20, 0, 0);
	Context::Get()->MainCamera()->Position(126, 35, 39);
	((Freedom*)Context::Get()->MainCamera())->Speed(100);

	heightMapFileDirectory = L"Terrain/";
	heightMapFileName = L"Gray256.png";
	shader = Shader::Load(L"01_Mesh.fxo");

	sky = new CubeSky(L"Environment/Mountain1024.dds");
	brush = new Brush();

	funcOpen = bind(&Editor::OpenComplete, this, placeholders::_1);
	UpdateDataMapFileList();

	peojTexure = new ProjectionTexture(Shader::Load(URI::Shaders + L"01_Terrain.fxo"), L"Environment/MagicCircle.png", 217.0f, 220.0f);

	shadow = new Shadow(Vector3(128, 0, 128), 65.0f);
	Shadow::SetGlobal(shadow);
}

void Editor::Destroy()
{
	Shadow::SetGlobal(nullptr);
	SafeDelete(shadow);

	SafeDelete(peojTexure);

	SafeDelete(brush);
	SafeDelete(terrain);

	SafeDelete(sky);
	SafeRelease(shader);
}

void Editor::Update()
{
	ImGui::Begin("ImGui Test");

	ImGui::SliderFloat3("Directional Light", Lighting::Get()->DirectionalDesc()->Direction, -1.0f, 1.0f);

	//if (ImGui::Button("Direction Light"))
	//	Debug::Gizmo->SetTransform(Lighting::Get()->GetDirectional()->GetTransform());

	peojTexure->Update();
	//shadow->ImGuiRender();

	ImGui::End();

	// 다이얼로그
	{
		if (ImGui::Button("Open HeightMap"))
		{
			Path::OpenFileDialog(heightMapFileName, Path::ImageFilter, heightMapFileDirectory, funcOpen, WinDesc::GetHandle());
		
			SafeDelete(heightMapFile);
			heightMapFile = new Texture(heightMapFileDirectory + heightMapFileName);
		}

		// 콤보 채널 선택
		const char* items[] = { "Red", "Green", "Blue", "Alpha" };
		ImGui::Combo("Channel", &selectedChannel, items, 4);

		// 컨버트 버튼
		if (ImGui::Button("Convert Map File"))
		{
			ToDataMapFile();
			UpdateDataMapFileList();
		}

		// 임구이 이미지
		if (heightMapFile != nullptr)
			ImGui::Image(heightMapFile->SRV(), ImVec2(256, 256));

		// dds 리스트 -> 버튼을 누르면 -> 선택한 dds로 지형 생성
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Data Map List", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (UINT i = 0; i < dataMapFileList.size(); i++)
			{
				if (ImGui::Button(String::ToString(dataMapFileList[i]).c_str(), ImVec2(200, 0)))
				{
					SafeDelete(terrain);

					wstring path = heightMapFileDirectory + dataMapFileList[i] + L".dds";
					terrain = new Terrain(path);
					terrain->BaseMap(L"Terrain/Dirt3.png");

					brush->SetTerrain(terrain);
				}
			}

		}
	}

	sky->Update();
	if (terrain != nullptr)
	{
		brush->Update();
		//terrain->RenderVisibleNormal();
		terrain->Update();
	}
}

void Editor::PreRender()
{
	shadow->PreRender();
	if (terrain != nullptr)
		terrain->PreRender_Depth();
}

void Editor::Render()
{
	sky->Render();

	peojTexure->Render();

	if (terrain != nullptr)
	{
		brush->Render();
		terrain->Render();
	}
}

void Editor::OpenComplete(wstring fileName)
{
	heightMapFileName = Path::GetFileName(fileName);
}

void Editor::ToDataMapFile()
{
	if (heightMapFile == nullptr)
		return;

	Texture2DDesc dstDesc;
	ID3D11Texture2D* readTexture = dstDesc.CopyResourceTexture(heightMapFile->GetTexture());

	UINT pixelCount = dstDesc.Desc().Width * dstDesc.Desc().Height;
	UINT* pixels = new UINT[pixelCount];

	dstDesc.MapCopyFromOutput(pixels, sizeof(UINT) * pixelCount);

	UINT* heights = new UINT[pixelCount];
	for (UINT i = 0; i < pixelCount; i++)
	{
		// 0xAABBGGRR -> 0xAAXXXXXX
		UINT pixel = pixels[i];
		UINT result = 0;

		Channel channel = (Channel)selectedChannel;

		switch (channel)
		{
		case Editor::Channel::Alpha: result = (pixel & 0xFF000000) >> 24; break;
		case Editor::Channel::Blue:  result = (pixel & 0x00FF0000) >> 16; break;
		case Editor::Channel::Green: result = (pixel & 0x0000FF00) >> 8;  break;
		case Editor::Channel::Red:   result = (pixel & 0x000000FF) >> 0;  break;
		}

		heights[i] = result << 24;
	}

	Texture2DDesc saveDesc;
	D3D11_TEXTURE2D_DESC& desc = saveDesc.Desc();
	desc.Width = dstDesc.Desc().Width;
	desc.Height = dstDesc.Desc().Height;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.MipLevels = 1;
	desc.SampleDesc = dstDesc.Desc().SampleDesc;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	saveDesc.SetColors(heights);

	ID3D11Texture2D* saveTexture = saveDesc.CreateTexture(sizeof(UINT));

	wstring fileName = Path::GetFileNameWithoutExtension(heightMapFile->GetFile());
	fileName = URI::Textures + heightMapFileDirectory + fileName + L".dds";
	D3DX11SaveTextureToFile(D3D::GetDC(), saveTexture, D3DX11_IFF_DDS, fileName.c_str());

	//SafeRelease(srcTexture);
	SafeRelease(readTexture);
	SafeRelease(saveTexture);

	SafeDeleteArray(heights);
	SafeDeleteArray(pixels);
}

void Editor::UpdateDataMapFileList()
{
	dataMapFileList.clear();
	Path::GetFiles(&dataMapFileList, URI::Textures + heightMapFileDirectory, L"*.dds", false);

	for (wstring& file : dataMapFileList)
		file = Path::GetFileNameWithoutExtension(file);


}
