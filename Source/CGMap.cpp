#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGMap.h"
#include "CProps.h"
#pragma once

namespace game_framework {

	CGMap::CGMap()
		:MW(32), MH(32), ESWALLNUM(40), MONSTERNUM(10)// 砖块数对应地图‘3’，注意！3的个数要和砖块数一致！
	{
		level = 1;
		SwitchingMap();
		for (int i = 0; i < ESWALLNUM; i++)
			ceswall.push_back(new CESWall());
		for (int i = 0; i < 13; i++)			// 过场图
			cutFigure.push_back(new CMovingBitmap());
		isCutFigure = true;
	}

	void CGMap::LoadBitmap() {
		for (int i = 0; i < 13; i++)	
			cutFigure[i]->LoadBitmap(270+i);
		green.LoadBitmap(IDB_GREENS);
		grey.LoadBitmap(IDB_WALL);
		wall.LoadBitmap(IDB_WALL1);
		door.LoadBitmap();
		speed.LoadBitmap();
		bombnum.LoadBitmap();
		bombpow.LoadBitmap();
		missall.LoadBitmap();
		crossbomb.LoadBitmap();
		crosswall.LoadBitmap();
		control.LoadBitmap();
		missfire.LoadBitmap();
		for (int i = 0; i < ESWALLNUM; i++)
			ceswall[i]->LoadBitmap();
	}

	void CGMap::OnShow()
	{
		ShowMap(gamemap);
	}

	void CGMap::OnMove() // 用于取坐标
	{
		for (int i = 0; i < 40; i++) {		// 往右显示i张图
			for (int j = 0; j < 15; j++)	// 往下显示j张图
			{
				if (cycletime == false)
				{
					switch (gamemap[j][i]) {
					case 1:	// 硬墙  
						walls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 2:	// 怪物
						monsters.push_back(make_pair((MW*i), (MH*j)));
						clearing.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 3: // 可炸墙
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 4:
						speed.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 5: // 空地
						clearing.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 6: 
						crosswall.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 7:
						bombnum.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 8:
						bombpow.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 9:
						missall.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 10:
						crossbomb.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 11:
						control.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 12:
						missfire.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 13:
						door.SetIsAlive(true);
						clearing.push_back(make_pair((MW*i), (MH*j)));
						walls.push_back(make_pair((MW*i), (MH*j)));
						eswalls.push_back(make_pair((MW*i), (MH*j)));
						break;
					case 14:	// 怪物
						monsters.push_back(make_pair((MW*i), (MH*j)));
						clearing.push_back(make_pair((MW*i), (MH*j)));
						break;
					}
				}
			}
		}
		cycletime = true;
	}

	void CGMap::ShowMap(int map[15][40])
	{
		for (int i = 0; i < 40; i++) {		// 往右显示i张图
			for (int j = 0; j < 15; j++)	// 往下显示j张图
			{
				// 先铺满一层绿色背景
				green.SetTopLeft((MW*(float)i) - SX, (MH*(float)j) - SY);
				green.ShowBitmap();
				switch (map[j][i]) {
				case 0:	// 顶部灰色背景
					grey.SetTopLeft((MW*(float)i) - SX, (MH*(float)j) - SY);
					grey.ShowBitmap();
					break;
				case 1:	// 硬墙
					wall.SetTopLeft((MW*(float)i) - SX, (MH*(float)j) - SY);
					wall.ShowBitmap();
					break;
				case 3: // 可炸墙			 
					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 4:	// 加速
					speed.SetXY((MW*(float)i), (MH*(float)j));
					speed.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 6:// 穿墙
					crosswall.SetXY((MW*(float)i), (MH*(float)j));
					crosswall.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 7:	// 加炸弹数
					bombnum.SetXY((MW*(float)i), (MH*(float)j));
					bombnum.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 8:	// 加威力
					bombpow.SetXY((MW*(float)i), (MH*(float)j));
					bombpow.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 9:	// 无敌
					missall.SetXY((MW*(float)i), (MH*(float)j));
					missall.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 10:	// 穿炸弹
					crossbomb.SetXY((MW*(float)i), (MH*(float)j));
					crossbomb.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 11:	// 控制炸弹
					control.SetXY((MW*(float)i), (MH*(float)j));
					control.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
				case 12:	// 无视火焰威力
					missfire.SetXY((MW*(float)i), (MH*(float)j));
					missfire.OnShow(SX, SY);

					for (int index = 0; index < ESWALLNUM; index++) {
						if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
						{
							ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
							break;
						}
					}
					break;
					case 13:// 门
						door.SetXY((MW*(float)i), (MH*(float)j));
						for (int index = 0; index < ESWALLNUM; index++) {
							if (ceswall[index]->GetX1() == 0 && ceswall[index]->GetY1() == 0)
							{
								ceswall[index]->SetXY((MW*(float)i), (MH*(float)j));
								break;
							}
						}
						break;
				}
			}
		}
		door.OnShow(SX, SY);
		for (int index = 0; index < ESWALLNUM; index++) {
			if(ceswall[index]->GetX1() != 0 && ceswall[index]->GetY1() != 0)
				ceswall[index]->OnShow(SX, SY);
		}

		if (isCutFigure)
			ShowCutFigure();
	}

