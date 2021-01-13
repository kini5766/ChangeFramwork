#include "Framework.h"

CsResource::CsResource()
{
}

CsResource::~CsResource()
{
	SafeRelease(input);
	SafeRelease(srv);
	SafeRelease(output);
	SafeRelease(uav);
}

void CsResource::CreateBuffer()
{
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
}