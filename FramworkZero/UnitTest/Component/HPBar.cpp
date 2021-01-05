#include "stdafx.h"
#include "HPBar.h"

HPBar::HPBar()
{
	green = new Texture(L"Green.png");
	red = new Texture(L"Red.png");

	hpbarG = new Render2D();
	hpbarR = new Render2D();

	hpbarPos = new Transform();

	hpbarG->GetTransform()->Position(0.0f, 25.0f, 0.0f);
	hpbarR->GetTransform()->Position(0.0f, 25.0f, 0.0f);
	hpbarG->GetTransform()->Scale(barXScale, 15.0f, 1.0f);
	hpbarR->GetTransform()->Scale(0.0f, 15.0f, 1.0f);

	hpbarG->SRV(green->SRV());
	hpbarR->SRV(red->SRV());
}

HPBar::~HPBar()
{
	SafeDelete(hpbarPos);

	SafeDelete(hpbarR);
	SafeDelete(hpbarG);

	SafeDelete(red);
	SafeDelete(green);
}

void HPBar::Update(float hp, float hpMax)
{
	float hpPer = hp / hpMax;

	Matrix W, V, P;
	hpbarPos->LossyWorld(&W);
	V = Context::Get()->View();
	P = Context::Get()->Projection();
	Vector3 pPos = Context::Get()->GetViewport()->
		Project(Vector3(0, 0, 0), W, V, P);

	hpbarG->GetTransform()->Scale(hpPer * barXScale, 15.0f, 1.0f);
	hpbarR->GetTransform()->Scale((1 - hpPer) * barXScale, 15.0f, 1.0f);
	Vector3 pPosR = pPos;
	pPosR.x -= (1 - hpPer) * barXScale * 0.5f;
	hpbarG->GetTransform()->Position(pPosR);
	Vector3 pPosG = pPos;
	pPosG.x += hpPer * barXScale * 0.5f;
	hpbarR->GetTransform()->Position(pPosG);
	hpbarG->Update();
	hpbarR->Update();

	Vector3 pPosT = pPos;
	pPosT.x -= barXScale * 0.35f;
	pPosT.y -= 5.0f;
	string str = string("HP : ") + to_string((int)hp) + "/" + to_string((int)hpMax);
	Gui::Get()->RenderText(pPosT.x, pPosT.y, 1, 1, 1, str);
}

void HPBar::Render()
{
	hpbarG->Render();
	hpbarR->Render();
}
