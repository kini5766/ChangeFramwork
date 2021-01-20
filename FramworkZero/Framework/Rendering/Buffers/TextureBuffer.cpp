#include "Framework.h"

TextureBuffer::TextureBuffer(ID3D11Texture2D * src)
	: CsResource()
{
	// 텍스쳐 정보 불러오기
	srcDesc = new Texture2DDesc(src);
	srcDesc->Desc().Usage = D3D11_USAGE_DEFAULT;
	//desc.MipLevels = 1;
	//desc.SampleDesc.Count = 1;

	Texture2DDesc tex2DDesc(srcDesc);
	tex2DDesc.Desc().BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* texture = tex2DDesc.CreateBlankTexture();
	D3D::GetDC()->CopyResource(texture, src);

	input = (ID3D11Resource*)texture;

	CreateBuffer();

	tex2DDesc = Texture2DDesc((ID3D11Texture2D*)output);
	D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	result = tex2DDesc.CreateBlankTexture();
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &result));
}

TextureBuffer::~TextureBuffer()
{
	SafeDelete(srcDesc);
}

void TextureBuffer::CreateSRV()
{
	srv = Texture2DDesc((ID3D11Texture2D*)input).CreateSRVArray();
}

void TextureBuffer::CreateOutput()
{
	Texture2DDesc tex2DDesc(srcDesc);
	tex2DDesc.Desc().BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tex2DDesc.Desc().CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	output = (ID3D11Resource*)tex2DDesc.CreateBlankTexture();

	outputSRV = tex2DDesc.CreateSRVArray();
}

void TextureBuffer::CreateUAV()
{
	ID3D11Texture2D* texture = (ID3D11Texture2D*)output;
	Texture2DDesc tex2DDesc(texture);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = tex2DDesc.Desc().ArraySize;

	Check(D3D::GetDevice()->CreateUnorderedAccessView(texture, &uavDesc, &uav));
}

void TextureBuffer::CopyToInput(ID3D11Texture2D * texture)
{
	D3D::GetDC()->CopyResource(input, texture);
}

ID3D11Texture2D * TextureBuffer::CopyFromOutput()
{
	D3D::GetDC()->CopyResource(result, output);
	return result;
}
