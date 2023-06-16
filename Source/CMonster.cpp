#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CGMap.h"
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>
namespace game_framework {

	CMonster::CMonster()
	{
		Initialize();
	}

	CMonster::CMonster(int num)
	{
		stochastic = num;
		Initialize();
	}

	bool CMonster::HitEraser(CPlayer *eraser)
	{
		// 檢測擦子所構成的矩形是否碰到球
		return HitRectangle(eraser->GetX1(), eraser->GetY1(),
			eraser->GetX2(), eraser->GetY2());
	}


	bool CMonster::HitRectangle(float tx1, float ty1, float tx2, float ty2) //tx1左上角x坐标、ty1左上角y坐标。
	{
		float x1 = x ;				// 球的左上角x座標
		float y1 = y ;				// 球的左上角y座標 
		float x2 = x1 + 28;	// 球的右下角x座標
		float y2 = y1 + 28;	// 球的右下角y座標									
									// 檢測球的矩形與參數矩形是否有交集				 				
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	} 


	int CMonster::GetRandomNum()
	{
		return (rand() % 4) + 1;
	}

	void CMonster::SetMovingDown(bool flag)
	{
		isMoveDown = flag;
	}

	void CMonster::SetMovingLeft(bool flag)
	{
		isMoveLeft = flag;
	}

	void CMonster::SetMovingRight(bool flag)
	{
		isMoveRight = flag;
	}

	void CMonster::SetMovingUp(bool flag)
	{
		isMoveUp = flag;
	}

	void CMonster::SetRandomNum(int num)
	{
		randomNum = num;
	}

	void CMonster::ResetAnimation()
	{
		monsteralive.Reset();
		monsterdead.Reset();
	}

	void CMonster::SetIsTurn(bool flag)
	{
		isTurn = flag;
	}

	void CMonster::SetDirection()
	{
		const int UP = 1;
		const int LEFT = 2;
		const int RIGHT = 3;
		const int DOWN = 4;
		if (randomNum == UP)
		{
			isMoveUp = true;
			isMoveRight = false;
			isMoveLeft = false;
			isMoveDown = false;

		}
		else if (randomNum == LEFT)
		{
			isMoveUp = false;
			isMoveRight = false;
			isMoveLeft = true;
			isMoveDown = false;
		}
		else if (randomNum == RIGHT)
		{
			isMoveUp = false;
			isMoveRight = true;
			isMoveLeft = false;
			isMoveDown = false;
		}
		else if (randomNum == DOWN)
		{
			isMoveUp = false;
			isMoveRight = false;
			isMoveLeft = false;
			isMoveDown = true;
		}
	}

	void CMonster::InitializeMonsterForNext()
	{
		ResetAnimation();
		Initialize();
	}

	bool CMonster::IsRight(CGMap * gamemap, CPlayer * player)
	{
		if (stochastic % 3 == 0 && stochastic != 0) {
			return !(gamemap->HitRectangleOfMonster(GetX1() + 8, GetY1() + 3, GetX2() + 4, GetY2())
				|| player->HitRectangleOfBomb(GetX1() + 8, GetY1() + 3, GetX2() + 4, GetY2()));
		}
		else {
			return !(gamemap->HitRectangle(GetX1() + 8, GetY1() + 3, GetX2() + 4, GetY2())
				|| player->HitRectangleOfBomb(GetX1() + 8, GetY1() + 3, GetX2() + 4, GetY2()));
		}

	}

	bool CMonster::IsUp(CGMap * gamemap, CPlayer * player)
	{
		if (stochastic % 3 == 0 && stochastic != 0) {
			return !(gamemap->HitRectangleOfMonster(GetX1() + 3, GetY1() - 1, GetX2() - 2, GetY2() - 3)
				|| player->HitRectangleOfBomb(GetX1() + 3, GetY1() - 2, GetX2() - 2, GetY2() - 3));
		}
		else {
			return !(gamemap->HitRectangle(GetX1() + 3, GetY1() - 1, GetX2() - 2, GetY2() - 3)
				|| player->HitRectangleOfBomb(GetX1() + 3, GetY1() - 2, GetX2() - 2, GetY2() - 3));
		}
	}

	bool CMonster::IsDown(CGMap * gamemap, CPlayer * player)
	{
		if (stochastic % 3 == 0 && stochastic != 0) {
			return !(gamemap->HitRectangleOfMonster(GetX1() + 3, GetY1() + 3, GetX2() - 2, GetY2() + 4)
				|| player->HitRectangleOfBomb(GetX1() + 3, GetY1() + 3, GetX2() - 2, GetY2() + 3));
		}
		else {
			return !(gamemap->HitRectangle(GetX1() + 3, GetY1() + 3, GetX2() - 2, GetY2() + 4)
				|| player->HitRectangleOfBomb(GetX1() + 3, GetY1() + 3, GetX2() - 2, GetY2() + 3));
		}
	}

