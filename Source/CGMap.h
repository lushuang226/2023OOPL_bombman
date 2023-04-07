#ifndef CGMAP_H
#define CGMAP_H
#include <map>
#include <vector>
#include "CESWall.h"
#include "CProps.h"
#include "CPlayer.h"
#include "CMonster.h"
#include <algorithm>

namespace game_framework {

	class CProps;
	class CGMap 
	{
	public:
		CGMap();
		void LoadBitmap();												// 载入地图
		void OnShow();													// 显示地图
		void OnMove();													// 位移逻辑（目前不需要实现）
		void ShowMap(int map[15][40]);									// 显示地图
		void ShowCutFigure();											// 切过场图
		void AssignMap(int map[15][40]);								// 给gamemap赋值
		void SwitchingMap();											// 切换地图
		bool HitRectangle(float tx1, float ty1, float tx2, float ty2, CPlayer *player);	//检测是否碰到參數範圍的矩形
		bool HitRectangle(float tx1, float ty1, float tx2, float ty2);					//检测是否碰到參數範圍的矩形
		bool HitRectangleOfMonster(float tx1, float ty1, float tx2, float ty2);			//检测是否碰到參數範圍的矩形用于特殊怪物
		bool HitRectangleOfProps(CPlayer *player, CGMap *gamemap, CMonster *monster);	//检测是否碰到參數範圍的矩形用于门道具
		void ChangePass(CPlayer *player, CGMap *gamemap,bool x);		// 单独的切关功能
		bool HitRectangleOfSpeed(CPlayer *player);						//检测是否碰到參數範圍的矩形用于加速道具
		bool HitRectangleOfBombnum(CPlayer *player);					//检测是否碰到參數範圍的矩形用于加炸弹数道具
		bool HitRectangleOfBombpow(CPlayer *player);					//检测是否碰到參數範圍的矩形用于加炸弹威力道具
		bool HitRectangleOfMissAll(CPlayer *player);					//检测是否碰到參數範圍的矩形用于无敌道具
		bool HitRectangleOfCrossBomb(CPlayer *player);					//检测是否碰到參數範圍的矩形用于穿炸弹道具
		bool HitRectangleOfControl(CPlayer *player);					//检测是否碰到參數範圍的矩形用于控制炸弹道具
		bool HitRectangleOfCrossWall(CPlayer *player);					//检测是否碰到參數範圍的矩形用于穿墙道具
		bool HitRectangleOfMissFire(CPlayer *player);					//检测是否碰到參數範圍的矩形用于无视火焰道具
		void InitializeForRestart();									// 初始化可炸墙块用于重开游戏
		void SetIsAliveByXY(float x, float y, bool flag);				// 根据XY坐标来设置软墙块的存活状态
		void SetLevel(int x);											// 设置关卡
		int GetLevel();													// 获取目前关卡数
		float GetIndexOfESWallByXY(float x, float y);					// 根据XY坐标获取可炸墙块的索引
		vector<pair<float, float>> GetCoordinatesOfTheWall();			// 获取可炸墙块的坐标，返回类型为multimap。
		vector<pair<float, float>> GetCoordinatesOfTheMonster();		// 获取怪物的坐标，返回类型为vector<pair<float, float>>。
		vector<pair<float, float>> GetCoordinatesOfAllWalls();			// 获取所有墙块的坐标（硬墙和软墙）
		vector<pair<float, float>> GetCoordinatesOfclearing();			// 获取空地坐标
		void DeleteBrokenWallCoordinates(float x, float y);				// 根据XY坐标删除对应的软墙
		void ClearnCorrdinates();										// 清空所有vector里储存的坐标
		int GetMonsterNum();											// 获取怪物的数量
		void SetIsCutFigure(bool cutFigure);							// 设定切关
		bool GetIsCutFigure();											// 获取切关
		void SetStart(time_t s);										// 获取怪物的数量
		void MovingScreen(CPlayer *player);								// 移动视窗
		void SetSX(float sx);											// 获得荧幕左边坐标
		float GetSX();													// 获得荧幕左边坐标
		float GetSY();													// 获得荧幕右边坐标
		~CGMap();														// 析构函数
	private:
		CMovingBitmap green;											// 建立绿色地图
		CMovingBitmap grey;												// 建立灰色地图
		CMovingBitmap wall;												// 建立墙的地图
		vector<CMovingBitmap*> cutFigure;								// 过场图
		bool isCutFigure;												// 判断过场图是否显示
		CDoor		  door;												// 门
		CSpeed		  speed;											// 加速
		CBombnum	  bombnum;											// 加炸弹数
		CBombpow	  bombpow;											// 加威力
		CMissAll	  missall;											// 无敌
		CCrossBomb	  crossbomb;										// 穿炸弹
		CControl	  control;											// 控制炸弹
		CCrossWall    crosswall;										// 穿墙
		CMissFire     missfire;											// 无视火焰
		CAnimation animation;											// 可炸墙的动画
		time_t start;													// 起始时间
		time_t end;														// 结束时间
		int gamemap[15][40];											// 建立一个地图矩阵的index
		vector<pair<float, float>>eswalls;								// 存放可炸墙块坐标
		vector<pair<float, float>> monsters;							// 存放怪物坐标
		vector<pair<float, float>> walls;								// 存放墙块坐标（包含硬墙和软墙）
		vector<pair<float, float>> clearing;							// 存放空地坐标
		float SX = 0, SY = 0;											// (sx, sy)為螢幕(的左上角)在地圖上的點座標
		const float MW, MH;												// 每张小地图的高、宽
		vector<CESWall*> ceswall;										// 存放可炸墙块
		const int ESWALLNUM;											// 可炸墙块数（上限值）
		const int MONSTERNUM;											// 怪物数量（上限值）
		int level;														// 关卡值
		bool cycletime;													// 判断是否循环一次
	};
}
#endif