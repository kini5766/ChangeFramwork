#include "Framework.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(float width, float height, bool bUseStencil)
{
	this->width = (width < 1) ? Screen::Width() : width;
	this->height = (height < 1) ? Screen::Height() : height;

	// texture
	Texture2DDesc tex2DDesc;
	D3D11_TEXTURE2D_DESC& textureDesc = tex2DDesc.Desc();
	textureDesc.Width = (UINT)this->width;
	textureDesc.Height = (UINT)this->height;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = (bUseStencil) ?  // Depth + Stencil 합해서 32비트
		DXGI_FORMAT_R24G8_TYPELESS :
		DXGI_FORMAT_R32_TYPELESS;
	textureDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |
		D3D11_BIND_SHADER_RESOURCE;
	texture = tex2DDesc.CreateBlankTexture();

	// DSV
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = (bUseStencil) ?  // 위랑 다름
		DXGI_FORMAT_D24_UNORM_S8_UINT :
		DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	Check(D3D::GetDevice()->CreateDepthStencilView(texture, &dsvDesc, &dsv));


	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = (bUseStencil) ?
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS : // X : 사용 안 함 (스텐실은 그림으로 표현 안 됨)
		DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));

}

DepthStencil::~DepthStencil()
{
	SafeRelease(texture);
	SafeRelease(dsv);
	SafeRelease(srv);
}
