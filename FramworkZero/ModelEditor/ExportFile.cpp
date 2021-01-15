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

	//Paladin();
	//Friedrich();
	//Swat();
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
	Converter conv;
	conv.ReadFile(L"Kachujin/Mesh.fbx");
	conv.ExportMesh(L"Kachujin/Mesh");
	conv.ExportMaterial(L"Kachujin/Mesh");

	conv.ReadFile(L"Kachujin/Idle.fbx");
	conv.ExportAnimClip(0, L"Kachujin/Idle");

	conv.ReadFile(L"Kachujin/Walk.fbx");
	conv.ExportAnimClip(0, L"Kachujin/Walk");

	conv.ReadFile(L"Kachujin/Run.fbx");
	conv.ExportAnimClip(0, L"Kachujin/Run");

	conv.ReadFile(L"Kachujin/Slash.fbx");
	conv.ExportAnimClip(0, L"Kachujin/Slash");

	conv.ReadFile(L"Kachujin/Fall.fbx");
	conv.ExportAnimClip(0, L"Kachujin/Fall");
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

void ExportFile::Paladin()
{
	{
		Converter conv;
		conv.ReadFile(L"Paladin/Mesh.fbx");
		conv.ExportMesh(L"Paladin/Mesh");
		conv.ExportMaterial(L"Paladin/Mesh");
	}

	{
		Converter conv;
		conv.ReadFile(L"Paladin/Idle.fbx");
		conv.ExportAnimClip(0, L"Paladin/Idle");
	}
	{
		Converter conv;
		conv.ReadFile(L"Paladin/Walk.fbx");
		conv.ExportAnimClip(0, L"Paladin/Walk");
	}
	{
		Converter conv;
		conv.ReadFile(L"Paladin/Run.fbx");
		conv.ExportAnimClip(0, L"Paladin/Run");
	}
	{
		Converter conv;
		conv.ReadFile(L"Paladin/Taunt.fbx");
		conv.ExportAnimClip(0, L"Paladin/Taunt");
	}
	{
		Converter conv;
		conv.ReadFile(L"Paladin/Attack.fbx");
		conv.ExportAnimClip(0, L"Paladin/Attack");
	}
	{
		Converter conv;
		conv.ReadFile(L"Paladin/React.fbx");
		conv.ExportAnimClip(0, L"Paladin/React");
	}
	{
		Converter conv;
		conv.ReadFile(L"Paladin/Fall.fbx");
		conv.ExportAnimClip(0, L"Paladin/Fall");
	}
}

void ExportFile::Friedrich()
{
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Mesh.fbx");
		conv.ExportMesh(L"Friedrich/Mesh");
		conv.ExportMaterial(L"Friedrich/Mesh");
	}

	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Idle.fbx");
		conv.ExportAnimClip(0, L"Friedrich/Idle");
	}
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Walk.fbx");
		conv.ExportAnimClip(0, L"Friedrich/Walk");
	}
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Run.fbx");
		conv.ExportAnimClip(0, L"Friedrich/Run");
	}
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Taunt.fbx");
		conv.ExportAnimClip(0, L"Friedrich/Taunt");
	}
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Attack.fbx");
		conv.ExportAnimClip(0, L"Friedrich/Attack");
	}
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/React.fbx");
		conv.ExportAnimClip(0, L"Friedrich/React");
	}
	{
		Converter conv;
		conv.ReadFile(L"Friedrich/Fall.fbx");
		conv.ExportAnimClip(0, L"Friedrich/Fall");
	}
}

void ExportFile::Swat()
{
	{
		Converter conv;
		conv.ReadFile(L"Swat/Mesh.fbx");
		conv.ExportMesh(L"Swat/Mesh");
		conv.ExportMaterial(L"Swat/Mesh");

		conv.ReadFile(L"Swat/Idle.fbx");
		conv.ExportAnimClip(0, L"Swat/Idle");

		conv.ReadFile(L"Swat/Walk.fbx");
		conv.ExportAnimClip(0, L"Swat/Walk");

		conv.ReadFile(L"Swat/Run.fbx");
		conv.ExportAnimClip(0, L"Swat/Run");

		conv.ReadFile(L"Swat/Slash.fbx");
		conv.ExportAnimClip(0, L"Swat/Slash");
	}
}
