#include "Framework.h"
#include "Fixity.h"

Fixity::Fixity()
{
	orthographic = new Orthographic(Screen::Width(), Screen::Height());
	projection = orthographic;

	Rotation();
	Move();
}

Fixity::~Fixity()
{
}

void Fixity::Update()
{
}

void Fixity::ResizeScreen(float width, float height)
{
	viewport->Set(width, height);
	orthographic->Set(width, height);
}
