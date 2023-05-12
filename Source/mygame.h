#include "CPlayer.h"
#include "CBomb.h"
#include "CMonster.h"
#include "CGMap.h"
#include <ctime>
#include "CMenu.h"
#pragma once
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {										// 定義各種音效的編號
		AUDIO_DING,										// 0,撞击声
		AUDIO_LAKE,										// 1,进入下一关的音乐
		AUDIO_NTUT,										// 2,游戏时背景音乐
		AUDIO_TITLE,									// 3,标题背景音乐
		AUDIO_WIN,										// 4,闯关成功
		AUDIO_LOSE,										// 5,闯关失败
		AUDIO_EAT,										// 6,吃道具
		AUDIO_MOUSE,									// 7,光标
		AUDIO_BOMB,										// 8,炸弹爆炸
		AUDIO_SETBOMB,									// 9,放置炸弹

	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 键盘按下的动作
		void OnKeyDown(UINT, UINT, UINT);				// 键盘放开的动作
		static void SetSuccess();						// 通关成功
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void OnMove();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// 游戏的logo
		CMenu menu;										// 选单class
		static bool IsSuccess;							// 判断是否通关
		int statusNum = 0;
		bool isShow;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);				// 键盘按下的动作
		void OnKeyUp(UINT, UINT, UINT);					// 键盘放开的动作		
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		void CutOff();									// 切关时的参数配置
		void BumpedMonster(CMonster *monster);			// 碰怪方法
		void BlowUpMonsterLogic(CMonster *monster);		// 炸怪方法
		void SetMonsterCoordinates(CMonster *monster);	// 给怪坐标
		time_t			begintimes;						// 起始时间
		time_t			endtimes;						// 结束时间
		time_t			begin;							// 起始时间，用于切关音乐
		const int		NUMBALLS;						// 球的總數
		int				NUMMONSTERS;					// 怪物的總數
		vector<CInteger*>		mytime;					// [0]为生命，[01]为倒计时时间，[2]为分数
		CMovingBitmap	background;						// 背景圖
		CMovingBitmap	help;							// 說明圖
		CMovingBitmap	logo_time;						// TIME单字图形	
		CMovingBitmap	logo_left;						// LEFT单字图形
		CMonster		*monster;						// 怪物
		CMonstercoin	*monstercoin;					// 怪物
		CPlayer			player;							// 人物
		CGMap		    gamemap;						// 地图
		bool			holdl;							// 判断是否一直按着左方向键
		bool			holdr;							// 判断是否一直按着右方向键
		bool			holdu;							// 判断是否一直按着上方向键
		bool			holdd;							// 判断是否一直按着下方向键
		bool			changing;						// 为true时代表正在切图
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
		void OnKeyUp(UINT, UINT, UINT);					// 键盘放开的动作		
		static void SetSuccess();						// 通关成功
		static void SetFail();							// 通关失败
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;									// 倒數之計數器
		static bool IsSuccess;							// 判断是否通关
		CMovingBitmap over;								// 通关失败的logo
		CMovingBitmap pass;								// 通关成功的logo
	};
}