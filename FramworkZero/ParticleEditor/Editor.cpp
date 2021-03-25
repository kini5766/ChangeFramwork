#include "stdafx.h"
#include "Editor.h"

#include "Utilities/Xml.h"

void Editor::Initialize()
{
	Context::Get()->MainCamera()->GetTransform()->RotationEuler(EulerAngle(20, 0, 0));
	Context::Get()->MainCamera()->GetTransform()->Position(1, 36, -85);

	sky = new CubeSky(L"Environment/SunsetCube1024.dds");

	Meshes();
	UpdateParticleList();
	UpdateTextureList();
}

void Editor::Destroy()
{
	SafeDelete(particle);
	SafeDelete(sky);
	SafeDelete(sphere);
	SafeDelete(grid);

	SafeRelease(shaderMesh);
}

void Editor::Update()
{
	ImGui::SliderFloat3("LightDirection", Lighting::Get()->DirectionalDesc()->Direction, -1, 1);

	OnGui();

	sky->Update();
	grid->Update();

	Vector3 p;
	sphere->GetTransform()->Position(&p);
	if (Input::Mouse()->Press(1) == false)
	{
		Vector3 F = Context::Get()->MainCamera()->GetTransform()->Forward();
		Vector3 R = Context::Get()->MainCamera()->GetTransform()->Right();
		Vector3 U = Context::Get()->MainCamera()->GetTransform()->Up();

		if (Input::Keyboard()->Press('W'))
			p += Vector3(F.x, 0.0f, F.z) * 20.0f * Time::Delta();
		if (Input::Keyboard()->Press('S'))
			p -= Vector3(F.x, 0.0f, F.z) * 20.0f * Time::Delta();

		if (Input::Keyboard()->Press('A'))
			p -= Vector3(R.x, 0.0f, R.z) * 20.0f * Time::Delta();
		if (Input::Keyboard()->Press('D'))
			p += Vector3(R.x, 0.0f, R.z) * 20.0f * Time::Delta();
	
		if (Input::Keyboard()->Press('E'))
			p += U * 20.0f * Time::Delta();
		if (Input::Keyboard()->Press('Q'))
			p -= U * 20.0f * Time::Delta();
	}
	sphere->GetTransform()->Position(p);
	sphere->Update();

	if (particle != nullptr)
	{
		particle->Emit(p);
		particle->Update();
	}
}

void Editor::Render()
{
	sky->Render();

	sphere->Render();
	grid->Render();

	if (particle != nullptr)
		particle->Render();
}

void Editor::Meshes()
{
	shaderMesh = Shader::Load(L"01_Mesh.fxo");
	grid = new Mesh(shaderMesh, MeshPlane(15, 15));
	grid->GetTransform()->Position(0, 0, 0);
	grid->GetTransform()->Scale(20, 1, 20);
	grid->GetMaterial()->DiffuseMap("Floor.png");
	//grid->GetMaterial()->NormalMap("Floor_Normal.png");
	grid->GetMaterial()->NormalMap("Environment/Wave.dds");
	grid->GetMaterial()->SpecularMap("Floor_Specular.png");
	grid->GetMaterial()->Specular(1, 1, 1, 20);

	sphere = new Mesh(shaderMesh, MeshSphere(0.5f, 20, 20));
	sphere->GetTransform()->Position(0, 5, 0);
	sphere->GetTransform()->Scale(5, 5, 5);
	sphere->GetMaterial()->DiffuseMap("Bricks.png");
	sphere->GetMaterial()->NormalMap("Bricks_Normal.png");
	sphere->GetMaterial()->SpecularMap("Bricks_Specular.png");
	sphere->GetMaterial()->Specular(1, 1, 1, 20);
	sphere->GetMaterial()->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

	meshes.push_back(grid);
	meshes.push_back(sphere);
}

void Editor::Pass(UINT pass)
{
	for (Mesh* mesh : meshes)
		mesh->Pass(pass);
}

void Editor::UpdateParticleList()
{
	particleList.clear();
	Path::GetFiles(&particleList, URI::Particles, L"*.xml", false);

	for (wstring& file : particleList)
		file = Path::GetFileNameWithoutExtension(file);
}

