#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CPlayer.h"
#include "CGMap.h"
namespace game_framework {
	CPlayer::CPlayer():BOMBNUM(10)
	{
		Initialize();
	}

	float CPlayer::GetX1()
	{
		return x;
	}

	float CPlayer::GetY1()
	{
		return y;
	}

	float CPlayer::GetX2()
	{
		return x + 28;
	}

	float CPlayer::GetY2()
	{
		return y + 28;
	}

	void CPlayer::Initialize()
	{
		animation = Up;
		isAlive = true;
		const float X_POS = 32;
		const float Y_POS = 96;
		x = X_POS;												// 人物x初始坐标
		y = Y_POS;												// 人物y初始坐标
		bomb_num = 1;											// 携带炸弹数
		missall = false;										// 重置无敌状态(false)
		crossbomb = false;										// 重置穿炸弹状态(false)
		control = bcontrol = false;								// 重置控制炸弹(false)
		speed = lspeed = rspeed = uspeed = dspeed = 1;			// 初始速度
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isSpace = cyctime = false;
		crosswalls = false;
		missfire = false;
		isMoving = true;

		if (cbomb.size() == 0) {
			for (int i = 0; i < BOMBNUM; i++)
			{
				cbomb.push_back(new CBomb());
			}
		}
	}

	void CPlayer::InitializeBomb()
	{
		for (int i = 0; i < BOMBNUM; i++) {
			cbomb[i]->SetIsEnd(false);
			cbomb[i]->SetIsUse(false);
			cbomb[i]->SetIsAlive(true);
			cbomb[i]->SetXY(0, 0);
			cbomb[i]->ResetAnimation();
			cbomb[i]->SetEnd(0);
			cbomb[i]->SetStart(0);
		}

	}

	void CPlayer::LoadBitmap()									   
	{
		animation.AddBitmap(IDB_PU1, RGB(183, 185, 184));

		Up.AddBitmap(IDB_PU1, RGB(183, 185, 184));
		Up.AddBitmap(IDB_PU2, RGB(183, 185, 184));
		Up.AddBitmap(IDB_PU3, RGB(183, 185, 184));
		Up.AddBitmap(IDB_PU4, RGB(183, 185, 184));

		Down.AddBitmap(IDB_PD1, RGB(183, 185, 184));
		Down.AddBitmap(IDB_PD2, RGB(183, 185, 184));
		Down.AddBitmap(IDB_PD3, RGB(183, 185, 184));
		Down.AddBitmap(IDB_PD4, RGB(183, 185, 184));

		Left.AddBitmap(IDB_PL1, RGB(183, 185, 184));
		Left.AddBitmap(IDB_PL2, RGB(183, 185, 184));
		Left.AddBitmap(IDB_PL3, RGB(183, 185, 184));
		Left.AddBitmap(IDB_PL4, RGB(183, 185, 184));

		Right.AddBitmap(IDB_PR1, RGB(183, 185, 184));
		Right.AddBitmap(IDB_PR2, RGB(183, 185, 184));
		Right.AddBitmap(IDB_PR3, RGB(183, 185, 184));
		Right.AddBitmap(IDB_PR4, RGB(183, 185, 184));

		Dead.AddBitmap(IDB_PLAYERDEAD1, RGB(183, 185, 184));
		Dead.AddBitmap(IDB_PLAYERDEAD2, RGB(183, 185, 184));
		Dead.AddBitmap(IDB_PLAYERDEAD3, RGB(183, 185, 184));
		Dead.AddBitmap(IDB_PLAYERDEAD4, RGB(183, 185, 184));
		Dead.AddBitmap(IDB_PLAYERDEAD5, RGB(183, 185, 184));
		Dead.AddBitmap(IDB_PLAYERDEAD6, RGB(183, 185, 184));
		Dead.AddBitmap(IDB_PLAYERDEAD7, RGB(183, 185, 184));

		for (int i = 0; i < BOMBNUM; i++) {
			cbomb[i]->LoadBitmap();
		}
	}

