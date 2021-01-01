#include "Framework.h"
#include "Ray.h"

Raycast::Raycast(const RaycastOutput * output)
	: output(output)
{
	bools[0] = false;
	bools[1] = true;
	bools[2] = false;
}

Raycast::~Raycast()
{
}
