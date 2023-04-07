#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CESWall.h"
#include "CGMap.h"
namespace game_framework {

	CESWall::CESWall()
	{
		Initialize();
	}

	void CESWall::LoadBitmap()
	{
		eswall_live.AddBitmap(IDB_ESWALL);
		eswall_dead.AddBitmap(IDB_ESWALL1, RGB(183, 185, 184));
		eswall_dead.AddBitmap(IDB_ESWALL2, RGB(183, 185, 184));
		eswall_dead.AddBitmap(IDB_ESWALL3, RGB(183, 185, 184));
		eswall_dead.AddBitmap(IDB_ESWALL4, RGB(183, 185, 184));
		eswall_dead.AddBitmap(IDB_ESWALL5, RGB(183, 185, 184));
	}

	void CESWall::Initialize()
	{
		x = y = 0;
		isAlive = true;
	}

	void CESWall::OnShow(float sx, float sy)
	{
		if (isAlive)
		{
			eswall_live.SetTopLeft(x - sx, y - sy);
			eswall_live.OnMove();
			eswall_live.OnShow();
		}
		else if (isAlive == false) {
			if (eswall_dead.IsFinalBitmap())
			{

			}
			else {
				eswall_dead.SetTopLeft(x - sx, y - sy);
				eswall_dead.OnMove();
				eswall_dead.OnShow();
			}
		}
	}

	void CESWall::SetXY(float nx, float ny)
	{
		x = nx;
		y = ny;
	}

	void CESWall::SetIsAlive(bool alive)
	{
		isAlive = alive;
	}

	bool CESWall::GetIsAlive()
	{
		return isAlive;
	}

	float CESWall::GetX1()
	{
		return x;
	}

	float CESWall::GetY1()
	{
		return y;
	}

	float CESWall::GetX2()
	{
		return x + eswall_live.Width();
	}

	float CESWall::GetY2()
	{
		return y + eswall_live.Height();
	}

	void CESWall::ResetAnimation()
	{
		eswall_live.Reset();
		eswall_dead.Reset();
	}

	CESWall::~CESWall()
	{
	}
}