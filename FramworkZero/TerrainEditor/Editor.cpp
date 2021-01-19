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
	//shadow = new Shadow(shader, Vector3(128, 0, 128), 65);

	sky = new CubeSky(L"Environment/Mountain1024.dds");

	funcOpen = bind(&Editor::OpenComplete, this, placeholders::_1);
	UpdateDataMapFileList();

	peojTexure = new ProjectionTexture(Shader::Load(URI::Shaders + L"01_Terrain.fxo"), L"Environment/MagicCircle.png", 217.0f, 220.0f);


	shadow = new Shadow(Vector3(0, 0, 0), 65.0f);
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
					SafeDelete(brush);

					wstring path = heightMapFileDirectory + dataMapFileList[i] + L".dds";
					terrain = new Terrain(path);
					terrain->BaseMap(L"Terrain/Dirt3.png");
					brush = new Brush(terrain);
				}
			}

		}
	}

	//sky->Update();
	if (terrain != nullptr)
	{
		terrain->RenderVisibleNormal();
		terrain->Update();
	}
}

void Editor::PreRender()
{
	//shadow->PreRender();
	if (terrain != nullptr)
		terrain->PreRender_Depth();
}

void Editor::Render()
{
	//sky->Render();

	peojTexure->Render();

	if (terrain != nullptr)
		terrain->Render();
}

void Editor::OpenComplete(wstring fileName)
{
	heightMapFileName = Path::GetFileName(fileName);
}

void Editor::ToDataMapFile()
{
	if (heightMapFile == nullptr)
		return;

	ID3D11Texture2D* srcTexture = heightMapFile->GetTexture();
	D3D11_TEXTURE2D_DESC srcDesc;
	srcTexture->GetDesc(&srcDesc);

	ID3D11Texture2D* readTexture;
	D3D11_TEXTURE2D_DESC readDesc;
	ZeroMemory(&readDesc, sizeof(D3D11_TEXTURE2D_DESC));
	readDesc.Width = srcDesc.Width;
	readDesc.Height = srcDesc.Height;
	readDesc.ArraySize = 1;
	readDesc.Format = srcDesc.Format;
	readDesc.MipLevels = 1;
	readDesc.SampleDesc = srcDesc.SampleDesc;
	readDesc.Usage = D3D11_USAGE_STAGING;
	readDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Check(D3D::GetDevice()->CreateTexture2D(&readDesc, nullptr, &readTexture));
	D3D::GetDC()->CopyResource(readTexture, srcTexture);

	UINT* pixels = new UINT[readDesc.Width * readDesc.Height];
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(readTexture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		// UINT 16진수로 들어옴 (0xAABBGGRR) ARGB순서 꼭 확인
		memcpy(pixels, subResource.pData, sizeof(UINT) * readDesc.Width * readDesc.Height);
	}
	D3D::GetDC()->Unmap(readTexture, 0);

	UINT* heights = new UINT[readDesc.Width * readDesc.Height];
	for (UINT i = 0; i < readDesc.Width * readDesc.Height; i++)
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

	ID3D11Texture2D* saveTexture;
	D3D11_TEXTURE2D_DESC saveDesc;
	ZeroMemory(&saveDesc, sizeof(D3D11_TEXTURE2D_DESC));
	saveDesc.Width = readDesc.Width;
	saveDesc.Height = readDesc.Height;
	saveDesc.ArraySize = 1;
	saveDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	saveDesc.MipLevels = 1;
	saveDesc.SampleDesc = readDesc.SampleDesc;
	saveDesc.Usage = D3D11_USAGE_STAGING;
	saveDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA saveSubResource;
	saveSubResource.pSysMem = heights;
	saveSubResource.SysMemPitch = sizeof(UINT) * readDesc.Width;
	saveSubResource.SysMemSlicePitch = sizeof(UINT) * readDesc.Width * readDesc.Height;

	Check(D3D::GetDevice()->CreateTexture2D(&saveDesc, &saveSubResource, &saveTexture));

	wstring fileName = Path::GetFileNameWithoutExtension(heightMapFile->GetFile());
	fileName = URI::Textures + heightMapFileDirectory + fileName + L".dds";
	D3DX11SaveTextureToFile(D3D::GetDC(), saveTexture, D3DX11_IFF_DDS, fileName.c_str());

	SafeRelease(srcTexture);
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
