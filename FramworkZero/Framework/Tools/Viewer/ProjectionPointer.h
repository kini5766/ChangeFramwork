#pragma once

class ProjectionPointer : public Projection
{
public:
	ProjectionPointer(Projection* pointer = nullptr);
	~ProjectionPointer();

	void GetMatrix(Matrix* matrix) override;

private:
	Projection* value = nullptr;
};
