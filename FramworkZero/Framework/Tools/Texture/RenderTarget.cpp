#include "Framework.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(float width, float height, DXGI_FORMAT format)
	: format(format)
{
	this->width = (width > 0.0f) ? width : (UINT)Screen::Width();
	this->height = (height > 0.0f) ? height : (UINT)Screen::Height();

	// texture
	Texture2DDesc tex2DDesc;
	D3D11_TEXTURE2D_DESC& textureDesc = tex2DDesc.Desc();
	textureDesc.Width = (UINT)this->width;
	textureDesc.Height = (UINT)this->height;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = format;
	textureDesc.BindFlags =  // 2개를 연결 했으니 2개다 쓸 수 있음
		D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	texture = tex2DDesc.CreateBlankTexture();

	// Render target
	rtv = tex2DDesc.CreateRTV();

	// SRV
	srv = tex2DDesc.CreateSRV();
}

RenderTarget::~RenderTarget()
{
	SafeRelease(texture);
	SafeRelease(rtv);
	SafeRelease(srv);
}

void RenderTarget::PreRender(DepthStencil * depthStencil)
{
	D3D::GetDC()->OMSetRenderTargets(1, &rtv, depthStencil->DSV());
	D3D::Get()->Clear(Color(0, 0, 0, 1), rtv, depthStencil->DSV());
}

void RenderTarget::SaveTextureFile(wstring file)
{
	D3DX11SaveTextureToFile(D3D::GetDC(), texture, D3DX11_IFF_PNG, file.c_str());
}
