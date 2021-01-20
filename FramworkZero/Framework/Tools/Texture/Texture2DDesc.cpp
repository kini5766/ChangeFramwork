#include "Framework.h"
#include "Texture2DDesc.h"

Texture2DDesc::Texture2DDesc(UINT arraySize)
{
	datas.assign(arraySize, nullptr);
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.ArraySize = arraySize;
}

Texture2DDesc::Texture2DDesc(Texture2DDesc * copyDesc)
{
	memcpy(&desc, &copyDesc->desc, sizeof(D3D11_TEXTURE2D_DESC));
	datas.assign(copyDesc->desc.ArraySize, nullptr);
}

Texture2DDesc::Texture2DDesc(ID3D11Texture2D * texture)
	: texture(texture)
{
	texture->GetDesc(&desc);
	datas.assign(desc.ArraySize, nullptr);
}

Texture2DDesc::Texture2DDesc(UINT width, UINT height, bool bCpuWirte, bool bGpuWrite, UINT arraySize)
{
	datas.assign(arraySize, nullptr);
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = arraySize;

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

Texture2DDesc::~Texture2DDesc()
{
}


#pragma region Geter Setter

void Texture2DDesc::SetArraySize(UINT size)
{
	datas.assign(size, nullptr);
	desc.ArraySize = size;
}

void Texture2DDesc::SetTexture(ID3D11Texture2D * value)
{
	texture = value;

	value->GetDesc(&desc);
	SetArraySize(desc.ArraySize);
}

#pragma endregion


#pragma region Texture2D

void Texture2DDesc::SetColors(const void * data)
{
	datas[0] = data;
}

ID3D11Texture2D * Texture2DDesc::CreateBlankTexture()
{
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));
	return texture;
}

ID3D11Texture2D* Texture2DDesc::CreateTexture(UINT stride)
{
	// 1줄의 바이트 크기
	UINT widthSize = desc.Width * stride;
	// 1장의 바이트 크기
	UINT pageSize = widthSize * desc.Height;

	D3D11_SUBRESOURCE_DATA subResouce{ 0 };
	subResouce.pSysMem = datas[0];
	subResouce.SysMemPitch = widthSize;  // 1줄의 바이트 크기
	subResouce.SysMemSlicePitch = pageSize;  // 1장의 바이트 크기
	Check(D3D::GetDevice()->CreateTexture2D(&desc, &subResouce, &texture));
	return texture;
}

ID3D11ShaderResourceView* Texture2DDesc::CreateSRV()
{
	ID3D11ShaderResourceView* srv;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));

	return srv;
}

ID3D11RenderTargetView * Texture2DDesc::CreateRTV()
{
	ID3D11RenderTargetView* rtv;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Check(D3D::GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv));

	return rtv;
}

#pragma endregion


#pragma region Texture2DArray

void Texture2DDesc::SetColorsArray(UINT arrayIndex, const void * data)
{
	datas[arrayIndex] = data;
}

ID3D11Texture2D* Texture2DDesc::CreateTextureArray(UINT stride)
{
	// 1줄의 바이트 크기
	UINT widthSize = desc.Width * stride;
	// 1장의 바이트 크기
	UINT pageSize = widthSize * desc.Height;

	D3D11_SUBRESOURCE_DATA* subResouce = new D3D11_SUBRESOURCE_DATA[desc.ArraySize];
	for (UINT i = 0; i < desc.ArraySize; i++)
	{
		subResouce[i].pSysMem = datas[i];
		subResouce[i].SysMemPitch = widthSize;  // 1줄의 바이트 크기
		subResouce[i].SysMemSlicePitch = pageSize;  // 1장의 바이트 크기
	}

	Check(D3D::GetDevice()->CreateTexture2D(&desc, subResouce, &texture));
	SafeDeleteArray(subResouce);

	return texture;
}

ID3D11ShaderResourceView* Texture2DDesc::CreateSRVArray()
{
	ID3D11ShaderResourceView* srv;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
	srvDesc.Texture2DArray.ArraySize = desc.ArraySize;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));

	return srv;
}

#pragma endregion


#pragma region Texture

ID3D11Texture2D * Texture2DDesc::CopyResourceTexture(ID3D11Texture2D * value)
{
	Texture2DDesc srcDesc(value);

	desc.Width = srcDesc.Desc().Width;
	desc.Height = srcDesc.Desc().Height;
	desc.ArraySize = 1;
	desc.Format = srcDesc.Desc().Format;
	desc.MipLevels = 1;
	desc.SampleDesc = srcDesc.Desc().SampleDesc;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	texture = CreateBlankTexture();
	D3D::GetDC()->CopyResource(texture, value);

	return texture;
}

void Texture2DDesc::MapCopyFromOutput(void * out, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(out, subResource.pData, size);
	}
	D3D::GetDC()->Unmap(texture, 0);
}

void Texture2DDesc::MapCopyToInput(const void * data, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE, 0, &subResource);
	{
		// UINT 16진수로 들어옴 (0xAABBGGRR) ARGB순서 꼭 확인
		memcpy(subResource.pData, data, size);
	}
	D3D::GetDC()->Unmap(texture, 0);
}

#pragma endregion


