#pragma once
#include "Systems/IExecute.h"

class EditorDemo : public IExecute
{
public:
	// IExecute��(��) ���� ��ӵ�
	void Initialize() override;
	void Ready() override {}
	void Destroy() override;
	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}
	void ResizeScreen() override {}

private:
	Shader* shaderModel;
	struct SceneValue* value;
	class SceneValueEditor* editor;

};