	void CGMap::ShowCutFigure()
	{		
		end = time(NULL);
		cutFigure[level - 1]->SetTopLeft(0, 0);		// 过场图
		cutFigure[level - 1]->ShowBitmap();
		if(end - start == 4)
			isCutFigure = false;
	}

	void CGMap::AssignMap(int map[15][40])
	{
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 40; j++)
				gamemap[i][j] = map[i][j];
	}

	void CGMap::SwitchingMap()
	{
		int map_init[15][40] =   //地图1
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,5,5,5,5,5,5,5,11,5,5,3,5,3,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,2,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,5,1,5,1},
		{1,5,8,10,5,5,12,5,5,5,9,5,6,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,3,5,5,5,3,5,2,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,13,1,5,1},// <--门在这
		{1,5,3,5,4,5,3,7,5,5,5,5,3,5,5,5,2,3,5,5,5,5,5,5,3,5,3,5,5,5,5,5,3,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,5,5,2,5,5,3,5,5,5,5,5,5,5,3,3,3,5,5,5,5,5,5,5,5,3,5,5,2,5,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,2,3,5,3,5,3,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,3,3,5,5,5,5,5,5,5,5,2,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,5,5,5,5,3,3,5,5,5,5,5,3,2,5,5,5,5,5,5,5,5,5,5,5,5,5,3,3,5,5,5,5,5,5,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		int map_init2[15][40] =   //地图2
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,5,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,8,5,5,3,2,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,1},
		{1,1,12,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,3,5,3,5,5,5,3,5,3,5,5,5,5,2,5,3,3,5,5,5,5,5,5,5,3,5,5,11,5,2,5,5,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,3,5,5,5,5,5,5,5,5,5,5,3,2,5,5,5,5,3,5,5,10,5,5,5,5,5,5,5,5,5,5,3,3,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,3,4,3,5,5,5,5,5,5,3,5,5,5,5,5,2,5,5,3,3,5,5,5,5,5,5,5,5,5,5,2,5,5,5,3,1},
		{1,1,5,1,5,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,13,1},// <--门在这
		{1,5,5,3,5,5,5,5,5,5,5,3,3,5,5,5,3,3,5,5,3,5,5,5,5,6,5,2,5,5,5,5,3,3,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,2,5,5,5,5,5,5,3,5,5,5,5,5,3,3,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,2,5,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		int map_init3[15][40] =   //地图2
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,5,5,5,5,5,3,3,5,5,5,2,5,5,5,5,7,3,5,5,5,3,5,5,5,5,5,5,2,5,5,3,5,5,3,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,5,1},
		{1,3,5,6,3,5,3,5,3,5,5,5,5,3,5,5,5,2,5,3,5,5,3,5,5,5,5,3,5,3,5,5,5,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,3,5,5,5,2,5,5,5,5,5,12,3,5,5,5,5,5,8,5,5,5,5,5,3,5,5,5,5,5,5,5,5,2,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,5,3,3,5,5,5,5,5,5,5,3,3,5,5,5,5,5,2,5,5,5,5,3,5,5,2,5,5,5,10,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,3,3,5,5,5,5,5,5,5,5,5,5,5,2,5,5,5,5,5,3,5,5,5,5,5,5,3,5,5,5,5,5,3,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,13,1,5,1},// <--门在这
		{1,5,5,5,5,5,5,9,5,5,2,5,5,5,5,5,5,5,5,3,3,5,5,5,5,5,5,3,5,5,5,5,2,5,5,5,5,5,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		int map_init4[15][40] =   //地图2
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,5,5,5,5,5,5,3,5,5,5,5,3,5,2,5,5,3,5,5,5,5,5,5,5,5,5,3,5,5,2,5,6,5,5,3,5,2,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,3,5,3,5,3,5,3,5,5,5,5,5,3,5,5,2,5,5,2,5,5,5,7,5,5,3,5,5,5,5,5,5,5,5,5,5,5,1},
		{1,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,2,5,6,5,5,5,6,3,7,5,5,3,3,5,5,5,5,3,5,5,5,3,3,5,5,5,5,3,9,2,5,5,3,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,3,5,3,5,5,3,5,5,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,3,5,5,5,5,3,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,1,5,5,11,5,5,3,5,5,5,3,5,5,5,5,5,5,5,5,8,3,5,5,5,5,5,5,2,5,2,5,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,3,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,3,3,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,5,5,5,13,5,5,5,1},// <--门在这
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		int bonusss[15][40] =   //奖励关
		{
		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		  {1,5,5,5,5,5,5,5,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1},
		  {1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		  {1,5,5,5,5,5,2,5,5,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1},
		  {1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		  {1,5,5,5,5,5,5,2,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1},
		  {1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		  {1,5,5,2,5,5,5,5,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1},
		  {1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		  {1,5,5,5,5,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1},
		  {1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		  {1,5,5,5,2,5,5,5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1},
		  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

		};
		int map_init5[15][40] =   //地图5
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,5,5,5,5,5,5,3,5,5,2,5,3,5,2,5,1,3,5,5,5,5,5,3,5,5,5,3,1,5,2,5,5,5,5,3,5,2,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,3,5,3,5,3,5,3,5,5,5,5,5,3,5,5,2,5,5,2,5,3,5,7,5,5,3,5,5,5,5,2,5,5,5,5,5,5,1},
		{1,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,2,5,5,2,5,5,6,3,5,5,5,3,3,5,5,5,5,3,5,5,5,3,3,5,5,5,5,3,9,2,5,5,3,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,2,1,1,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,1,3,3,5,3,5,5,3,5,5,5,5,1,5,5,5,5,5,5,5,3,5,5,5,5,5,5,3,5,5,5,5,3,5,5,1,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,5,11,5,5,3,5,5,5,3,5,5,5,5,3,5,5,5,8,3,5,5,5,5,5,5,2,5,5,1,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,3,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,3,3,5,5,2,5,5,2,5,5,3,5,5,5,5,1,5,5,5,2,5,5,3,5,5,1,5,5,5,5,5,5,13,5,5,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		int map_init6[15][40] =   //地图6
		{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,5,5,5,5,5,5,3,5,5,5,5,3,5,2,5,5,3,5,5,5,3,5,5,5,5,5,3,5,5,2,5,5,5,5,3,5,5,5,1},
		{1,1,3,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,3,5,3,5,3,5,3,5,5,1,5,5,3,5,5,2,5,5,2,5,5,5,7,5,5,3,5,5,5,5,5,5,5,5,5,5,5,1},
		{1,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,2,5,5,5,5,5,6,3,5,5,5,3,3,5,5,5,5,3,5,5,5,3,3,5,5,5,5,3,9,2,5,5,3,5,5,3,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,5,3,5,3,5,5,3,5,5,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,3,5,1,5,5,3,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,2,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,1,3,5,11,5,5,3,5,1,5,3,5,5,5,5,5,5,5,5,8,3,5,5,5,5,5,5,2,5,5,5,5,5,5,5,5,5,1},
		{1,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,3,1,5,1,3,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1},
		{1,5,5,3,3,5,5,2,5,5,5,3,5,5,5,5,5,5,5,5,5,5,5,5,5,3,5,5,5,5,5,5,5,5,5,13,5,5,5,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		};
		switch (level) {
		case 1:
			AssignMap(map_init);
			break;
		case 2:
			AssignMap(map_init2);
			break;
		case 3:
			AssignMap(map_init3);
			break;
		case 4:
			AssignMap(map_init4);
			break;
		case 5:
			AssignMap(bonusss);
			break;
		default:
			break;
		}
	}

	bool CGMap::HitRectangle(float tx1, float ty1, float tx2, float ty2, CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		int a = 3, b = 4, c = 7, d = 8, e = 9, f = 10, g = 11, h = 6, k = 12, l = 13;
		if (player->GetCrossWalls()) {
			a = b = c = d = e = f = g = h = k = l = 1;
		}
		else {
			a = 3, b = 4, c = 7, d = 8, e = 9, f = 10, g = 11, h = 6, k = 12, l = 13;
		}
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 15; j++) {
				if (gamemap[j][i] == 1 || ((gamemap[j][i] == a || gamemap[j][i] == b || gamemap[j][i] == c || gamemap[j][i] == d || gamemap[j][i] == e || gamemap[j][i] == f || gamemap[j][i] == g || gamemap[j][i] == h || gamemap[j][i] == k || gamemap[j][i] == l) && ceswall[GetIndexOfESWallByXY((MW*(float)i), (MH*(float)j))]->GetIsAlive() == true))
				{
					float x1 = (MW*(float)i);				// 左上角x座標
					float y1 = (MH*(float)j);				// 左上角y座標
					float x2 = x1 + 32;						// 右下角x座標
					float y2 = y1 + 32;						// 右下角y座標
					if (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2) {
						return true;				
					}
				}
			}
		}
		return false;
	}

	bool CGMap::HitRectangle(float tx1, float ty1, float tx2, float ty2)
	{
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 15; j++) {
				if (gamemap[j][i] == 1 || ((gamemap[j][i] == 3 || gamemap[j][i] == 4 || gamemap[j][i] == 6 || gamemap[j][i] == 7 || gamemap[j][i] == 8 || gamemap[j][i] == 9 || gamemap[j][i] == 10 || gamemap[j][i] == 11 || gamemap[j][i] == 12 || gamemap[j][i] == 13) && ceswall[GetIndexOfESWallByXY((MW*(float)i), (MH*(float)j))]->GetIsAlive() == true))
				{
					float x1 = (MW*(float)i);				
					float y1 = (MH*(float)j);				
					float x2 = x1 + 32;						
					float y2 = y1 + 32;						
					if (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2) {
						return true;
					}
				}
			}
		}
		return false;
	}

	bool CGMap::HitRectangleOfMonster(float tx1, float ty1, float tx2, float ty2)
	{
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 15; j++) {
				if (gamemap[j][i] == 1)
				{
					float x1 = (MW*(float)i);
					float y1 = (MH*(float)j);
					float x2 = x1 + 32;
					float y2 = y1 + 32;
					if (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2) {
						return true;
					}
				}
			}
		}
		return false;
	}

	bool CGMap::HitRectangleOfProps(CPlayer *player, CGMap *gamemap, CMonster *monster) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return door.HitRectangleFromDoor(player, gamemap, walls, monster);
	}

	void CGMap::ChangePass(CPlayer *player, CGMap *gamemap,bool xx)	// 单独的切关功能
	{
		door.ChangePass(player, gamemap, xx);
	}

	bool CGMap::HitRectangleOfSpeed(CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return speed.HitRectangleFromSpeed(player, walls);
	}

	bool CGMap::HitRectangleOfBombnum(CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return bombnum.HitRectangleFromBombNum(player, walls);
	}

	bool CGMap::HitRectangleOfBombpow(CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return bombpow.HitRectangleFromBombPow(player, walls);
	}

	bool CGMap::HitRectangleOfMissAll(CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return missall.HitRectangleFromMissAll(player, walls);
	}

	bool CGMap::HitRectangleOfControl(CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return control.HitRectangleFromBombControl(player, walls);
	}

	bool CGMap::HitRectangleOfCrossWall(CPlayer * player)
	{
		return crosswall.HitRectangleFromCrossWalls(player, walls);
	}

	bool CGMap::HitRectangleOfMissFire(CPlayer * player)
	{
		return missfire.HitRectangleFromMissFire(player,walls);
	}

	bool CGMap::HitRectangleOfCrossBomb(CPlayer *player) // tx1左上角x坐标、ty1左上角y坐标。
	{
		return crossbomb.HitRectangleFromCrossBomb(player, walls);
	}

	void CGMap::InitializeForRestart()
	{
		for (int i = 0; i < ESWALLNUM; i++) {
			ceswall[i]->Initialize();
			ceswall[i]->ResetAnimation();
		}
		crossbomb.SetIsAlive(false);
		crosswall.SetIsAlive(false);
		control.SetIsAlive(false);
		missall.SetIsAlive(false);
		bombpow.SetIsAlive(false);
		bombnum.SetIsAlive(false);
		speed.SetIsAlive(false);
		missfire.SetIsAlive(false);
		door.SetIsAlive(false);
	}

	void CGMap::SetIsAliveByXY(float x, float y, bool flag)
	{
		float index = GetIndexOfESWallByXY(x, y);
		DeleteBrokenWallCoordinates(x, y);
		ceswall[index]->SetIsAlive(flag);
	}

	void CGMap::SetLevel(int x)
	{
		level = x;
		if (x >= 5) {
			level = 5;
		}
	}

	int CGMap::GetLevel()
	{
		return level;
	}

	float CGMap::GetIndexOfESWallByXY(float x, float y)
	{
		float index = 0; // 只能为0
		for (int i = 0; i < ESWALLNUM; i++) {
			if (ceswall[i]->GetX1() == x && ceswall[i]->GetY1() == y)
			{
				index = i;
			}
		}
		return index;
	}

	vector<pair<float, float>>CGMap::GetCoordinatesOfTheWall()
	{
		return eswalls;
	}

	vector<pair<float, float>> CGMap::GetCoordinatesOfTheMonster()
	{
		return monsters;
	}

	vector<pair<float, float>> CGMap::GetCoordinatesOfAllWalls()
	{
		return walls;
	}

	vector<pair<float, float>> CGMap::GetCoordinatesOfclearing()
	{
		return clearing;
	}

	void CGMap::DeleteBrokenWallCoordinates(float x, float y)
	{
		vector<pair<float, float>>::iterator it;
		it = find(walls.begin(), walls.end(), make_pair(x, y));
		if (it != walls.end()) {
			walls.erase(it);
		}
	}

	void CGMap::ClearnCorrdinates()
	{
		while (!monsters.empty())
		{
			monsters.pop_back();
		}
		while (!walls.empty())
		{
			walls.pop_back();
		}
		while (!clearing.empty())
		{
			clearing.pop_back();
		}
		eswalls.clear();
		cycletime = false;

	}

	int CGMap::GetMonsterNum()
	{
		return MONSTERNUM;
	}

	void CGMap::SetIsCutFigure(bool cutFigure)
	{
		isCutFigure = cutFigure;
	}

	bool CGMap::GetIsCutFigure()
	{
		return isCutFigure;
	}

	void CGMap::SetStart(time_t s)
	{
		start = s;
	}

	void CGMap::MovingScreen(CPlayer * player)
	{
		if (player->GetX1() >= 320 && player->GetX1() <= 960) {
			
			SX = player->GetX1() - 320.0;
		}
	}

	void CGMap::SetSX(float sx) {
		SX = sx;
	}

	float CGMap::GetSX()
	{
		return SX;
	}

	float CGMap::GetSY()
	{
		return SY;
	}

	CGMap::~CGMap()
	{
		for (int i = 0; i < ESWALLNUM; i++)
			delete ceswall[i];
		for (int i = 0; i < cutFigure.size(); i++)
			delete cutFigure[i];

	}
}