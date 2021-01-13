#pragma once

class Gizmo
{
public:
	Gizmo();
	~Gizmo();

public:
	void Update();
	void Render();

public:
	void SetTransform(class Transform* transform);

private:
	class Transform* transform = nullptr;
	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE mode = ImGuizmo::WORLD;
};