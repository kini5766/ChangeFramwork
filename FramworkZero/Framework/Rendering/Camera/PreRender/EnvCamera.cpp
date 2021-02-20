#include "Framework.h"
#include "EnvCamera.h"

EnvCamera::EnvCamera(float width, float height)
{
	DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Create Texture 2D (RTV)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.ArraySize = 6;  // 텍스쳐는 1개 지만 면이 6개
		desc.Format = rtvFormat;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE
			| D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &rtvTexture));
	}

	// Create RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D::GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv));
	}

	// Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = 1;

		Check(D3D::GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &srv));
	}

	DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT;
	// Create Texture 2D (DSV)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.ArraySize = 6;
		desc.Format = dsvFormat;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &dsvTexture));
	}

	// Create DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		desc.Format = dsvFormat;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D::GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv));
	}

	transform = new Transform();
	viewport = new Viewport((float)width, (float)height);
	projection = new Perspective(width, height);
}

EnvCamera::~EnvCamera()
{
	SafeDelete(projection);
	SafeDelete(viewport);
	SafeDelete(transform);

	SafeRelease(rtvTexture);
	SafeRelease(srv);
	SafeRelease(rtv);
	SafeRelease(dsvTexture);
	SafeRelease(dsv);
}


#pragma region ICamera

void EnvCamera::GetView(Matrix * out)
{
	//Context::Get()->MainCamera()->GetView(out);
	(*out) = forword;
}

void EnvCamera::GetProjection(Matrix * out)
{
	//Context::Get()->MainCamera()->GetProjection(out);
	projection->GetMatrix(out);
}

void EnvCamera::GetPlanes_Side4(Plane * planes)
{
	ZeroMemory(planes, sizeof(Plane) * 4);
}

#pragma endregion


void EnvCamera::PreRender()
{
	D3D::Get()->SetRenderTarget(rtv, dsv);
	D3D::Get()->Clear(Color(0, 0, 0, 1), rtv, dsv);
	viewport->RSSetViewport();

	Context::Get()->PreRender(this);
}

void EnvCamera::GetEnvProjection(Matrix * out)
{
	projection->GetMatrix(out);
}

void EnvCamera::GetEnvViews_Side6(Matrix * out)
{
	WorldMatrix world;
	transform->UpdateWorld();
	world.GetWorld_Global(transform);

	Vector3 position = world.Position();
	Vector3 scale = world.LossyScale();

	float x = position.x;
	float y = position.y;
	float z = position.z;

	struct LookAt
	{
		Vector3 At;
		Vector3 Up;
	} lookAt[6];
	lookAt[0] = { Vector3(x + scale.x, y, z), Vector3(0, 1, 0) };  // R
	lookAt[1] = { Vector3(x - scale.x, y, z), Vector3(0, 1, 0) };  // L
	lookAt[2] = { Vector3(x, y + scale.y, z), Vector3(0, 0,-1) };  // U
	lookAt[3] = { Vector3(x, y - scale.y, z), Vector3(0, 0, 1) };  // D
	lookAt[4] = { Vector3(x, y, z + scale.z), Vector3(0, 1, 0) };  // F
	lookAt[5] = { Vector3(x, y, z - scale.z), Vector3(0, 1, 0) };  // B

	for (UINT i = 0; i < 6; i++)
		D3DXMatrixLookAtLH(&out[i], &position, &lookAt[i].At, &lookAt[i].Up);


	Vector3 f = Context::Get()->MainCamera()->GetTransform()->Forward();
	Vector3 u = Context::Get()->MainCamera()->GetTransform()->Up();
	D3DXMatrixLookAtLH(&forword, &position, &(position + f), &u);
}

void EnvCamera::SetProjection(float width, float height, float zNear, float zFar, float fov)
{
	((Perspective*)projection)->Set(width, height, zNear, zFar, fov);
}
