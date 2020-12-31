#include "stdafx.h"
#include "MainCameraDebug.h"

MainCameraDebug::MainCameraDebug()
{
}

MainCameraDebug::~MainCameraDebug()
{
}

void MainCameraDebug::Render()
{
	Camera* camera = Context::Get()->GetCamera();

	Vector3 P;
	camera->Position(&P);
	ImGui::DragFloat3("Position", P);
	camera->Position(P);

	Vector3 R;
	Quaternion q;
	camera->Rotation(&q);
	{
		float xy2 = 2 * q.x * q.y;
		float xz2 = 2 * q.x * q.z;
		float yz2 = 2 * q.y * q.z;

		float xx2 = 2 * q.x * q.x;
		float yy2 = 2 * q.y * q.y;
		float zz2 = 2 * q.z * q.z;

		float xw2 = 2 * q.x * q.w;
		float yw2 = 2 * q.y * q.w;
		float zw2 = 2 * q.z * q.w;

		R.x = atan2(xw2 - yz2, 1 - xx2 - zz2);
		R.y = atan2(yw2 - xz2, 1 - yy2 - zz2);
		R.z = asin(xy2 + zw2);
	}
	R *= Math::Rad2Deg;

	Vector3 CurrR = R;
	ImGui::DragFloat3("Rotation", CurrR, 1.0f);
	//camera->Rotation(CurrR);
	Vector3 DistR = CurrR - R;
	camera->RotatePitchDegree(DistR.x);
	camera->RotateYawDegree(DistR.y);
	camera->RotateRollDegree(DistR.z);
}