	bool CMonster::IsLeft(CGMap * gamemap, CPlayer * player)
	{
		if (stochastic % 3 == 0 && stochastic != 0) {
			return !(gamemap->HitRectangleOfMonster(GetX1(), GetY1() + 3, GetX2() - 5, GetY2())
				|| player->HitRectangleOfBomb(GetX1(), GetY1() + 3, GetX2() - 5, GetY2()));
		}
		else {
			return !(gamemap->HitRectangle(GetX1(), GetY1() + 3, GetX2() - 5, GetY2())
				|| player->HitRectangleOfBomb(GetX1(), GetY1() + 3, GetX2() - 5, GetY2()));
		}
		
	}

	void CMonster::changestochastic(int num)
	{
		stochastic =  num;
	}

	int CMonster::GetStochastic()
	{
		return stochastic;
	}

	bool CMonster::IsAlive()
	{
		return is_alive;
	}

	void CMonster::Initialize()
	{
		is_alive = true;
		x = y = 0;
		start = time(NULL);
		randomNum = 2;

		if (stochastic % 3 == 0)		// 共10只怪物
			SPEED = 1.2;				// 3强力怪
		else if(stochastic % 5 == 0)
			SPEED = 2;					// 2超强力怪
		else
			SPEED = 1;					// 5普通怪

		isMoveUp = false;
		isMoveRight = false;
		isMoveLeft = false;
		isMoveDown = false;
		isTurn = false;
		SetDirection();
	}

	float CMonster::GetX1()
	{
		return x;
	}

	float CMonster::GetY1()
	{
		return y;
	}

	float CMonster::GetX2()
	{
		return x + 28;
	}

	float CMonster::GetY2()
	{
		return y + 28;
	}

	void CMonster::LoadBitmap()
	{
		//随机数
		if (stochastic % 5 == 0 && stochastic != 0)
		{
			monsteralive.AddBitmap(IDB_ENEMY21, RGB(183, 185, 184));//活的怪
			monsteralive.AddBitmap(IDB_ENEMY22, RGB(183, 185, 184));
			monsteralive.AddBitmap(IDB_ENEMY23, RGB(183, 185, 184));
			monsteralive.AddBitmap(IDB_ENEMY24, RGB(183, 185, 184));

			monsterdead.AddBitmap(IDB_ENEMY25, RGB(183, 185, 184));//死的怪
			monsterdead.AddBitmap(IDB_ENEMY26, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY27, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY28, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY29, RGB(183, 185, 184));
		}
		else if (stochastic % 3 == 0 && stochastic != 0)
		{
			monsteralive.AddBitmap(IDB_ENEMY11, RGB(183, 185, 184));//活的怪
			monsteralive.AddBitmap(IDB_ENEMY12, RGB(183, 185, 184));
			monsteralive.AddBitmap(IDB_ENEMY13, RGB(183, 185, 184));
			monsteralive.AddBitmap(IDB_ENEMY14, RGB(183, 185, 184));

			monsterdead.AddBitmap(IDB_ENEMY15, RGB(183, 185, 184));//死的怪
			monsterdead.AddBitmap(IDB_ENEMY16, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY17, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY18, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY19, RGB(183, 185, 184));
		}
		else if (stochastic  == 889)
		{
			monsteralive.AddBitmap(IDB_ENEMY31, RGB(183, 185, 184));//活的怪
			monsteralive.AddBitmap(IDB_ENEMY32, RGB(183, 185, 184));
			monsteralive.AddBitmap(IDB_ENEMY33, RGB(183, 185, 184));
			monsteralive.AddBitmap(IDB_ENEMY34, RGB(183, 185, 184));
											
			monsterdead.AddBitmap(IDB_ENEMY35, RGB(183, 185, 184));//死的怪
			monsterdead.AddBitmap(IDB_ENEMY36, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY37, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY38, RGB(183, 185, 184));
			monsterdead.AddBitmap(IDB_ENEMY39, RGB(183, 185, 184));
		}
		else {
			monsteralive.AddBitmap(IDB_ENEMY1, RGB(0, 153, 0));//活的怪
			monsteralive.AddBitmap(IDB_ENEMY2, RGB(0, 153, 0));
			monsteralive.AddBitmap(IDB_ENEMY3, RGB(0, 153, 0));
			monsteralive.AddBitmap(IDB_ENEMY4, RGB(0, 153, 0));

			monsterdead.AddBitmap(IDB_ENEMY5, RGB(0, 153, 0));//死的怪
			monsterdead.AddBitmap(IDB_ENEMY6, RGB(0, 153, 0));
			monsterdead.AddBitmap(IDB_ENEMY7, RGB(0, 153, 0));
			monsterdead.AddBitmap(IDB_ENEMY8, RGB(0, 153, 0));
			monsterdead.AddBitmap(IDB_ENEMY9, RGB(0, 153, 0));
		}
	}

