#include "Framework.h"
#include "ProjectionPointer.h"

ProjectionPointer::ProjectionPointer(Projection * pointer)
	: value(pointer)
{
}

ProjectionPointer::~ProjectionPointer()
{
}

void ProjectionPointer::GetMatrix(Matrix * matrix)
{
	if (value == nullptr)
		Context::Get()->MainCamera()->GetProjection()->GetMatrix(matrix);
	else value->GetMatrix(matrix);
}
