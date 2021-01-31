#include "stdafx.h"
#include "TransformLoader.h"

#include "Utilities/BinaryFile.h"

TransformLoader::TransformLoader()
{
}

TransformLoader::~TransformLoader()
{
}

void TransformLoader::Load(Transform * t, BinaryReader * r)
{
	t->Position(r->Vector3());
	t->Rotation(Quaternion((float*)r->Vector4()));
	t->Scale(r->Vector3());
}
