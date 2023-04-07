#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CPlayer.h"
#include "CGMap.h"
#include "CMenu.h"

namespace game_framework {

	void CMenu::Initialize()
	{
	}

	void CMenu::Initialize(int num)
	{
		number = num;
		flag = 0;
		isSpace = isMovingUp = isMovingDown = isShow = isLeft = false;
		way = 1;
	}

	void CMenu::LoadBitmap()
	{
		starts.LoadBitmap(IDB_START);				// 开始菜单
		help.LoadBitmap(IDB_HELP);					// 帮助菜单
		about.LoadBitmap(IDB_ABOUT);				// 关于菜单													   
		mouse.LoadBitmap(IDB_MOUSE);				// 光标
		about_background.LoadBitmap(IDB_ABOUTUS);	// 关于的图片
		player_background.LoadBitmap(IDB_HOWTOPLAY);	// 如何游戏的图片
	}

	void CMenu::OnShow()
	{
		starts.SetTopLeft(250, 310);
		starts.ShowBitmap();
		help.SetTopLeft(250, 370);
		help.ShowBitmap();
		about.SetTopLeft(250, 430);
		about.ShowBitmap();
		mouse.SetTopLeft(x, y);
		mouse.ShowBitmap();
		if (number == 2 && isSpace && !isLeft)
		{
			about_background.ShowBitmap();
		}
		if (number == 1 && isSpace && !isLeft)
		{
			player_background.ShowBitmap();
		}
	}

	void CMenu::setNumber(int number)
	{
		number = number;
	}

	void CMenu::setIsShow(bool flag)
	{
		isShow = flag;
	}

	void CMenu::setMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CMenu::setMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CMenu::setIsLeft(bool flag)
	{
		isLeft = flag;
		isSpace = false;
		way = 1;
	}

	void CMenu::setIsSpace(bool flag)
	{
		isSpace = flag;
		way = 2;
	}

	void CMenu::OnMove()
	{
		const int STEP_SIZE = 1;
		if (isMovingUp)
		{
			if (number > 0 && flag == 0)
			{
				number -= STEP_SIZE;
				flag = 1;
			}
		}
		if (isMovingDown && flag == 0)
		{
			if (number < 2)
			{
				number += STEP_SIZE;
				flag = 1;
			}
		}
		if (number == 0)
		{
			x = 150;
			y = 310;

		}
		else if (number == 1)
		{
			x = 150;
			y = 370;
		}
		else if (number == 2)
		{
			x = 150;
			y = 430;
		}
	}

	int CMenu::getStatusNum()
	{
		return number;
	}

	void CMenu::setFlag(bool Flag)
	{
		flag = Flag;
	}

	int CMenu::getWay()
	{
		return way;
	}
}