	void CMonster::OnMove(CGMap *gamemap, CPlayer *player)
	{
		end = time(NULL);
		vector<pair<float, float>> clearns = gamemap->GetCoordinatesOfclearing();
		vector<pair<float, float>>::iterator it;
		it = find(clearns.begin(), clearns.end(), make_pair(x, y));

		if ((end - start) % 4 == 0)
		{
			isTurn = true;
		}
		if (isTurn && it != clearns.end() && (IsLeft(gamemap, player) || IsRight(gamemap, player) || IsDown(gamemap, player) || IsUp(gamemap, player))) {
			SetMovingLeft(false);
			SetMovingRight(false);
			SetMovingDown(false);
			SetMovingUp(false);
			int a[4];
			if (IsLeft(gamemap, player)) {
				a[0] = 2;
			}
			else {
				a[0] = randomNum;
			}
			if (IsRight(gamemap, player)) {
				a[1] = 3;
			}
			else {
				a[1] = randomNum;
			}
			if (IsDown(gamemap, player)) {
				a[2] = 4;
			}
			else {
				a[2] = randomNum;
			}
			if (IsUp(gamemap, player)) {
				a[3] = 1;
			}
			else {
				a[3] = randomNum;
			}
			int num = rand() % 4;
			SetRandomNum(a[num]); // 3往右走
			SetDirection();
			isTurn = false;
		}
		else {
			if (randomNum == 2 && !IsLeft(gamemap, player)) {
				SetMovingLeft(false);
				int a[3];
				//monster[i].SetRandomNum((rand() % 4) + 1);
				if (IsUp(gamemap, player))
					a[0] = 1;
				else
					a[0] = 3;
				if (IsDown(gamemap, player))
					a[1] = 4;
				else
					a[1] = 3;
				a[2] = 3;
				int num = rand() % 3;
				SetRandomNum(a[num]); // 3往右走
				SetDirection();
				//SetIsTurn(true);

			}
			if (randomNum == 3 && !IsRight(gamemap, player)) {
				SetMovingRight(false);
				int a[3];
				//monster[i].SetRandomNum((rand() % 4) + 1);
				if (IsUp(gamemap, player))
					a[0] = 1;
				else
					a[0] = 2;
				if (IsDown(gamemap, player))
					a[1] = 4;
				else
					a[1] = 2;
				a[2] = 2;
				int num = rand() % 3;
				SetRandomNum(a[num]); // 2往左走
				SetDirection();
				//SetIsTurn(true);
			}

			if (randomNum == 4 && !IsDown(gamemap, player)) {
				SetMovingDown(false);
				int a[3];
				if (IsRight(gamemap, player))
					a[0] = 3;
				else
					a[0] = 1;
				if (IsLeft(gamemap, player))
					a[1] = 2;
				else
					a[1] = 1;
				a[2] = 1;
				int num = rand() % 3;
				//SetRandomNum((rand() % 4) + 1);
				SetRandomNum(a[num]); // 1往上走
				SetDirection();
				//SetIsTurn(true);
			}

			if (randomNum == 1 && !IsUp(gamemap, player)) {
				SetMovingUp(false);
				int a[3];
				if (IsRight(gamemap, player))
					a[0] = 3;
				else
					a[0] = 4;
				if (IsLeft(gamemap, player))
					a[1] = 2;
				else
					a[1] = 4;
				a[2] = 4;
				int num = rand() % 3;

				SetRandomNum(a[num]); // 4往下走
				//monster[i].SetRandomNum((rand() % 4) + 1);
				SetDirection();
				//SetIsTurn(true);
			}
		}
	
		if (is_alive) {
			if (isMoveUp)
				y -= SPEED;
			else if (isMoveDown)
				y += SPEED;
			else if (isMoveLeft)
				x -= SPEED;
			else if (isMoveRight)
				x += SPEED;
		}	
	}

	void CMonster::SetIsAlive(bool alive)
	{
		is_alive = alive;
	}

	void CMonster::SetXY(float nx, float ny)
	{
		x = nx; y = ny;
	}

	void CMonster::OnShow(float sx, float sy)
	{
		//活的动画
		if (is_alive) {
			monsteralive.SetTopLeft(x - sx , y - sy);
			monsteralive.OnMove();
			monsteralive.OnShow();
		}
		if (is_alive == false) {
			if (monsterdead.IsFinalBitmap()) {
			}
			else {
				monsterdead.SetTopLeft(x - sx, y - sy);
				monsterdead.OnMove();
				monsterdead.OnShow();
			}
		}
	}
	CMonstercoin::CMonstercoin()
	{
		stochastic = 889;
		Initialize();
	}
}