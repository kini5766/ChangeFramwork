#pragma once

class DepthStencil
{
public:
	DepthStencil(float width = 0, float height = 0, bool bUseStencil = false);
	~DepthStencil();

public:
	ID3D11Texture2D* Texture() const { return texture; }
	ID3D11DepthStencilView* DSV() const { return dsv; }
	ID3D11ShaderResourceView* SRV() const { return srv; }

private:
	float width, height;
	//DXGI_FORMAT format;

	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* srv;
};