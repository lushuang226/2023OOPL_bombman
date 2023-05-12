#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CProps.h"
#include "CGMap.h"

namespace game_framework {

	CProps::CProps()
	{
		is_alive = true;
		x = y = 0.0;
	}

	float CProps::GetX1()
	{
		return x;
	}

	float CProps::GetY1()
	{
		return y;
	}

	float CProps::GetX2()
	{
		return x + 32.0;
	}

	float CProps::GetY2()
	{
		return y + 32.0;
	}

	bool CProps::IsAlive()
	{
		return is_alive;
	}

	void CProps::SetIsAlive(bool alive)
	{
		is_alive = alive;
	}

	bool CProps::HitRectangle(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangles(player->GetX1() + 3, player->GetY1() + 3, player->GetX2() - 3, player->GetY2() - 3) && is_alive == true) {
			vector<pair<float, float>>::iterator it;
			it = find(eswalls.begin(), eswalls.end(), make_pair(x, y));
			if (it == eswalls.end()) {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}

	bool CProps::HitRectangles(float tx1, float ty1, float tx2, float ty2)
	{
		float x1 = x + 3.0;					// 左上角x座標
		float y1 = y + 3.0;					// 左上角y座標
		float x2 = x1 + 32.0 - 3.0;			// 右下角x座標
		float y2 = y1 + 32.0 - 3.0;			// 右下角y座標
		return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	}

	void CProps::SetXY(float nx, float ny)
	{
		x = nx; y = ny;
	}

	// ======================================门================================================
	bool CDoor::HitRectangleFromDoor(CPlayer *player, CGMap *gamemap, vector<pair<float, float>> eswalls, CMonster *monster)//传入玩家是检查有没有碰到道具，传入地图是设置道具关卡。
	{
		int flag = 0;
		for (int i = 0; i < gamemap->GetMonsterNum(); i++)
		{
			if (monster[i].IsAlive())
				flag++;
		}
		// 檢測是否碰到
		if (HitRectangle(player, eswalls) && flag == 0) {
			ChangePass(player, gamemap, true);
			return true;
		}
		flag = 0;
		return false;
	}

	void CDoor::LoadBitmap()
	{
		door.LoadBitmap(IDB_DOOR);										// 載入圖形
	}

	void CDoor::OnShow(float sx, float sy)
	{
		if (is_alive) {
			door.SetTopLeft(x - sx, y - sy);
			door.ShowBitmap();
		}
	}

	void CDoor::ChangePass(CPlayer *player, CGMap *gamemap, bool flag)	// 单独的切关功能
	{
		if (flag == true) {
			float speed = player->GetSpeed();			
			int bombnum = player->GetBombNum();			
			player->Initialize();						
			player->InitializeBomb();					
			player->SetSpeed(speed);					
			player->SetBombNum(bombnum);				
			gamemap->ClearnCorrdinates();				
			gamemap->InitializeForRestart();
			gamemap->SetLevel(gamemap->GetLevel() +1);	
			gamemap->SwitchingMap();					
			gamemap->SetIsCutFigure(true);				
			gamemap->SetStart(time(NULL));	
			gamemap->SetSX(0.0);
		}
	}

	// ======================================加速================================================
	bool CSpeed::HitRectangleFromSpeed(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->AddSpeed(0.5);														// 加速
			is_alive = false;
			return true;
		}
		return false;
	}

	void CSpeed::LoadBitmap()
	{
		speed.LoadBitmap(IDB_SPEED);
	}

	void CSpeed::OnShow(float sx, float sy)
	{
		if (is_alive) {
			speed.SetTopLeft(x - sx, y - sy);
			speed.ShowBitmap();
		}
	}

	// ======================================加炸弹数================================================
	bool CBombnum::HitRectangleFromBombNum(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->SetBombNum(player->GetBombNum() + 1);								// 加炸弹数
			is_alive = false;
			return true;
		}
		return false;
	}

	void CBombnum::LoadBitmap()
	{
		bombnum.LoadBitmap(IDB_BOMBNUM);
	}
	
	void CBombnum::OnShow(float sx, float sy)
	{
		if (is_alive) {
			bombnum.SetTopLeft(x - sx, y - sy);
			bombnum.ShowBitmap();
		}
	}

	// ======================================加威力================================================
	bool CBombpow::HitRectangleFromBombPow(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->SetBombPow();														//加威力
			is_alive = false;
			return true;
		}
		return false;
	}
	
	void CBombpow::LoadBitmap()
	{
		bombpow.LoadBitmap(IDB_BOMBPOW);
	}

	void CBombpow::OnShow(float sx, float sy)
	{
		if (is_alive) {
			bombpow.SetTopLeft(x - sx, y - sy);
			bombpow.ShowBitmap();
		}
	}

	// ======================================无敌================================================
	bool CMissAll::HitRectangleFromMissAll(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->SetMissAll(true);													//无敌
			is_alive = false;
			return true;
		}
		return false;
	}
	
	void CMissAll::LoadBitmap()
	{
		missall.LoadBitmap(IDB_MISSALL);
	}
	
	void CMissAll::OnShow(float sx, float sy)
	{
		if (is_alive) {
			missall.SetTopLeft(x - sx, y -sy);
			missall.ShowBitmap();
		}
	}

	// ======================================穿炸弹================================================
	bool CCrossBomb::HitRectangleFromCrossBomb(CPlayer * player, vector<pair<float, float>> eswalls)
	{

		if (HitRectangle(player, eswalls)) {
			player->SetCrossBomb(true);													//穿炸弹
			is_alive = false;
			return true;
		}
		return false;
	}
	
	void CCrossBomb::LoadBitmap()
	{
		crossbomb.LoadBitmap(IDB_CROSSBOMB);
	}
	
	void CCrossBomb::OnShow(float sx, float sy)
	{
		if (is_alive) {
			crossbomb.SetTopLeft(x - sx, y - sy);
			crossbomb.ShowBitmap();
		}
	}

	// ======================================控制炸弹================================================
	bool CControl::HitRectangleFromBombControl(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->SetBombControl(true);												//控制
			is_alive = false;
			return true;
		}
		return false;
	}
	
	void CControl::LoadBitmap()
	{
		control.LoadBitmap(IDB_CONTROL);
	}
	
	void CControl::OnShow(float sx, float sy)
	{
		if (is_alive) {
			control.SetTopLeft(x -sx, y - sy);
			control.ShowBitmap();
		}
	}

	bool CCrossWall::HitRectangleFromCrossWalls(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->SetCrossWalls(true);												//穿墙
			is_alive = false;	
			return true;
		}
		return false;
	}
	
	void CCrossWall::LoadBitmap()
	{
		crosswall.LoadBitmap(IDB_CROSSWALL);
	}
	void CCrossWall::OnShow(float sx, float sy)
	{
		if (is_alive) {
			crosswall.SetTopLeft(x - sx, y - sy);
			crosswall.ShowBitmap();
		}
	}

	bool CMissFire::HitRectangleFromMissFire(CPlayer * player, vector<pair<float, float>> eswalls)
	{
		if (HitRectangle(player, eswalls)) {
			player->SetMissFire(true);													//无视火焰
			is_alive = false;
			return true;
		}
		return false;
	}

	void CMissFire::LoadBitmap()
	{
		missfire.LoadBitmap(IDB_MISSFIRE);
	}

	void CMissFire::OnShow(float sx, float sy)
	{
		if (is_alive) {
			missfire.SetTopLeft(x - sx, y - sy);
			missfire.ShowBitmap();
		}
	}
}
