#ifndef CPROPS_H
#define CPROPS_H
#include "CPlayer.h"
#include "CMonster.h"
#include <algorithm>
namespace game_framework {

	class CGMap;
	class CProps
	{
	public:
		CProps();
		bool IsAlive();															// 是否活著
		void SetXY(float nx, float ny);											// 設定座標
		void SetIsAlive(bool alive);											// 設定是否活著
		bool HitRectangle(CPlayer *player, vector<pair<float, float>> eswalls);	// 是否碰到參數範圍的矩形
		bool HitRectangles(float tx1, float ty1, float tx2, float ty2);			// 是否碰到參數範圍的矩形
		float  GetX1();															// 获取左上角 x 座標
		float  GetY1();															// 获取左上角 y 座標
		float  GetX2();															// 擦子右下角 x 座標
		float  GetY2();															// 擦子右下角 y 座標
	protected:
		float x, y;																// 座標
		bool is_alive;															// 是否活著		
	};
	// ======================================门================================================
	class CDoor :public CProps 
	{
	public:
		bool HitRectangleFromDoor(CPlayer *player, CGMap *gamemap, vector<pair<float, float>> eswalls, CMonster *monster);		// 碰到门切下一关前提是所有怪杀完
		void LoadBitmap();																										// 載入图形	
		void OnShow(float sx, float sy);																						// 貼到畫面
		void ChangePass(CPlayer *player, CGMap *gamemap, bool flag);															// 切关功能
	protected:
		CMovingBitmap door;				
	};
	// ======================================加速================================================
	class CSpeed :public CProps
	{
	public:
		bool HitRectangleFromSpeed(CPlayer *player, vector<pair<float, float>> eswalls);										// 碰到加速道具会使人物速度增加50%
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap speed;					
	};
	// ======================================加炸弹数================================================
	class CBombnum :public CProps																								
	{
	public:
		bool HitRectangleFromBombNum(CPlayer *player, vector<pair<float, float>> eswalls);										// 碰到加炸弹道具会使人物携带炸弹数+1
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap bombnum;				
	};
	// ======================================加威力================================================
	class CBombpow :public CProps
	{
	public:
		bool HitRectangleFromBombPow(CPlayer *player, vector<pair<float, float>> eswalls);										// 碰到加威力道具会使人物炸弹威力+1
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap bombpow;			
	};
	// ======================================穿墙================================================
	class CCrossWall :public CProps
	{
	public:
		bool HitRectangleFromCrossWalls(CPlayer *player, vector<pair<float, float>> eswalls);									// 碰到穿墙道具会使人物进入穿墙状态
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap crosswall;
	};
	// ======================================穿炸弹================================================
	class CCrossBomb :public CProps
	{
	public:
		bool HitRectangleFromCrossBomb(CPlayer *player, vector<pair<float, float>> eswalls);									// 碰到穿炸弹道具会使人物进入穿炸弹状态
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap crossbomb;
	};
	// ======================================控制炸弹================================================
	class CControl :public CProps
	{
	public:
		bool HitRectangleFromBombControl(CPlayer *player, vector<pair<float, float>> eswalls);									// 碰到控制炸弹道具会使人物进入可引爆炸弹状态
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap control;
	};
	// ======================================无敌================================================
	class CMissAll :public CProps
	{
	public:
		bool HitRectangleFromMissAll(CPlayer *player, vector<pair<float, float>> eswalls);										// 碰到无敌道具会使人物进入无敌状态
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap missall;
	};
	// ======================================无视火焰================================================
	class CMissFire :public CProps
	{
	public:
		bool HitRectangleFromMissFire(CPlayer *player, vector<pair<float, float>> eswalls);										// 碰到无视火焰道具会使人物进入无视火焰状态
		void LoadBitmap();
		void OnShow(float sx, float sy);
	protected:
		CMovingBitmap missfire;
	};
}
#endif