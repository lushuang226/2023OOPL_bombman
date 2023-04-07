#ifndef CBOMB_H
#define CBOMB_H
#include <ctime>
#include <vector>
#include <map>
#include <cmath>
namespace game_framework {

	class CBomb {
	public:
		CBomb();										// 初始化
		void LoadBitmap();								// 載入圖形
		void OnMove();									// 移動
		void OnShow(float sx, float sy);				// 將圖形貼到荧幕
		void SetXY(float x, float y);					// 设置炸弹XY坐标
		void SetSpace(bool flag);						// 设置isSpace属性
		float GetX();									// 获取x坐标
		float GetY();									// 获取y坐标
		bool GetIsSpace();								// 获取isSpace属性
		bool GetIsAlive();								// 获取isAlive属性
		void SetIsAlive(bool flag);						// 设置isSpace属性
		void SetIsEnd(bool flag);						// 设置isEnd属性
		bool GetIsEnd();								// 获取isEnd属性
		void ResetAnimation();							// 重置动画张数
		void SetIsUse(bool flag);						// 设置isUse属性
		bool GetIsUse();								// 获取isUse属性
		void SetStart(int time);						// 设置start属性
		void SetEnd(int time);							// 设置end属性
		bool HitRectangleOfBomb(float tx1, float ty1, float tx2, float ty2);											// 是否碰到參數範圍的矩形（调用炸弹里的HitRectangle）
		bool HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2);										// 是否碰到參數範圍的矩形用于火焰
		bool HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2, vector<pair<float, float>> eswalls);	// 是否碰到參數範圍的矩形
		void SetFire(vector<pair<float, float>> walls);																	// 设置火焰长度用于炸弹火焰碰到墙的情况
		void SetBombLevel(int level);																					// 设置炸弹等级
		int GetBombLevel();																								// 获取炸弹等级
		~CBomb();
	private:
		float x, y;					// 圖形座標
		CAnimation bomb;			// 炸弹的动画
		CAnimation explode;			// 爆炸动画
		CAnimation uptop;			// 炸弹上顶部火焰动画
		CAnimation righttop;		// 炸弹右顶部火焰动画
		CAnimation lefttop;			// 炸弹左顶部火焰动画
		CAnimation dowmtop;			// 炸弹下底部火焰
		vector<CAnimation*> up;		// up爆炸动画
		vector<CAnimation*> down;	// down爆炸动画
		vector<CAnimation*> left;	// left爆炸动画
		vector<CAnimation*> right;	// right爆炸动画
		int btime;					// 炸弹持续时间
		bool isSpace;				// 判断人物是否按空格
		bool isUse;					// 判断炸弹的状态是否为被使用状态
		bool isAlive;				// 判断炸弹是否爆炸
		bool isEnd;					// 判断炸弹是否完全爆炸
		time_t start;				// 记录炸弹释放时的时间戳
		time_t end;					// 记录炸弹结束时的时间戳
		bool utop;					// 用于判断炸弹顶部火焰动画能否出现
		bool dtop;					// 用于判断炸弹下低部火焰动画能否出现
		bool rtop;					// 用于判断炸弹右顶部火焰动画能否出现
		bool ltop;					// 用于判断炸弹左顶部火焰动画能否出现
		bool bombing;				// 用于判断炸弹是否爆炸
		const int FIRENUM;			// 设置炸弹火焰等级上限
		int bomb_level;				// 炸弹等级属性
		int ubomb_level;			// 炸弹等级属性
		int dbomb_level;			// 炸弹等级属性
		int lbomb_level;			// 炸弹等级属性
		int rbomb_level;			// 炸弹等级属性
	};

}
#endif

