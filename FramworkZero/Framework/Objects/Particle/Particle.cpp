#include "Framework.h"
#include "Particle.h"
#include "Utilities/Xml.h"

Particle::Particle(wstring xmlFile)
{
	Shader* shader = Shader::Load(L"01_Particle.fxo");
	material = new Material(shader);
	perTransform = new PerTransform(shader);
	
	ReadFile(URI::Particles + xmlFile + L".xml");

	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	material->SetConstantBuffer("CB_Particle", buffer->Buffer());

	material->SetSRV("ParticleMap", map->SRV());
	Reset();
}

Particle::~Particle()
{
	SafeDelete(map);
	SafeDelete(buffer);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);
	mesh.SafeDeleteData();

	SafeDelete(perTransform);
	material->GetShader()->Release();
	SafeDelete(material);
}

void Particle::Update()
{
	perTransform->Update();

	currentTime += Time::Delta();

	MapVertices();
	Activation();
	Deactivation();

	// 면단위 계산
	if (activeCount == leadCount)
		currentTime = 0.0f;

	desc.MinColor = data.MinColor;
	desc.MaxColor = data.MaxColor;

	desc.Gravity = data.Gravity;
	desc.EndVelocity = data.EndVelocity;

	desc.StartSize = Vector2((float)data.MinStartSize, (float)data.MaxStartSize);
	desc.EndSize = Vector2((float)data.MinEndSize, (float)data.MaxEndSize);
	desc.RotateSpeed = Vector2(data.MinRotateSpeed, data.MaxRotateSpeed);

	desc.ReadyTime = data.ReadyTime;
	desc.ReadyRandomTime = data.ReadyRandomTime;
}

void Particle::Render()
{
	desc.CurrentTime = currentTime;
	buffer->Render();

	indexBuffer->Render();
	vertexBuffer->Render();
	perTransform->Render();
	material->Render();

	UINT pass = (UINT)data.Type;
	if (activeCount != gpuCount)
	{
		if (gpuCount > activeCount)
		{
			// activeCount ~ gpuCount
			material->GetShader()->DrawIndexed(0, pass, (gpuCount - activeCount) * 6, activeCount * 6);
		}
		else
		{
			// 0 ~ gpuCount, activeCount ~ Max
			material->GetShader()->DrawIndexed(0, pass, (data.MaxParticles - activeCount) * 6, activeCount * 6);

			if (gpuCount > 0)
				material->GetShader()->DrawIndexed(0, pass, gpuCount * 6);
		}
	}
}

void Particle::Reset()
{
	currentTime = 0.0f;
	lastAddTime = Time::Get()->Running();

	leadCount = 0;
	gpuCount = 0;
	activeCount = 0;
	deactiveCount = 0;

	mesh.SafeDeleteData();
	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	VertexParticle* v  = mesh.NewVertices<VertexParticle>(data.MaxParticles * 4);

	for (UINT i = 0; i < data.MaxParticles; i++)
	{
		v[i * 4 + 0].Corner = Vector2(-1, -1);
		v[i * 4 + 1].Corner = Vector2(-1, +1);
		v[i * 4 + 2].Corner = Vector2(+1, -1);
		v[i * 4 + 3].Corner = Vector2(+1, +1);
	}

	mesh.NewIndices(data.MaxParticles * 6);
	for (UINT i = 0; i < data.MaxParticles; i++)
	{
		mesh.Indices[i * 6 + 0] = i * 4 + 0;
		mesh.Indices[i * 6 + 1] = i * 4 + 1;
		mesh.Indices[i * 6 + 2] = i * 4 + 2;
		mesh.Indices[i * 6 + 3] = i * 4 + 2;
		mesh.Indices[i * 6 + 4] = i * 4 + 1;
		mesh.Indices[i * 6 + 5] = i * 4 + 3;
	}

	vertexBuffer = new VertexBuffer(mesh.Vertices, mesh.VertexCount, mesh.Stride, 0, true);
	indexBuffer = new IndexBuffer(mesh.Indices, mesh.IndexCount);
}

void Particle::Emit(const Vector3& position)
{
	// 1000.0f (단위 ms -> s)
	if (Time::Get()->Running() - lastAddTime < 60 / 1000.0f)
		return;
	lastAddTime = Time::Get()->Running();

	UINT next = leadCount + 1;
	if (next >= data.MaxParticles)
	{
		if (data.IsLoop)
			next = 0;
		//else next = data.MaxParticles;
		else return;
	}

	if (next == deactiveCount)
		return;

	Vector3 velocity = Vector3(1, 1, 1);
	velocity *= data.StartVelocity;

	VertexParticle* v = (VertexParticle*) mesh.Vertices;

	float horizontalVelocity = data.MinHorizontalVelocity +
		(data.MaxHorizontalVelocity - data.MinHorizontalVelocity) *
		Math::Random(0.0f, 1.0f);
	float horizontalAngle = Math::PI * 2 * Math::Random(0.0f, 1.0f);

	velocity.x += horizontalVelocity * cos(horizontalAngle);
	velocity.y += horizontalVelocity * sin(horizontalAngle);
	velocity.z += data.MinVerticalVelocity + 
		(data.MaxVerticalVelocity - data.MinVerticalVelocity) * 
		Math::Random(0.0f, 1.0f);

	Vector4 random = (float*)Math::RandomColor4();

	for (UINT i = 0; i < 4; i++)
	{
		v[leadCount * 4 + i].Position = position;
		v[leadCount * 4 + i].Velocity = velocity;
		v[leadCount * 4 + i].Random = random;
		v[leadCount * 4 + i].Time = currentTime;
	}

	leadCount = next;
}