void Editor::UpdateTextureList()
{
	vector<wstring> files;
	Path::GetFiles(&files, URI::Particles, L"*.*", false);

	textureList.clear();
	for (wstring& file : files)
	{
		wstring ext = Path::GetExtension(file);
		transform(ext.begin(), ext.end(), ext.begin(), toupper);  // toupper �빮�ڷ� �ٲ���
		file = Path::GetFileName(file);
		if (ext == L"PNG" || ext == L"TGA" || ext == L"JPG") // �ʿ信 ���� �߰�
			textureList.push_back(file);
	}
}

void Editor::OnGui()
{
	float width = Screen::Width();
	float height = Screen::Height();

	bool bOpen = true;
	bOpen = ImGui::Begin("Particle", &bOpen);
	ImGui::SetWindowPos(ImVec2(width - windowWidth, 0));
	ImGui::SetWindowSize(ImVec2(windowWidth, height));
	{
		OnGui_List();
		OnGui_Settings();
	}
	ImGui::End();
}

void Editor::OnGui_List()
{
	if (ImGui::CollapsingHeader("Particle List", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (UINT i = 0; i < particleList.size(); i++)
		{
			// 0 : ��Ʈ ũ�⿡ ���缭
			if (ImGui::Button(String::ToString(particleList[i]).c_str(), ImVec2(200, 0)))
			{
				SafeDelete(particle);

				file = particleList[i];
				particle = new Particle(file);

				bLoop = particle->GetData().IsLoop;
				maxParticle = particle->GetData().MaxParticles;
			}
		}
	}
}

void Editor::OnGui_Settings()
{
	if (particle == nullptr) return;

	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Particle Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();

		ImGui::SliderInt("MaxParticles", (int *)&maxParticle, 1, 1000);
		ImGui::Checkbox("Loop", &bLoop);

		if (ImGui::Button("Apply"))
		{
			particle->GetData().IsLoop = bLoop;
			particle->GetData().MaxParticles = maxParticle;
			particle->Reset();
		}


		ImGui::Separator();

		const char* types[] = { "Opaque", "Additive", "AlphaBlend" };
		ImGui::Combo("BlendType", (int *)&particle->GetData().Type, types, 3);

		ImGui::SliderFloat("ReadyTime", &particle->GetData().ReadyTime, 0.1f, 10.0f);
		ImGui::SliderFloat("ReadyRandomTime", &particle->GetData().ReadyRandomTime, 0.0f, 100.0f);

		ImGui::SliderFloat("StartVelocity", &particle->GetData().StartVelocity, 0.0f, 10.0f);
		ImGui::SliderFloat("EndVelocity", &particle->GetData().EndVelocity, -100.0f, 100.0f);

		ImGui::SliderFloat("MinHorizontalVelocity", &particle->GetData().MinHorizontalVelocity, -100.0f, 100.0f);
		ImGui::SliderFloat("MaxHorizontalVelocity", &particle->GetData().MaxHorizontalVelocity, -100.0f, 100.0f);

		ImGui::SliderFloat("MinVerticalVelocity", &particle->GetData().MinVerticalVelocity, -100.0f, 100.0f);
		ImGui::SliderFloat("MaxVerticalVelocity", &particle->GetData().MaxVerticalVelocity, -100.0f, 100.0f);

		ImGui::SliderFloat3("Gravity", particle->GetData().Gravity, -100, 100);

		ImGui::ColorEdit4("MinColor", particle->GetData().MinColor);
		ImGui::ColorEdit4("MaxColor", particle->GetData().MaxColor);

		ImGui::SliderFloat("MinRotateSpeed", &particle->GetData().MinRotateSpeed, -10, 10);
		ImGui::SliderFloat("MaxRotateSpeed", &particle->GetData().MaxRotateSpeed, -10, 10);

		ImGui::SliderInt("MinStartSize", &particle->GetData().MinStartSize, 0, 500);
		ImGui::SliderInt("MaxStartSize", &particle->GetData().MaxStartSize, 0, 500);

		ImGui::SliderInt("MinEndSize", &particle->GetData().MinEndSize, 0, 500);
		ImGui::SliderInt("MaxEndSize", &particle->GetData().MaxEndSize, 0, 500);

		// �ణ ����߷���
		ImGui::Spacing();
		OnGui_Write();
		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Texture List", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const wstring& textureFile : textureList)
			{
				if (ImGui::Button(String::ToString(textureFile).c_str(), ImVec2()))
				{
					particle->GetData().TextureFile = textureFile;
					particle->SetTexture(L"Particles/" + textureFile);
				}
			}
		}
	}
}

