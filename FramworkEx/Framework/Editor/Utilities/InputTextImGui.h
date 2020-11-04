#pragma once

class InputTextImGui
{
public:
	InputTextImGui(UINT maxSize);
	~InputTextImGui();

public:
	void Render(const char* label);

	void Text(const char* value);
	char* Text() { return text; }
	UINT MaxSize() { return maxSize; }

private:
	char* text;
	UINT maxSize;
};