	void CPlayer::OnMove(CGMap *map, CPlayer *player)
	{
		if (map->HitRectangle(GetX1() + 8, GetY1() + 3, GetX2() + 2, GetY2(),player)) {
			isMovingRight = 0;
		}

		if (map->HitRectangle(GetX1(), GetY1() + 3, GetX2() - 5, GetY2(), player)) {
			isMovingLeft = 0;
		}

		if (map->HitRectangle(GetX1() + 3, GetY1() + 3, GetX2()-1, GetY2() + 4, player)) {
			isMovingDown = 0;
		}

		if (map->HitRectangle(GetX1() + 3, GetY1() - 2, GetX2()-1, GetY2() - 3, player)) {
			isMovingUp = 0;			
		}

		if(cyctime == false){
			int count = 0;

			for (int i = 0; i < BOMBNUM; i++) {
				if (!(cbomb[i]->HitRectangleOfBomb(GetX1(), GetY1(), GetX2(), GetY2()))) {
					count++;
				}
			}

			if (count == BOMBNUM)
			{
				isMoving = false;
				cyctime = true;
			}
			else {
				isMoving = true;
			}
		}

		for (int i = 0; i < BOMBNUM; i++) {
			if (((cbomb[i]->HitRectangleOfBomb(GetX1(), GetY1()-2, GetX2(), GetY2()-26) && i != bomb_index) 
				|| (cbomb[i]->HitRectangleOfBomb(GetX1(), GetY1() - 2, GetX2(), GetY2() - 2) && i == bomb_index && isMoving == false) )) 
			{
				if (crossbomb) {
					uspeed = speed;
				}
				else {
					uspeed = 0;
				}
				break;
			}
			else {
				uspeed = speed;
			}
		}

		for (int i = 0; i < BOMBNUM; i++) {
			if (((cbomb[i]->HitRectangleOfBomb(GetX1()-3, GetY1(), GetX2()-26, GetY2()) && i != bomb_index) 
				|| (cbomb[i]->HitRectangleOfBomb(GetX1() - 3, GetY1(), GetX2() - 3, GetY2()) && i == bomb_index && isMoving == false) )) 
			{
				if (crossbomb) {
					lspeed = speed;		
				}
				else {
					lspeed = 0;	
				}
				break;
			}
			else {
				lspeed = speed;
			}
		}

		for (int i = 0; i < BOMBNUM; i++) {
			if (((cbomb[i]->HitRectangleOfBomb(GetX1(), GetY1()+26, GetX2(), GetY2()+2) && i != bomb_index) 
				|| (cbomb[i]->HitRectangleOfBomb(GetX1(), GetY1() + 2, GetX2(), GetY2() + 2) && i == bomb_index && isMoving == false) )) 
			{
				if (crossbomb) {
					dspeed = speed;
				}
				else {
					dspeed = 0;
				}
				break;
			}
			else {
				dspeed = speed;
			}
		}

		for (int i = 0; i < BOMBNUM; i++) {
			if ((cbomb[i]->HitRectangleOfBomb(GetX1() + 26, GetY1(), GetX2() + 3, GetY2()) && i != bomb_index) 
				|| (cbomb[i]->HitRectangleOfBomb(GetX1() + 3, GetY1(), GetX2() + 3, GetY2()) && i == bomb_index && isMoving == false) ) 
			{
				if (crossbomb)
				{
					rspeed = speed;	
				}
				else {
					rspeed = 0;	
				}				
				break;
			}
			else {
				rspeed = speed;
			}
		}

		if (!missall && !missfire) {
			if(HitRectangleOfExplosion(GetX1(), GetY1(), GetX2(), GetY2()))
				isAlive = false;
		}

		if (isAlive) {
			if (isMovingLeft) {
				x -= lspeed;
				Left.OnMove();
			}
			if (isMovingRight) {
				x += rspeed;
				Right.OnMove();
			}
			if (isMovingUp) {
				y -= uspeed;
				Up.OnMove();
			}
			if (isMovingDown) {
				y += dspeed;
				Down.OnMove();
			}
		}
		
		if (isAlive == false)
			Dead.OnMove();

		for (int i = 0; i < bomb_num; i++) {
			cbomb[i]->OnMove();
			if (cbomb[i]->GetIsEnd() && cbomb[i]->GetIsUse()) {
				cbomb[i]->SetIsEnd(false);
				cbomb[i]->SetIsUse(false);
				cbomb[i]->SetIsAlive(true);
				cbomb[i]->SetXY(0, 0);
				cbomb[i]->ResetAnimation();
				cbomb[i]->SetEnd(0);
				cbomb[i]->SetStart(0);
			}
		}
	}

