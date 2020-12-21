#include "Framework.h"
#include "Texture2D.h"

Texture2D::Texture2D(UINT width, UINT height, bool bCpuWirte, bool bGpuWrite, UINT arraySize)
	: arraySize(arraySize)
{
	datas = new const void*[arraySize];

	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	if (bCpuWirte == false && bGpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
	}
	else if (bCpuWirte == true && bGpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (bCpuWirte == false && bGpuWrite == true)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
	}
	else
	{
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
}

Texture2D::~Texture2D()
{
	SafeDeleteArray(datas);
}

void Texture2D::SetColors(const void * data)
{
	datas[0] = data;
}

void Texture2D::CreateTexture()
{
	// 1줄의 바이트 크기
	UINT widthSize = desc.Width * stride;
	// 1장의 바이트 크기
	UINT pageSize = widthSize * desc.Height;

	desc.ArraySize = 1;

	D3D11_SUBRESOURCE_DATA* subResouce = nullptr;
	if (datas[0] != nullptr)
	{
		subResouce = new D3D11_SUBRESOURCE_DATA{ 0 };
		subResouce->pSysMem = datas[0];
		subResouce->SysMemPitch = widthSize;  // 1줄의 바이트 크기
		subResouce->SysMemSlicePitch = pageSize;  // 1장의 바이트 크기
	}

	Check(D3D::GetDevice()->CreateTexture2D(&desc, subResouce, &texture));
	SafeDelete(subResouce);
	desc.ArraySize = arraySize;
}

void Texture2D::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}

void Texture2D::SetColorsArray(const void ** datas)
{
	memcpy(this->datas, datas, sizeof(const void*) * arraySize);
}

void Texture2D::CreateTextureArray()
{
	// 1줄의 바이트 크기
	UINT widthSize = desc.Width * stride;
	// 1장의 바이트 크기
	UINT pageSize = widthSize * desc.Height;

	D3D11_SUBRESOURCE_DATA* subResouce = new D3D11_SUBRESOURCE_DATA[arraySize];
	for (UINT i = 0; i < arraySize; i++)
	{
		subResouce[i].pSysMem = datas[i];
		subResouce[i].SysMemPitch = widthSize;  // 1줄의 바이트 크기
		subResouce[i].SysMemSlicePitch = pageSize;  // 1장의 바이트 크기
	}

	Check(D3D::GetDevice()->CreateTexture2D(&desc, subResouce, &texture));
	SafeDeleteArray(subResouce);
}

void Texture2D::CreateArraySRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = arraySize;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}
