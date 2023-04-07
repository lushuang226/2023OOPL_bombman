#pragma once
namespace game_framework {

	class CGMap;
	class CMonster
	{
	public:
		CMonster();
		CMonster(int num);
		bool HitEraser(CPlayer *player);						// 是否碰到怪物
		bool IsAlive();											// 是否活著
		void Initialize();										// 初始化
		void LoadBitmap();										// 載入圖形
		void OnMove(CGMap *gamemap, CPlayer *player);			// 移動
		void OnShow(float sx, float sy);						// 將圖形貼到畫面
		void SetXY(float nx, float ny);							// 設定圓心的座標
		void SetIsAlive(bool alive);							// 設定是否活著
		float  GetX1();											// 怪物左上角 x 座標
		float  GetY1();											// 怪物左上角 y 座標
		float  GetX2();											// 怪物右下角 x 座標
		float  GetY2();											// 怪物右下角 y 座標
		bool HitRectangle(float tx1, float ty1, float tx2, float ty2);	// 是否碰到參數範圍的矩形
		int GetRandomNum();										// 获得一个1~4区间的随机数
		void SetMovingDown(bool flag);							// 設定是否正在往下移動
		void SetMovingLeft(bool flag);							// 設定是否正在往左移動
		void SetMovingRight(bool flag);							// 設定是否正在往右移動
		void SetMovingUp(bool flag);							// 設定是否正在往上移動
		void SetRandomNum(int num);								// 修改randomnum	
		void ResetAnimation();									// 重置怪物动画
		void SetIsTurn(bool flag);								// 修该isTurn的状态
		void SetDirection();									// 设置怪物方向
		void InitializeMonsterForNext();						// 初始化用于下一关
		bool IsRight(CGMap *gamemap, CPlayer *player);			// 判断能否往右走
		bool IsUp(CGMap *gamemap, CPlayer *player);				// 判断能否往上走
		bool IsDown(CGMap *gamemap, CPlayer *player);			// 判断能否往下走
		bool IsLeft(CGMap *gamemap, CPlayer *player);			// 判断能否往左走
		void changestochastic(int num);							// 改变随机数
		int GetStochastic();									// 获取stochastic
	protected:
		int stochastic;											// 随机数，决定怪物种类
	private:
		CAnimation monsteralive;								// 怪活着的動畫
		CAnimation monsterdead;									// 怪死亡的動畫
		bool is_alive;											// 是否活著
		bool isMoveUp;											// 判断怪物是否向上
		bool isMoveRight;										// 判断怪物是否向右
		bool isMoveLeft;										// 判断怪物是否向左
		bool isMoveDown;										// 判断怪物是否向下
		time_t start;											// start时间戳
		time_t end;												// end时间戳
		float x, y;												// 圓心的座標
		int randomNum;											// 随机数
		bool isTurn;											// 判定怪物是否可以移动
		float SPEED;											// 怪物速度

	};

	class CMonstercoin :public CMonster {
	public:
		CMonstercoin();
	};

}