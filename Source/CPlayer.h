#ifndef CPLAYER_H
#define CPLAYER_H
#include "CBomb.h"
#include <algorithm>
#include <vector>
#include <map>
namespace game_framework {

	class CGMap;
	class CPlayer
	{
	public:
		CPlayer();
		float  GetX1();								// 人物左上角 x 座標
		float  GetY1();								// 人物左上角 y 座標 
		float  GetX2();								// 人物右下角 x 座標
		float  GetY2();								// 人物右下角 y 座標
		void Initialize();							// 設定人物為初始值
		void InitializeBomb();						// 初始化炸弹，主要用于重玩游戏时。
		void LoadBitmap();							// 載入圖形
		void OnMove(CGMap *map, CPlayer *player);	// 移動player
		void OnShow(CGMap *map);					// 將人物圖形貼到畫面
		void SetMovingDown(bool flag);				// 設定是否正在往下移動
		void SetMovingLeft(bool flag);				// 設定是否正在往左移動
		void SetMovingRight(bool flag);				// 設定是否正在往右移動
		void SetMovingUp(bool flag);				// 設定是否正在往上移動
		void SetIsSpace(bool flag);					// 設定空格后的动作
		void SetXY(float nx, float ny);				// 設定擦子左上角座標
		void SetIsAlive(bool flag);					// 设置人物生命状态
		bool GetIsSpace();							// 判断是否按了空格
		void ResetAnimation();						// 重置所有动画张数
		bool GetIsAlive();							// 获取人物生命状态
		bool HitRectangleOfBomb(float tx1, float ty1, float tx2, float ty2);											// 有关炸弹的相交检测
		bool HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2);										// 有关爆炸火焰的相交检测
		bool HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2, vector<pair<float, float>> eswalls);	// 有关爆炸火焰的相交检测（函数重载）
		void SetBombFlames(vector<pair<float, float>> walls);															// 变换炸弹火焰长度
		void SerBombXY(vector<pair<float, float>> clearing, vector<pair<float, float>>eswalls);							// 设置炸弹坐标
		void SetBombNum(int num);		// 设置人物携带的炸弹数量
		void AddBombNum(int num);		// 增加人物携带的炸弹数量
		int GetBombNum();				// 获取炸弹数
		void SetBombPow();				// 设置人物携带的炸弹威力
		int GetBombPow();				// 获取人物携带的炸弹威力
		void SetSpeed(float num);		// 设置人物速度
		void AddSpeed(float num);		// 增加人物速度
		float GetSpeed();				// 获取人物速度
		void SetRSpeed(float speed);	// 设置人物右方向速度
		float GetRSpeed();				// 获取人物右方向速度
		void SetLSpeed(float speed);	// 设置人物左方向速度
		float GetLSpeed();				// 获取人物左方向速度
		void SetMissAll(bool flag);		// 设置missall属性
		bool GetMissAll();				// 获取missall属性
		void SetCrossBomb(bool flag);	// 设置crossbomb属性
		bool GetCrossBomb();			// 获取crossbomb属性
		void SetControl(bool flag);		// 设置control属性
		bool GetControl();				// 获取control属性
		void SetBombControl(bool flag);	// 设置bcontrol属性
		void SetBombFire(int num);		// 获取bcontrol属性
		void Control();					// 引爆炸弹逻辑
		bool GetIsMovingRight();		// 获取isMovingRight属性
		bool GetIsMovingLeft();			// 获取isMovingLeft属性
		bool GetCrossWalls();			// 获取crosswalls属性
		bool GetIsFinal();				// 获取Dead动画是否为最后一张
		void SetCrossWalls(bool flag);	// 设置crosswalls属性
		void SetMissFire(bool flag);	// 设置missfire属性
		~CPlayer();						// 析构函数
	private:
		CAnimation animation;			// 人物初始动画
		CAnimation Up;					// 人物往上走的动画
		CAnimation Down;				// 人物往下走的动画
		CAnimation Right;				// 人物往右走的动画
		CAnimation Left;				// 人物往左走的动画
		CAnimation Dead;				// 人物死亡的动画
		float x, y;						// 人物左上角座標
		bool isMovingDown;				// 是否正在往下移動
		bool isMovingLeft;				// 是否正在往左移動
		bool isMovingRight;				// 是否正在往右移動
		bool isMovingUp;				// 是否正在往上移動
		bool isAlive;					// 人物是否活著的属性
		bool isSpace;					// 人物是否按空格的属性
		bool missall;					// 人物是否无敌的属性
		bool crossbomb;					// 人物是否穿炸弹的属性
		bool control;					// 人物引爆炸弹的属性
		bool bcontrol;					// 人物是否控制炸弹的属性
		bool crosswalls;				// 人物是否穿墙的属性
		bool missfire;					// 人物是否无视火焰的属性
		vector<CBomb*> cbomb;			// 炸弹类
		int bomb_num;					// 当前携带炸弹的数量
		const int BOMBNUM;				// 定义炸弹的上限
		float speed;					// 总速度
		float lspeed;					// 左速度
		float rspeed;					// 右速度
		float uspeed;					// 上速度
		float dspeed;					// 下速度
		bool isMoving;					// 判断人物是否撞到炸弹
		bool cyctime;					// 判定是否已循环一次
		bool isFinal;
		int bomb_index;					// 炸弹的索引
	};
}
#endif