void Particle::SetTexture(wstring file) 
{
	SafeDelete(map);
	map = new Texture(file);
	material->SetSRV("ParticleMap", map->SRV());
}


void Particle::MapVertices()
{
	// 0. 복사할 데이터 없다
	if (leadCount == gpuCount)
		return;

	D3D11_MAPPED_SUBRESOURCE subResource;
	// 1. 역전 되지 않음
	if (leadCount > gpuCount)
	{
		// NO_OVERWRITE : 전부 덮어쓰지 않을 것 
		D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);
		
		UINT start = gpuCount * 4 * mesh.Stride;
		UINT size = (leadCount - gpuCount) * 4 * mesh.Stride;
		UINT offset = gpuCount * 4 * mesh.Stride;

		BYTE* p = (BYTE*)subResource.pData + offset;
		memcpy(p, (BYTE*)mesh.Vertices + start, size);
		D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
	}
	// 2. 역전 됨
	{
		D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);

		UINT start = gpuCount * 4 * mesh.Stride;
		UINT size = (data.MaxParticles - gpuCount) * 4 * mesh.Stride;
		UINT offset = gpuCount * 4 * mesh.Stride;

		BYTE* p = (BYTE*)subResource.pData + offset;
		memcpy(p, (BYTE*)mesh.Vertices + start, size);

		if (leadCount > 0)
		{
			size = (leadCount) * 4 * mesh.Stride;
			memcpy(subResource.pData, mesh.Vertices, size);
		}

		D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
	}

	gpuCount = leadCount;
}

void Particle::Activation()
{
	// 쿨타임 마다 파티클 생성
	while (activeCount != gpuCount)
	{
		VertexParticle* v = (VertexParticle*)mesh.Vertices;
		float age = currentTime - v[activeCount * 4].Time;  // 현재 시각 - 마지막 생성시각
		
		if (age < data.ReadyTime)
			return;

		v[activeCount * 4].Time = currentTime;
		
		++activeCount;
		if (activeCount >= data.MaxParticles)
			activeCount = (data.IsLoop) ? 0 : data.MaxParticles;
	}
}

void Particle::Deactivation()
{
	while (deactiveCount != activeCount)
	{
		VertexParticle* v = (VertexParticle*)mesh.Vertices;
		float age = currentTime - v[activeCount * 4].Time;  // 현재 시각 - 마지막 생성시각

		if (age > data.ReadyTime)
			return;

		++deactiveCount;
		if (deactiveCount >= data.MaxParticles)
			deactiveCount = (data.IsLoop) ? 0 : data.MaxParticles;
	}
}

void Particle::ReadFile(wstring xmlFile)
{
	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLError error = document->LoadFile(String::ToString(xmlFile).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();

	Xml::XMLElement* node = root->FirstChildElement();
	data.Type = (ParticleData::BlendType)node->IntText();

	node = node->NextSiblingElement();
	data.IsLoop = node->BoolText();

	node = node->NextSiblingElement();
	wstring textureFile = String::ToWString(node->GetText());
	data.TextureFile = L"Particles/" + textureFile;
	map = new Texture(data.TextureFile);


	node = node->NextSiblingElement();
	data.MaxParticles = node->IntText();

	node = node->NextSiblingElement();
	data.ReadyTime = node->FloatText();

	node = node->NextSiblingElement();
	data.ReadyRandomTime = node->FloatText();

	node = node->NextSiblingElement();
	data.StartVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.EndVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MinHorizontalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxHorizontalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MinVerticalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxVerticalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.Gravity.x = node->FloatAttribute("X");
	data.Gravity.y = node->FloatAttribute("Y");
	data.Gravity.z = node->FloatAttribute("Z");

	node = node->NextSiblingElement();
	data.MinColor.r = node->FloatAttribute("R");
	data.MinColor.g = node->FloatAttribute("G");
	data.MinColor.b = node->FloatAttribute("B");
	data.MinColor.a = node->FloatAttribute("A");

	node = node->NextSiblingElement();
	data.MaxColor.r = node->FloatAttribute("R");
	data.MaxColor.g = node->FloatAttribute("G");
	data.MaxColor.b = node->FloatAttribute("B");
	data.MaxColor.a = node->FloatAttribute("A");

	node = node->NextSiblingElement();
	data.MinRotateSpeed = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxRotateSpeed = node->FloatText();

	node = node->NextSiblingElement();
	data.MinStartSize = (int)node->FloatText();

	node = node->NextSiblingElement();
	data.MaxStartSize = (int)node->FloatText();

	node = node->NextSiblingElement();
	data.MinEndSize = (int)node->FloatText();

	node = node->NextSiblingElement();
	data.MaxEndSize = (int)node->FloatText();

	SafeDelete(document);
}
