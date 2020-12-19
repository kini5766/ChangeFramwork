#include "Framework.h"

TextureBuffer::TextureBuffer(ID3D11Texture2D * src)
	: CsResource()
{
	// 텍스쳐 정보 불러오기
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	// 동일한 크기와 포멧으로 복사하기
	width = srcDesc.Width;
	height = srcDesc.Height;
	arraySize = srcDesc.ArraySize;
	format = srcDesc.Format;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	ID3D11Texture2D* texture = nullptr;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));
	D3D::GetDC()->CopyResource(texture, src);

	input = (ID3D11Texture2D*)texture;

	CreateBuffer();

	((ID3D11Texture2D*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &result));
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::CreateSRV()
{
	ID3D11Texture2D* texture = (ID3D11Texture2D*)input;

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = arraySize;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}

void TextureBuffer::CreateOutput()
{
	ID3D11Texture2D* texture = nullptr;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));

	output = (ID3D11Texture2D*)texture;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = arraySize;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &outputSRV));
}

void TextureBuffer::CreateUAV()
{
	ID3D11Texture2D* texture = (ID3D11Texture2D*)output;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = arraySize;

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
