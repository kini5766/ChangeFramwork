#include "Framework.h"
#include "Fixity.h"

Fixity::Fixity()
	: Camera(unique_ptr<Projection>(new Orthographic(Screen::Width(), Screen::Height())))
{
}

Fixity::~Fixity()
{
}
