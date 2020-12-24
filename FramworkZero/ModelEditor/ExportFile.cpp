#include "stdafx.h"
#include "ExportFile.h"

#include "Converter.h"

void ExportFile::Initialize()
{
	//Tank();
	//Kachujin();
	//Tower();
	//Airplane();

	//Atienza();
}

void ExportFile::Tank()
{
	Converter conv;

	conv.ReadFile(L"Tank/Tank.fbx");
	conv.ExportMesh(L"Tank/Tank");
	conv.ExportMaterial(L"Tank/Tank", false);
}

void ExportFile::Kachujin()
{
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Mesh.fbx");
		conv.ExportMesh(L"Kachujin/Mesh");
		conv.ExportMaterial(L"Kachujin/Mesh");
	}

	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Idle.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Idle");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Walk.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Walk");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Run.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Run");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Slash.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Slash");
	}
	{
		Converter conv;
		conv.ReadFile(L"Kachujin/Uprock.fbx");
		conv.ExportAnimClip(0, L"Kachujin/Uprock");
	}
}

void ExportFile::Tower()
{
	Converter conv;

	conv.ReadFile(L"Tower/Tower.fbx");
	conv.ExportMesh(L"Tower/Tower");
	conv.ExportMaterial(L"Tower/Tower");
}

void ExportFile::Airplane()
{
	Converter conv;

	conv.ReadFile(L"B787/Airplane.fbx");
	conv.ExportMesh(L"B787/Airplane");
	conv.ExportMaterial(L"B787/Airplane");
}

void ExportFile::Atienza()
{
	{
		Converter conv;
		conv.ReadFile(L"Atienza/Mesh.fbx");
		conv.ExportMesh(L"Atienza/Mesh");
		conv.ExportMaterial(L"Atienza/Mesh");
	}

	{
		Converter conv;
		conv.ReadFile(L"Atienza/Idle.fbx");
		conv.ExportAnimClip(0, L"Atienza/Idle");
	}
	{
		Converter conv;
		conv.ReadFile(L"Atienza/Walk.fbx");
		conv.ExportAnimClip(0, L"Atienza/Walk");
	}
	{
		Converter conv;
		conv.ReadFile(L"Atienza/Run.fbx");
		conv.ExportAnimClip(0, L"Atienza/Run");
	}
	{
		Converter conv;
		conv.ReadFile(L"Atienza/Kick.fbx");
		conv.ExportAnimClip(0, L"Atienza/Kick");
	}
}
