#ifndef CESWALL_H
#define CESWALL_H
namespace game_framework {

	class CGMap;
	class CESWall
	{
	public:
		CESWall();
		void LoadBitmap();										// 載入圖形
		void Initialize();										// 初始化
		void OnShow(float sx, float sy);						// 將圖形貼到畫面
		void SetXY(float nx, float ny);							// 設定圓心的座標
		void SetIsAlive(bool alive);							// 設定可炸墙块是否活著
		bool GetIsAlive();										// 获取可炸墙块存活状态
		float  GetX1();											// 获取可炸墙块左上角 x 座標
		float  GetY1();											// 获取可炸墙块左上角 y 座標
		float  GetX2();											// 获取可炸墙块右下角 x 座標
		float  GetY2();											// 获取可炸墙块右下角 y 座標
		void ResetAnimation();									// 重置动画张数
		~CESWall();
	private:
		CAnimation eswall_live;									// 可炸墙块活着的動畫
		CAnimation eswall_dead;									// 可炸墙块死亡的動畫
		bool isAlive;											// 是否活著
		float x, y;												// 可炸墙块的座標
	};
}
#endif
