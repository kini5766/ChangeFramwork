#include "stdafx.h"
#include "ColliderLoader.h"

#include "Utilities/BinaryFile.h"

ColliderLoader::ColliderLoader()
{
}

ColliderLoader::~ColliderLoader()
{
}

void ColliderLoader::Load(ColliderBox * c, BinaryReader * r)
{
	Transform* t = c->GetTransform();
	t->Position(r->Vector3());
	t->Rotation(Quaternion((float*)r->Vector4()));
	t->Scale(r->Vector3());

	c->SetLayer(r->UInt());
}
