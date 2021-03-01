#include "stdafx.h"
#include "PlaingOnce.h"

PlaingOnce::PlaingOnce(const PlaingOnceDesc & desc)
	: desc(desc)
{
	funcComple = bind(&PlaingOnce::CompleAnim, this, placeholders::_1);
}

PlaingOnce::~PlaingOnce()
{
}


void PlaingOnce::Call(const DelayReturn * _result)
{
	result = _result;

	desc.Anim->Play(desc.Clip);
	desc.Anim->SetFuncNext(funcComple);
}

void PlaingOnce::Update()
{
}

void PlaingOnce::Cancel()
{
	desc.Anim->SetFuncNext([](UINT) {});
	result = nullptr;
}

void PlaingOnce::CompleAnim(UINT i)
{
	desc.Anim->SetFuncNext([](UINT) {});
	(*result)();
	result = nullptr;
}
