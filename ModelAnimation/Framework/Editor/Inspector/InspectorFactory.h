#pragma once

#include "Objects/InspectorContext.h"
#include "Objects/InspectorModel.h"
#include "Objects/InspectorAnimator.h"
#include "Objects/InspectorCube.h"
#include "Objects/InspectorTerrain.h"

class InspectorFactory
{
private:
	// ���� �Լ� ������
	void Insert()
	{
		creator.push_back(make_pair("DirectionLight", 
			[]()->IInspectorEditor* { return new InspectorLight(); } ));
		creator.push_back(make_pair("MainCamera", 
			[]()->IInspectorEditor* { return new InspectorMainCamera(); } ));
		creator.push_back(make_pair("Model", 
			[]()->IInspectorEditor* { return new InspectorModel(shaderModel); } ));
		creator.push_back(make_pair("AnimationModel",
			[]()->IInspectorEditor* { return new InspectorAnimator(shaderModel); }));
		creator.push_back(make_pair("Cube", 
			[]()->IInspectorEditor* { return new InspectorCube(shaderMesh); } ));
		creator.push_back(make_pair("Terrain",
			[]()->IInspectorEditor* { return new InspectorTerrain(shaderTerrain); }));
	}

private:
	// ������ �μ���
	static Shader* shaderModel;
	static Shader* shaderMesh;
	static Shader* shaderTerrain;

private:
	// �μ��� ����
	static void CreateArgs();
	// �μ��� ����
	static void DeleteArgs();


public:
	InspectorFactory();
	~InspectorFactory();

public:
	string* GetNames(UINT* pSize);
	class IInspectorEditor* CreateEditor(string typeName);


private:
	// ������ ���� ��
	vector<pair<string, IInspectorEditor*(*)(void)>> creator;
	string* names;
	UINT size;
};