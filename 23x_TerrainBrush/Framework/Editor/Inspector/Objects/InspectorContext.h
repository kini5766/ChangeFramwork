#pragma once

#include "../IInspectorEditor.h"

class InspectorLight : public IInspectorEditor
{
public:
	InspectorLight();
	~InspectorLight();

public:
	// IInspectorEditor��(��) ���� ��ӵ�
	void Render() override;
	void Save(Xml::XMLDocument* document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;
};



class InspectorMainCamera : public IInspectorEditor
{
public:
	InspectorMainCamera();
	~InspectorMainCamera();

public:
	// IInspectorEditor��(��) ���� ��ӵ�
	void Render() override;
	void Save(Xml::XMLDocument* document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;
};