void Editor::OnGui_Write()
{
	ImGui::Separator();
	if (ImGui::Button("Save"))
	{
		Path::SaveFileDialog(file,
			L"Particle File\0*.xml", URI::Particles,
			bind(&Editor::WriteFile, this, placeholders::_1),
			WinDesc::GetHandle()
		);
	}
}

void Editor::WriteFile(wstring file)
{
	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Particle");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);


	Xml::XMLElement* node = NULL;

	node = document->NewElement("BlendState");
	node->SetText((int)particle->GetData().Type);
	root->LinkEndChild(node);


	string textureFile = String::ToString(particle->GetData().TextureFile);
	String::Replace(&textureFile, "Particles/", "");

	node = document->NewElement("Loop");
	node->SetText(particle->GetData().IsLoop);
	root->LinkEndChild(node);

	node = document->NewElement("TextureFile");
	node->SetText(textureFile.c_str());
	root->LinkEndChild(node);


	node = document->NewElement("MaxParticles");
	node->SetText(particle->GetData().MaxParticles);
	root->LinkEndChild(node);


	node = document->NewElement("ReadyTime");
	node->SetText(particle->GetData().ReadyTime);
	root->LinkEndChild(node);

	node = document->NewElement("ReadyRandomTime");
	node->SetText(particle->GetData().ReadyRandomTime);
	root->LinkEndChild(node);

	node = document->NewElement("StartVelocity");
	node->SetText(particle->GetData().StartVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("EndVelocity");
	node->SetText(particle->GetData().EndVelocity);
	root->LinkEndChild(node);


	node = document->NewElement("MinHorizontalVelocity");
	node->SetText(particle->GetData().MinHorizontalVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("MaxHorizontalVelocity");
	node->SetText(particle->GetData().MaxHorizontalVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("MinVerticalVelocity");
	node->SetText(particle->GetData().MinVerticalVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("MaxVerticalVelocity");
	node->SetText(particle->GetData().MaxVerticalVelocity);
	root->LinkEndChild(node);


	node = document->NewElement("Gravity");
	node->SetAttribute("X", particle->GetData().Gravity.x);
	node->SetAttribute("Y", particle->GetData().Gravity.y);
	node->SetAttribute("Z", particle->GetData().Gravity.z);
	root->LinkEndChild(node);


	node = document->NewElement("MinColor");
	node->SetAttribute("R", particle->GetData().MinColor.r);
	node->SetAttribute("G", particle->GetData().MinColor.g);
	node->SetAttribute("B", particle->GetData().MinColor.b);
	node->SetAttribute("A", particle->GetData().MinColor.a);
	root->LinkEndChild(node);

	node = document->NewElement("MaxColor");
	node->SetAttribute("R", particle->GetData().MaxColor.r);
	node->SetAttribute("G", particle->GetData().MaxColor.g);
	node->SetAttribute("B", particle->GetData().MaxColor.b);
	node->SetAttribute("A", particle->GetData().MaxColor.a);
	root->LinkEndChild(node);


	node = document->NewElement("MinRotateSpeed");
	node->SetText(particle->GetData().MinRotateSpeed);
	root->LinkEndChild(node);

	node = document->NewElement("MaxRotateSpeed");
	node->SetText(particle->GetData().MaxRotateSpeed);
	root->LinkEndChild(node);

	node = document->NewElement("MinStartSize");
	node->SetText((int)particle->GetData().MinStartSize);
	root->LinkEndChild(node);

	node = document->NewElement("MaxStartSize");
	node->SetText((int)particle->GetData().MaxStartSize);
	root->LinkEndChild(node);

	node = document->NewElement("MinEndSize");
	node->SetText((int)particle->GetData().MinEndSize);
	root->LinkEndChild(node);

	node = document->NewElement("MaxEndSize");
	node->SetText((int)particle->GetData().MaxEndSize);
	root->LinkEndChild(node);

	wstring folder = Path::GetDirectoryName(file);
	wstring fileName = Path::GetFileNameWithoutExtension(file);

	document->SaveFile(String::ToString(folder + fileName + L".xml").c_str());
	SafeDelete(document);

	UpdateParticleList();
}