	void CPlayer::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CPlayer::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CPlayer::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CPlayer::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CPlayer::SetIsSpace(bool flag)
	{
		isSpace = flag;
	}

	void CPlayer::SetXY(float nx, float ny)
	{
		x = nx; y = ny;
	}

	void CPlayer::SetIsAlive(bool flag)
	{
		isAlive = flag;
	}

	bool CPlayer::GetIsSpace()
	{
		return isSpace;
	}

	void CPlayer::ResetAnimation()
	{
		animation.Reset();
		Up.Reset();
		Down.Reset();
		Right.Reset();
		Left.Reset();
		Dead.Reset();
		for (int i = 0; i < bomb_num; i++) {
			cbomb[i]->ResetAnimation();
		}
	}

	bool CPlayer::GetIsAlive()
	{
		return isAlive;
	}

	bool CPlayer::HitRectangleOfBomb(float tx1, float ty1, float tx2, float ty2)
	{
		for (int i = 0; i < bomb_num; i++) {
			if (cbomb[i]->HitRectangleOfBomb(tx1, ty1, tx2, ty2)) {
				return true;
			}
		}
		return false;
	}

	bool CPlayer::HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2)
	{
		for (int i = 0; i < bomb_num; i++) {
			if (cbomb[i]->HitRectangleOfExplosion(tx1, ty1, tx2, ty2)) {
				return true;
			}
		}
		return false;
	}

	bool CPlayer::HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2, vector<pair<float, float>> eswalls)
	{
		for (int i = 0; i < bomb_num; i++) {
			if (cbomb[i]->HitRectangleOfExplosion(tx1, ty1, tx2, ty2,eswalls)) {
				return true;
			}
		}
		return false;
	}

	void CPlayer::SetBombFlames(vector<pair<float, float>> walls)
	{
		for (int i = 0; i < bomb_num; i++)
			cbomb[i]->SetFire(walls);
	}
	 
	void CPlayer::SerBombXY(vector<pair<float, float>> clearing, vector<pair<float, float>>eswalls)// clearing空地坐标,eswalls可炸墙坐标
	{
		if (isSpace)
		{
			float min = sqrt((clearing[0].first - x) * (clearing[0].first - x) + (clearing[0].second - y) * (clearing[0].second - y));
			float minx = clearing[0].first;
			float miny = clearing[0].second;

			for (int i = 0; i < clearing.size(); i++)
			{
				float temp = sqrt((clearing[i].first - x) * (clearing[i].first - x) + (clearing[i].second - y) * (clearing[i].second - y));
				if (temp < min) {
					min = temp;
					minx = clearing[i].first;
					miny = clearing[i].second;
				}
			}

			float x1 = minx;
			float y1 = miny;
			vector<pair<float, float>>::iterator it;
			it = find(eswalls.begin(), eswalls.end(), make_pair(minx, miny));
			bool flag = false;

			for (int i = 0; i < bomb_num; i++) {
				if (cbomb[i]->GetIsUse() == true && cbomb[i]->GetX() == x1 && cbomb[i]->GetY() == y1)
				{
					flag = true;
					break;
				}
			}

			for (int i = 0; i < bomb_num; i++) {
				if (cbomb[i]->GetIsEnd() == false && cbomb[i]->GetIsUse() == false && flag == false && it == eswalls.end()) {
					CAudio::Instance()->Play(9);					// 放置炸弹的音效
					cbomb[i]->SetXY(x1, y1);
					cbomb[i]->SetSpace(true);
					cbomb[i]->SetIsUse(true);
					bomb_index = i;
					break;
				}
			}
			isSpace = false;
			cyctime = false;
		}
	}

	void CPlayer::SetBombNum(int num)
	{
		if (num <= BOMBNUM) {
			bomb_num = num;
		}	
	}

	void CPlayer::AddBombNum(int num)
	{
		if (num <= BOMBNUM) {
			bomb_num += num;
		}
	}

	int CPlayer::GetBombNum()
	{
		return bomb_num;
	}

	void CPlayer::SetBombPow()
	{
		for (int i = 0; i < BOMBNUM; i++) {
			cbomb[i]->SetBombLevel(cbomb[i]->GetBombLevel() + 1);
		}
	}

	int CPlayer::GetBombPow()
	{
		return cbomb[0]->GetBombLevel();
	}

	void CPlayer::SetSpeed(float num)
	{
		speed = num;
	}

	void CPlayer::AddSpeed(float num)
	{
		speed += num;
	}

	float CPlayer::GetSpeed()
	{
		return speed;
	}

	float CPlayer::GetRSpeed()
	{
		return rspeed;
	}

	float CPlayer::GetLSpeed()
	{
		return lspeed;
	}

	void CPlayer::SetMissAll(bool flag)
	{
		missall = flag;
	}

	bool CPlayer::GetMissAll()
	{
		return missall;
	}

	void CPlayer::SetCrossBomb(bool flag)
	{
		crossbomb = flag;
	}

	bool CPlayer::GetCrossBomb()
	{
		return crossbomb;
	}

	void CPlayer::SetControl(bool flag)
	{
		control = flag;
	}

	void CPlayer::SetBombControl(bool flag)
	{
		bcontrol = flag;
	}

	bool CPlayer::GetControl()
	{
		return control;
	}

	void CPlayer::SetBombFire(int num)
	{
		for (int i = 0; i < BOMBNUM; i++)
			cbomb[i]->SetBombLevel(num);
	}

	void CPlayer::Control()
	{
		if (control && bcontrol) {
			for (int i = 0; i < BOMBNUM; i++) {
				if (cbomb[i]->GetIsUse() && cbomb[i]->GetIsAlive()) {
					cbomb[i]->SetIsAlive(false);
				}
			}
			control = false;
		}
	}

	void CPlayer::SetRSpeed(float speed)
	{
		rspeed = speed;
	}

	void CPlayer::SetLSpeed(float speed)
	{
		lspeed = speed;
	}

	bool CPlayer::GetIsMovingRight()
	{
		return isMovingRight;
	}

	bool CPlayer::GetIsMovingLeft()
	{
		return isMovingLeft;
	}

	bool CPlayer::GetCrossWalls()
	{
		return crosswalls;
	}

	bool CPlayer::GetIsFinal()
	{
		return Dead.IsFinalBitmap();
	}

	void CPlayer::SetCrossWalls(bool flag)
	{
		crosswalls = flag;
	}

	void CPlayer::SetMissFire(bool flag)
	{
		missfire = flag;
	}

	CPlayer::~CPlayer()
	{
		for (int i = 0; i < BOMBNUM; i++)
			delete cbomb[i];
	}

	void CPlayer::OnShow(CGMap *map)
	{
		if (isMovingDown && isMovingLeft ||
			isMovingDown  && isMovingUp  || 
			isMovingUp && isMovingLeft   ||
			isMovingDown && isMovingRight||
			isMovingUp && isMovingRight  ||
			isMovingRight && isMovingLeft) {

		}
		else if(isAlive) {
		
			if (isMovingLeft) {
				animation = Left;
			}
			if (isMovingUp) {
				animation = Up;
			}
			if (isMovingRight) {
				animation = Right;
			}
			if (isMovingDown) {
				animation = Down;
			}
		}
		else if (isAlive == false) {
			animation = Dead;
			if (Dead.IsFinalBitmap())
				animation = Up;
		}

		for (int i = 0; i < bomb_num; i++) {
			if (cbomb[i]->GetX() != 0) {
				cbomb[i]->OnShow(map->GetSX(),map->GetSY());
			}
		}
		animation.SetTopLeft(x - map->GetSX(), y - map->GetSY());
		animation.OnShow();
	}
}