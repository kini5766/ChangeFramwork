#include "Framework.h"
#include "InputTextImGui.h"

InputTextImGui::InputTextImGui(UINT maxSize)
	: maxSize(maxSize)
{
	text = new char[maxSize];
	ZeroMemory(text, sizeof(char) * maxSize);
}

InputTextImGui::~InputTextImGui()
{
	SafeDelete(text);
}

void InputTextImGui::Render(const char * label)
{
	ImGui::PushID(this); // �ٸ� ���� ���� ���̵��� ���� ����� ������
	ImGui::InputText(label, text, maxSize);
	ImGui::PopID();
}

void InputTextImGui::Text(const char * value)
{
	ZeroMemory(text, sizeof(char) * maxSize);
	for (UINT i = 0; i < maxSize; i++)
	{
		if (value[i] == '\0')
			break;

		text[i] = value[i];
	}
}
