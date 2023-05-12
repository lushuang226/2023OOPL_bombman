#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "CGMap.h"
#include <ctime>
#include <algorithm>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////
	bool CGameStateInit::IsSuccess = false;

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		ShowInitProgress(0);	// 一開始的loading進度為0%

		logo.LoadBitmap(IDB_TITLE);
		menu.LoadBitmap();
	}

	void CGameStateInit::OnBeginState()
	{
		menu.Initialize(0);
		if (IsSuccess == false)
			CAudio::Instance()->Load(AUDIO_TITLE, "sounds\\Title_Screen.mp3");	// 标题背景音乐

		CAudio::Instance()->Play(AUDIO_TITLE, false);
		CGameStateInit::SetSuccess();						// 闯关成功
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_ESC = 27;
		const char KEY_ENTER = 0xD;

		if (nChar == KEY_ENTER && menu.getStatusNum() == 0)
		{
			CAudio::Instance()->Stop(AUDIO_TITLE);
			GotoGameState(GAME_STATE_RUN);
		}

		if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲

		if (nChar == KEY_UP)
			menu.setMovingUp(false);
		menu.setFlag(0);

		if (nChar == KEY_DOWN)
			menu.setMovingDown(false);
		menu.setFlag(0);

		if (nChar == KEY_LEFT)
			menu.setIsLeft(false);
	}

	void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_ESC = 27;
		const char KEY_ENTER = 0xD;

		if (nChar == KEY_UP)
			if (menu.getWay() == 1)
				menu.setMovingUp(true);

		if (nChar == KEY_DOWN)
			if (menu.getWay() == 1)
				menu.setMovingDown(true);

		if (nChar == KEY_ENTER)
			menu.setIsSpace(true);

		if (nChar == KEY_LEFT)
			menu.setIsLeft(true);
	}

	void CGameStateInit::SetSuccess()
	{
		IsSuccess = true;
	}

	void CGameStateInit::OnMove()
	{
		menu.OnMove();
	}

	void CGameStateInit::OnShow()
	{
		logo.SetTopLeft(95, 25);
		logo.ShowBitmap();
		menu.OnShow();
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	bool CGameStateOver::IsSuccess = false;

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
	}

	void CGameStateOver::OnBeginState()
	{
		if (IsSuccess) {									// 根据情况播放成功或失败音乐
			CAudio::Instance()->Play(AUDIO_WIN, true);
		}
		else {
			CAudio::Instance()->Play(AUDIO_LOSE, true);
		}
	}

	void CGameStateOver::OnInit()
	{
		over.LoadBitmap(IDB_GAMEOVER);			// 载入游戏结束的画面
		pass.LoadBitmap(IDB_GAMEPASS);
		CAudio::Instance()->Load(AUDIO_WIN, "sounds\\Invincibility.mp3");	// 闯关成功音乐
		CAudio::Instance()->Load(AUDIO_LOSE, "sounds\\Game_Over.mp3");		// 闯关失败音乐
	}

	void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ENTER = 0xD;
		if (nChar == KEY_ENTER)					// 按回车返回主界面
		{
			CAudio::Instance()->Stop(AUDIO_WIN);
			CAudio::Instance()->Stop(AUDIO_LOSE);
			GotoGameState(GAME_STATE_INIT);
		}
	}

	void CGameStateOver::SetSuccess()
	{
		IsSuccess = true;
	}

	void CGameStateOver::SetFail()
	{
		IsSuccess = false;
	}

	void CGameStateOver::OnShow()
	{
		// 结束游戏时的画面
		if (IsSuccess) {
			pass.SetTopLeft(0, 0);		// 闯关成功
			pass.ShowBitmap();
		}
		else {
			over.SetTopLeft(0, 0);		// 闯关失败
			over.ShowBitmap();
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡=====================
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g), NUMBALLS(28)
	{
		mytime.push_back(new CInteger());				//生命
		mytime.push_back(new CInteger());				//倒计时
		mytime.push_back(new CInteger());				//分数
		mytime[2]->Change(5);							//分数改为5位数
		NUMMONSTERS = gamemap.GetMonsterNum();
		monster = new CMonster[NUMMONSTERS];
		monstercoin = new CMonstercoin[NUMMONSTERS];
		holdu = holdd = holdl = holdr = false;
	}

	CGameStateRun::~CGameStateRun()
	{
		delete[] monster;
		delete[] monstercoin;
		for (int i = 0; i < 3; i++)
			delete mytime[i];
	}

	void CGameStateRun::OnBeginState()
	{
		gamemap.SetStart(time(NULL));						// 初始化时间
		gamemap.InitializeForRestart();						// 重置关卡
		gamemap.ClearnCorrdinates();						// 清空坐标
		gamemap.SetLevel(1);								// 设置关卡
		gamemap.SwitchingMap();								// 换图
		gamemap.OnMove();									// 赋坐标
		gamemap.SetSX(0.0);									// 设置视窗坐标

		for (int i = 0; i < NUMMONSTERS; i++) {
			monster[i].InitializeMonsterForNext();
			monstercoin[i].InitializeMonsterForNext();
		}

		begintimes = time(NULL);
		begin = time(NULL);
		changing = false;
		holdu = holdd = holdl = holdr = false;
		player.Initialize();								// 人物初始化
		player.InitializeBomb();							// 人物携带的炸弹初始化
		help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
		logo_left.SetTopLeft(420, 20);
		logo_time.SetTopLeft(20, 20);
		mytime[0]->SetInteger(3);							// 指定生命
		mytime[0]->SetTopLeft(520, 20);						// 指定生命的座標
		mytime[1]->SetInteger(184);							// 指定倒计时时间
		mytime[1]->SetTopLeft(120, 20);						// 指定倒计时时间的座標
		mytime[2]->SetInteger(0);							// 指定分数
		mytime[2]->SetTopLeft(260, 20);						// 指定分数的座標
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		gamemap.MovingScreen(&player);
		endtimes = time(NULL);			// 倒计时
		if (endtimes - begintimes >= 1) {
			mytime[1]->Add(-1);
			begintimes = endtimes;
		}
		// 计时四秒用于播放切关音乐
		if (changing) {
			if (endtimes - begin >= 4) {
				CAudio::Instance()->Stop(AUDIO_LAKE);
				CAudio::Instance()->Play(AUDIO_NTUT, true);
				changing = false;
			}
		}
		// 碰到门之后的逻辑
		if (gamemap.HitRectangleOfProps(&player, &gamemap, monster)) {
			CutOff();
		}

		if (gamemap.HitRectangleOfSpeed(&player)) {		// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfBombnum(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfBombpow(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfMissAll(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfCrossBomb(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfControl(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfCrossWall(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}
		if (gamemap.HitRectangleOfMissFire(&player)) {	// 道具加分数
			mytime[2]->Add(500);
			CAudio::Instance()->Play(AUDIO_EAT);
		}

		gamemap.OnMove();								// gamemapOnMove

		if (mytime[1]->GetInteger() <= 0 || (gamemap.GetLevel() == 13)) {	// 时间结束或达到指定关卡时
			gamemap.SetIsCutFigure(true);			// 启动切关图片
			CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI

			if (gamemap.GetLevel() % 5 == 0) {						// 在奖励关时间结束则进入下一关
				changing = true;
				if (changing)
					begin = time(NULL);
				CAudio::Instance()->Play(AUDIO_LAKE, false);		// 开始音乐
				gamemap.ChangePass(&player, &gamemap, true);			// 切关
				gamemap.SetSX(0.0);									// 初始化视窗窗口
				mytime[1]->SetInteger(184);							// 指定倒计时时间
			}
			else if (gamemap.GetLevel() == 13) {					// 在进入第x关时结束游戏 EX:为2时则在第一关碰门后触发
				CGameStateOver::SetSuccess();						// 闯关成功
				CAudio::Instance()->Stop(AUDIO_LAKE);				// 停止音乐
				CAudio::Instance()->Stop(AUDIO_NTUT);				// 停止音乐
				GotoGameState(GAME_STATE_OVER);
			}
			else {													// 在普通关死亡则游戏结束
				CGameStateOver::SetFail();							// 闯关失败
				CAudio::Instance()->Stop(AUDIO_LAKE);				// 停止音乐
				CAudio::Instance()->Stop(AUDIO_NTUT);				// 停止音乐
				GotoGameState(GAME_STATE_OVER);
			}
		}

		BumpedMonster(monster);
		BumpedMonster(monstercoin);

		if (player.GetIsFinal() && player.GetIsAlive() == false) {
			player.SetXY(32.0, 96.0);
			player.SetIsAlive(true);
			gamemap.SetSX(0.0);
			player.ResetAnimation();
			mytime[0]->Add(-1);
			if (mytime[0]->GetInteger() <= 0) {			// 生命为0	
				gamemap.SetIsCutFigure(true);			// 启动切关图片
				CGameStateOver::SetFail();				// 闯关失败
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止音乐
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止音乐
				GotoGameState(GAME_STATE_OVER);
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//===============================================人物碰撞检测逻辑================================================
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (gamemap.GetIsCutFigure() == false) {
			if (holdr && !(gamemap.HitRectangle(player.GetX1() + 8, player.GetY1() + 3, player.GetX2() + 4, player.GetY2(), &player)))
				player.SetMovingRight(true);
			if (holdl && !(gamemap.HitRectangle(player.GetX1(), player.GetY1() + 3, player.GetX2() - 5, player.GetY2() - 0, &player)))
				player.SetMovingLeft(true);
			if (holdd && !(gamemap.HitRectangle(player.GetX1() + 3, player.GetY1() + 3, player.GetX2() - 1, player.GetY2() + 2, &player)))
				player.SetMovingDown(true);
			if (holdu && !(gamemap.HitRectangle(player.GetX1() + 3, player.GetY1() - 2, player.GetX2() - 1, player.GetY2() - 3, &player)))
				player.SetMovingUp(true);
		}
		else {
			holdr = holdl = holdd = holdu = false;
		}


		player.OnMove(&gamemap, &player);
		player.Control();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// ======================================炸弹砟墙逻辑================================================
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		vector<pair<float, float>> we = gamemap.GetCoordinatesOfTheWall();
		player.SetBombFlames(gamemap.GetCoordinatesOfAllWalls());
		player.SerBombXY(gamemap.GetCoordinatesOfclearing(), gamemap.GetCoordinatesOfAllWalls()); // 给炸弹坐标
		for (int i = 0; i < we.size(); i++) {
			if (player.HitRectangleOfExplosion(we[i].first, we[i].second, we[i].first + 32, we[i].second + 32, we)) {
				gamemap.SetIsAliveByXY(we[i].first, we[i].second, false);
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ============================================================怪物碰撞移动逻辑=================================================
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (gamemap.GetLevel() % 5 == 0) {
			SetMonsterCoordinates(monstercoin);
			BlowUpMonsterLogic(monstercoin);
		}
		else {
			SetMonsterCoordinates(monster);
			BlowUpMonsterLogic(monster);
		}

		for (int i = 0; i < NUMMONSTERS; i++) {
			if (gamemap.GetLevel() % 5 == 0) {
				monstercoin[i].OnMove(&gamemap, &player);
			}
			else {
				monster[i].OnMove(&gamemap, &player);
			}
		}
	}

	void CGameStateRun::OnInit()  							// 遊戲的初值及圖形設定
	{	///////////////////////////////////////////////////////////////
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		// 等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		////////////////////////////////////////////////////////////////
		ShowInitProgress(33);								// 接個前一個狀態的進度，此處進度視為33%

		// 開始載入資料
		for (int i = 0; i < NUMMONSTERS; i++)
		{
			monster[i].changestochastic(i);		// 改变怪物种类
			monster[i].LoadBitmap();			// 載入第i個monster的圖形
			monstercoin[i].LoadBitmap();		// 載入第i個monster的圖形
		}

		player.LoadBitmap();
		ShowInitProgress(66);
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));			//載入說明的圖形
		logo_left.LoadBitmap(IDB_LEFT);
		logo_time.LoadBitmap(IDB_TIME);
		mytime[0]->LoadBitmap();							//載入生命的圖形
		mytime[1]->LoadBitmap();							//載入倒计时的圖形	
		mytime[2]->LoadBitmap();							//載入分数的圖形	
		gamemap.LoadBitmap();								//载入地图的地形
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\hit.mp3");			// 撞击声
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\Stage_Complete.mp3");// 进入下一关的音乐
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\Stage_Theme.mp3");	// 背景音乐
		CAudio::Instance()->Load(AUDIO_EAT, "sounds\\EAT.mp3");			// 背景音乐
		CAudio::Instance()->Load(AUDIO_BOMB, "sounds\\bomb.mp3");			// 炸弹爆炸的音效
		CAudio::Instance()->Load(AUDIO_SETBOMB, "sounds\\setbomb.mp3");				// 放置炸弹的音效	

	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard左箭頭
		const char KEY_UP = 0x26;		// keyboard上箭頭
		const char KEY_RIGHT = 0x27;	// keyboard右箭頭
		const char KEY_DOWN = 0x28;		// keyboard下箭頭
		const char KEY_SPACE = 0x20;	// keyboard空格键
		const char KEY_Z = 0x5A;		// 切关界面结束才能移动
		const char KEY_R = 0x52;
		if (gamemap.GetIsCutFigure() == false) {
			if (nChar == KEY_LEFT) {
				player.SetMovingLeft(true);
				holdl = true;
			}
			if (nChar == KEY_RIGHT) {
				player.SetMovingRight(true);
				holdr = true;
			}
			if (nChar == KEY_UP) {
				player.SetMovingUp(true);
				holdu = true;
			}
			if (nChar == KEY_DOWN) {
				player.SetMovingDown(true);
				holdd = true;
			}
			if (player.GetIsAlive() == true) {
				if (nChar == KEY_SPACE)
					player.SetIsSpace(false);
			}
			if (nChar == KEY_Z) {
				player.SetControl(true);
			}
			if (nChar == KEY_R) {
				gamemap.ChangePass(&player, &gamemap, true);
				CutOff();
			}
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭頭
		const char KEY_UP = 0x26; // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28; // keyboard下箭頭
		const char KEY_SPACE = 0x20; // keyboard空格键
		if (gamemap.GetIsCutFigure() == false) {	// 切关界面结束才能移动
			if (nChar == KEY_LEFT) {
				player.SetMovingLeft(false);
				holdl = false;
			}
			if (nChar == KEY_RIGHT) {
				player.SetMovingRight(false);
				holdr = false;
			}
			if (nChar == KEY_UP) {
				player.SetMovingUp(false);
				holdu = false;
			}
			if (nChar == KEY_DOWN) {
				player.SetMovingDown(false);
				holdd = false;
			}
			if (player.GetIsAlive() == true) {
				if (nChar == KEY_SPACE)
					player.SetIsSpace(true);
			}
		}
	}

	void CGameStateRun::OnShow()
	{

		///////////////////////////////////////////////////////////////////////////////
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//////////////////////////////////////////////////////////////////////////////

		help.ShowBitmap();									// 貼上說明圖
		gamemap.OnShow();									// 贴上地图
		if (gamemap.GetIsCutFigure() == false) {
			for (int i = 0; i < NUMMONSTERS; i++)
				if (gamemap.GetLevel() % 5 == 0) {
					monstercoin[i].OnShow(gamemap.GetSX(), gamemap.GetSY());	// 貼上第i號monster
				}
				else {
					monster[i].OnShow(gamemap.GetSX(), gamemap.GetSY());	// 貼上第i號monster
				}
			player.OnShow(&gamemap);									// 贴上人物
			mytime[0]->ShowBitmap();							// 贴上生命
			mytime[1]->ShowBitmap();							// 贴上倒计时
			mytime[2]->ShowBitmap();							// 贴上分数
			logo_left.ShowBitmap();
			logo_time.ShowBitmap();
		}
	}

	void CGameStateRun::CutOff()
	{
		// 触发切关音乐
		changing = true;
		if (changing)
			begin = time(NULL);
		CAudio::Instance()->Play(AUDIO_LAKE, false);			// 开始音乐
		CAudio::Instance()->Stop(AUDIO_NTUT);					// 停止音乐
		if (gamemap.GetLevel() == 13) {							// 在进入第x关时结束游戏 EX:为2时则在第一关碰门后触发
			CGameStateOver::SetSuccess();						// 闯关成功
			CAudio::Instance()->Stop(AUDIO_LAKE);				// 停止音乐
			CAudio::Instance()->Stop(AUDIO_NTUT);				// 停止音乐
			GotoGameState(GAME_STATE_OVER);
		}
		else {
			for (int i = 0; i < NUMMONSTERS; i++) {
				if (gamemap.GetLevel() % 5 == 0) {
					monstercoin[i].InitializeMonsterForNext();
				}
				else {
					monster[i].InitializeMonsterForNext();
				}
			}
			mytime[0]->SetInteger(3);								// 指定生命
			mytime[0]->SetTopLeft(520, 20);							// 指定生命的座標
			mytime[1]->SetTopLeft(120, 20);							// 指定倒计时时间的座標
			mytime[2]->Add(1500);									// 加分数
			mytime[2]->SetTopLeft(260, 20);							// 指定分数的座標

			if (gamemap.GetLevel() % 5 == 0) {						// 检测是否为奖励关
				mytime[1]->SetInteger(24);							// 指定倒计时时间
				player.SetMissAll(true);							// 无敌
				player.SetBombControl(true);
			}
			else {
				mytime[1]->SetInteger(184);							// 指定倒计时时间
			}
		}
	}

	void CGameStateRun::BumpedMonster(CMonster *monster)
	{
		for (int i = 0; i < NUMMONSTERS; i++) {		// 碰到怪物之后的逻辑
			if (monster[i].IsAlive() && monster[i].HitEraser(&player) && gamemap.GetLevel() % 5 != 0 && monster[i].GetStochastic() != 889) {
				monster[i].SetIsAlive(false);
				mytime[2]->Add(10);				// 一只怪10分
				if (player.GetMissAll() == false) {
					holdr = holdu = holdl = holdd = false;
					player.SetMovingRight(false);
					player.SetMovingLeft(false);
					player.SetMovingDown(false);
					player.SetMovingUp(false);
					player.SetIsAlive(false);
					player.ResetAnimation();
				}
				CAudio::Instance()->Play(AUDIO_DING);
			}
		}
	}

	void CGameStateRun::BlowUpMonsterLogic(CMonster * monster)
	{
		for (int i = 0; i < NUMMONSTERS; i++) { // 炸怪逻辑
			if (player.HitRectangleOfExplosion(monster[i].GetX1(), monster[i].GetY1(), monster[i].GetX2(), monster[i].GetY2()) && monster[i].IsAlive()) {
				monster[i].SetIsAlive(false);
				monster[i].SetMovingDown(false);
				monster[i].SetMovingUp(false);
				monster[i].SetMovingLeft(false);
				monster[i].SetMovingRight(false);
				mytime[2]->Add(10);									// 加分数10
				CAudio::Instance()->Play(AUDIO_DING);
			}
		}
	}

	void CGameStateRun::SetMonsterCoordinates(CMonster * monster)
	{
		for (int i = 0; i < NUMMONSTERS; i++) // 設定monster的起始座標
			if (monster[i].GetX1() == 0 && monster[i].GetY1() == 0)
				monster[i].SetXY(gamemap.GetCoordinatesOfTheMonster()[i].first, gamemap.GetCoordinatesOfTheMonster()[i].second);
	}
}