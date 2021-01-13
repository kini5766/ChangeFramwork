#pragma once

class CsResource
{
public:
	CsResource();
	virtual ~CsResource();

protected:
	virtual void CreateInput() {}
	virtual void CreateSRV() {}

	virtual void CreateOutput() {}
	virtual void CreateUAV() {}

public:
	ID3D11ShaderResourceView* SRV() { return srv; }
	ID3D11UnorderedAccessView* UAV() { return uav; }

protected:
	void CreateBuffer();

protected:
	ID3D11Resource* input = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	ID3D11Resource* output = nullptr;
	ID3D11UnorderedAccessView* uav = nullptr;
};