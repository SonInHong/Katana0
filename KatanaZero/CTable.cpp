#include "pch.h"
#include "CTable.h"
#include "CTexture.h"

CTable::CTable()
{
	Texture = new CTexture;
	Texture->Load(L"BackgroundObj\\Table\\table.bmp");

	Scale.x = (double)Texture->GetWidth();
	Scale.y = (double)Texture->GetHeight();
}

CTable::~CTable()
{
}
