#include "Framework.h"
#include "MeshInstancing.h"

MeshInstancing::MeshInstancing(unique_ptr<MeshData> shape)
{
	shader = Shader::Load(L"01_Mesh.fxo");
	shadowCaster = Context::Get()->AddShadowCaster({ shader,
		bind(&MeshInstancing::PreRender_Depth, this) }
	);
	envCubeCaster = Context::Get()->AddEnvCubeCaster({ shader,
		bind(&MeshInstancing::PreRender_EnvCube, this) }
	);


	material = new Material(shader);
	perframe = new PerFrame();
	renderer = new MeshInstRenderer(move(shape));


	perframe->SetAtMaterial(material);
	renderer->GetRenderer()->SetMaterial(material);
}

MeshInstancing::~MeshInstancing()
{
	SafeDelete(renderer);
	SafeDelete(perframe);
	SafeDelete(material);

	SafeRelease(envCubeCaster);
	SafeRelease(shadowCaster);
	SafeRelease(shader);
}

void MeshInstancing::Update()
{
	perframe->Update();
}

void MeshInstancing::UpdateTransforms()
{
	renderer->UpdateTransforms();
}

void MeshInstancing::Render()
{
	perframe->Render();

	renderer->Pass(5);
	renderer->Render();
}

void MeshInstancing::Render_CubeMap()
{
	perframe->Render();

	renderer->Pass(8);
	renderer->Render();
}

void MeshInstancing::Render_EnvCube()
{
	perframe->Render();

	renderer->Pass(12);
	renderer->Render();
}


MeshInstance * MeshInstancing::AddInstance()
{
	return renderer->AddInstance();
}


void MeshInstancing::PreRender_Depth()
{
	perframe->Render();

	renderer->Pass(4);
	renderer->Render();
}

void MeshInstancing::PreRender_EnvCube()
{
	perframe->Render();

	renderer->Pass(10);
	renderer->Render();
}
