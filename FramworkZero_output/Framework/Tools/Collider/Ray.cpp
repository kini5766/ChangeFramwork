#include "Framework.h"
#include "Ray.h"

Raycast::Raycast(const RaycastOutput * output, UINT layer)
	: output(output), layer(layer)
{
	bools[0] = false;
	bools[1] = true;
	bools[2] = false;
}

Raycast::~Raycast()
{
}
