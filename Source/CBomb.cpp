#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBomb.h"
#include "CPlayer.h"

namespace game_framework {

	CBomb::CBomb(): FIRENUM(10)
	{
		x = y = 0;
		btime = 3;
		isSpace = isEnd = isUse = utop = dtop= rtop= ltop = false;
		isAlive = true;
		bomb_level = ubomb_level = dbomb_level = lbomb_level = rbomb_level = 0;
		for (int i = 0; i < FIRENUM; i++)
		{
			up.push_back(new CAnimation());
			down.push_back(new CAnimation());
			right.push_back(new CAnimation());
			left.push_back(new CAnimation());
		}
	}

	void CBomb::OnMove()
	{
		if (isSpace) {
			start = time(NULL);
			isSpace = false;
		}

		end = time(NULL);
		if ((end - start) == btime) {
			isAlive = false;
		}

	}

	void CBomb::OnShow(float sx, float sy)
	{
		int x1 = x;
		int y1 = y;
		if (isAlive) {
			bomb.SetTopLeft(x1-sx, y1-sy);
			bomb.OnMove();
			bomb.OnShow();
			bombing = true;
		}
		if (isAlive == false) {
			if (bombing == true)
				CAudio::Instance()->Play(8);		// ’®µØ±¨’®µƒ“Ù–ß					
			bombing = false;

			if (explode.IsFinalBitmap()) {
				SetIsEnd(true);
				rbomb_level = lbomb_level = ubomb_level = dbomb_level = bomb_level;
				utop = false;
				dtop = false;
				rtop = false;
				ltop = false;
				ResetAnimation();
			}
			else {
				explode.SetTopLeft(x1-sx, y1-sy);
				explode.OnMove();
				explode.OnShow();

				for (int i = 0; i < ubomb_level; i++) {							// …œ÷–≤øª—Êµƒœ‘ æ¬ﬂº≠
					up[i]->SetTopLeft(x1 - sx, y1 - (1 + i) * up[i]->Height() - sy);
					up[i]->OnMove();
					up[i]->OnShow();
				}
				for (int i = 0; i < dbomb_level; i++) {							// œ¬÷–≤øª—Êµƒœ‘ æ¬ﬂº≠
					down[i]->SetTopLeft(x1 -sx, y1 + (1 + i) * down[i]->Height() - sy);
					down[i]->OnMove();
					down[i]->OnShow();
				}
				for (int i = 0; i < lbomb_level; i++) {							// ◊Û÷–≤øª—Êµƒœ‘ æ¬ﬂº≠
					left[i]->SetTopLeft(x1 - (float)((1 + i) * left[i]->Width()) - (int)sx, y1 - (int)sy);
					left[i]->OnMove();
					left[i]->OnShow();
				}
				for (int i = 0; i < rbomb_level; i++) {							// ”“÷–≤øª—Êµƒœ‘ æ¬ﬂº≠
					right[i]->SetTopLeft(x1 + (float)((1 + i) * right[i]->Width()) - (int)sx, y1 - (int)sy);
					right[i]->OnMove();
					right[i]->OnShow();
				}
				if (!utop || ubomb_level != 0) {							    // …œ∂•≤øª—Êµƒœ‘ æ¬ﬂº≠
					uptop.SetTopLeft(x1-sx, y1 - (ubomb_level + 1) * up[0]->Height()-sy);
					uptop.OnMove();
					uptop.OnShow();
				}
				else {

				}
				if (!dtop || dbomb_level != 0) {							    // œ¬∂•≤øª—Êµƒœ‘ æ¬ﬂº≠									
					dowmtop.SetTopLeft(x1-sx, y1 + (dbomb_level + 1) * down[0]->Height()-sy);
					dowmtop.OnMove();
					dowmtop.OnShow();
				}
				else {

				}

				if (!rtop || rbomb_level != 0) {							    // ”“∂•≤øª—Êµƒœ‘ æ¬ﬂº≠					
					righttop.SetTopLeft(x1 +(rbomb_level + 1) * right[0]->Width()-sx, y1-sy);
					righttop.OnMove();
					righttop.OnShow();
				}
				else {

				}

				if (!ltop || lbomb_level != 0) {							    // ◊Û∂•≤øª—Êµƒœ‘ æ¬ﬂº≠					
					lefttop.SetTopLeft(x1 - (lbomb_level + 1) * left[0]->Width()-sx, y1-sy);
					lefttop.OnMove();
					lefttop.OnShow();
				}
				else {

				}
			}
		}
	}

	void CBomb::SetXY(float x, float y)			// ∞—’®µØ∑≈‘⁄◊ÓΩ¸µƒ16*16µƒ∑Ω∏Ò◊Û…œΩ«				
	{
		this->x = x;
		this->y = y;
	}

	void CBomb::ResetAnimation()				// ∂Øª≠÷ÿ÷√	
	{
		bomb.Reset();
		explode.Reset();
		for (int i = 0; i < FIRENUM; i++) {
			up[i]->Reset();
			down[i]->Reset();
			left[i]->Reset();
			right[i]->Reset();
		}
		uptop.Reset();
		dowmtop.Reset();
		righttop.Reset();
		lefttop.Reset();
	}

	bool CBomb::HitRectangleOfBomb(float tx1, float ty1, float tx2, float ty2)	// ’®µØ√Êª˝
	{
		if (isAlive == true && x != 0) {
			float x1 = x;													// ’®µØµƒ◊Û…œΩ«x◊˘òÀ
			float y1 = y;													// ’®µØµƒ◊Û…œΩ«y◊˘òÀ
			float x2 = x + 29;												// ’®µØµƒ”“œ¬Ω«x◊˘òÀ
			float y2 = y + 29;												// ’®µØµƒ”“œ¬Ω«y◊˘òÀ					
			return ((tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2));
		}
		return false;
	}

	bool CBomb::HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2) // ÷˜“™”√”⁄’®π÷
	{
		if (isAlive == false && !isEnd) {
			float x1 = x + 4;												// ’®µØ ˙œÚª—Êµƒ◊Û…œΩ«x◊˘òÀ
			float y1 = y - (ubomb_level + 1) * 32 + 3;						// ’®µØ ˙œÚª—Êµƒ◊Û…œΩ«y◊˘òÀ
			float x2 = x + 28;												// ’®µØ ˙œÚª—Êµƒ”“œ¬Ω«x◊˘òÀ
			float y2 = y + 29 + (dbomb_level + 1) * 32;						// ’®µØ ˙œÚª—Êµƒ”“œ¬Ω«y◊˘òÀ	
			float x_1 = x - (lbomb_level + 1) * 32;							// ’®µØ∫·œÚª—Êµƒ◊Û…œΩ«x◊˘òÀ
			float y_1 = y + 4;												// ’®µØ∫·œÚª—Êµƒ◊Û…œΩ«y◊˘òÀ
			float x_2 = x + 29 + (rbomb_level + 1) * 32;					// ’®µØ∫·œÚª—Êµƒ”“œ¬Ω«x◊˘òÀ
			float y_2 = y + 29;												// ’®µØ∫·œÚª—Êµƒ”“œ¬Ω«y◊˘òÀ
			return ((tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2) || (tx2 >= x_1 && tx1 <= x_2 && ty2 >= y_1 && ty1 <= y_2));
		}
		return false;

	}

	bool CBomb::HitRectangleOfExplosion(float tx1, float ty1, float tx2, float ty2, vector<pair<float, float>> eswalls) // ÷˜“™”√”⁄’®«Ω
	{
		if (isAlive == false && !isEnd) {

			float x1 = x + 4;												// ’®µØ ˙œÚª—Êµƒ◊Û…œΩ«x◊˘òÀ
			float y1 = y - (ubomb_level + 1) * 32 + 3;					// ’®µØ ˙œÚª—Êµƒ◊Û…œΩ«y◊˘òÀ
			float x2 = x + 28;											// ’®µØ ˙œÚª—Êµƒ”“œ¬Ω«x◊˘òÀ
			float y2 = y + 29 + (dbomb_level + 1) * 32;					// ’®µØ ˙œÚª—Êµƒ”“œ¬Ω«y◊˘òÀ	
			float x_1 = x - (lbomb_level + 1) * 32 + 3 ;						// ’®µØ∫·œÚª—Êµƒ◊Û…œΩ«x◊˘òÀ
			float y_1 = y + 4;											// ’®µØ∫·œÚª—Êµƒ◊Û…œΩ«y◊˘òÀ
			float x_2 = x + 29 + (rbomb_level + 1) * 32;					// ’®µØ∫·œÚª—Êµƒ”“œ¬Ω«x◊˘òÀ
			float y_2 = y + 29;											// ’®µØ∫·œÚª—Êµƒ”“œ¬Ω«y◊˘òÀ
			if (rbomb_level == 0 && !rtop && rbomb_level != bomb_level)
				x_2 = x + 32 + 32;
			if (rbomb_level == 0 && rtop && rbomb_level != bomb_level)
				x_2 = x + 32;
			if (lbomb_level == 0 && !ltop && lbomb_level != bomb_level)
				x_1 = x - 29 - 29;
			if (lbomb_level == 0 && !ltop && lbomb_level == bomb_level)
				x_1 = x - 29;
			if (lbomb_level == 0 && ltop && (lbomb_level != bomb_level || lbomb_level == bomb_level))
				x_1 = x - 29;
			if (ubomb_level == 0 && !utop&& ubomb_level != bomb_level)
				y1 = y - 29 - 29;
			if (ubomb_level == 0 && utop&& ubomb_level != bomb_level)
				y1 = y - 29;
			if (dbomb_level == 0 && !dtop&& dbomb_level != bomb_level)
				y2 = y + 32 + 32;
			if (dbomb_level == 0 && dtop&& dbomb_level != bomb_level)
				y2 = y + 32;

			if (ubomb_level > 0 && !utop && ubomb_level != bomb_level)
			{
				for (size_t i = 0; i < eswalls.size(); i++)
				{
					if ((eswalls[i].first + 32 >= x1 && eswalls[i].first <= x2 && eswalls[i].second + 32 >= y1 - 32 && eswalls[i].second <= y)) {
						y1 = y - (ubomb_level + 2) * 32 + 4;
						break;
					}	
				}
			}
			if (dbomb_level > 0 && !dtop && dbomb_level != bomb_level)
			{
				for (size_t i = 0; i < eswalls.size(); i++)
				{
					if ((eswalls[i].first + 32 >= x1 && eswalls[i].first <= x2 && eswalls[i].second + 32 >= y_2 && eswalls[i].second <= y2 + 32)) {
						y2 = y + 29 + (dbomb_level + 2) * 32;
						break;
					}
						
				}
			}
			if (lbomb_level > 0 && !ltop && lbomb_level != bomb_level)
			{
				for (size_t i = 0; i < eswalls.size(); i++)
				{
					if ((eswalls[i].first + 32 >= x_1 - 32 && eswalls[i].first <= x1 && eswalls[i].second + 32 >= y_1 && eswalls[i].second <= y_2)) {
						x_1 = x - (lbomb_level + 2) * 32 + 4;
						break;
					}
						
				}
			}

			if (rbomb_level > 0 && !rtop && rbomb_level != bomb_level)
			{
				for (size_t i = 0; i < eswalls.size(); i++)
				{
					if ((eswalls[i].first + 32 >= x1 && eswalls[i].first <= x_2 + 32 && eswalls[i].second + 32 >= y_1 && eswalls[i].second <= y_2)) {
						x_2 = x + 29 + (rbomb_level + 2) * 32;
						break;
					}
						

				}
			}
			return ((tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2) || (tx2 >= x_1 && tx1 <= x_2 && ty2 >= y_1 && ty1 <= y_2));
		}
		return false;
	}

	void CBomb::SetFire(vector<pair<float, float>> walls)					// …Ë÷√ª—Ê¥Û–°
	{
		if (isAlive == false && !isEnd) {
			for (int i = 0; i <= bomb_level; i++)
			{
				float x1 = x + 26;											// ’®µØ ˙œÚª—Ê…œ∞Î≤ø∑÷µƒ◊Û…œΩ«x◊˘òÀ
				float y1 = y - (i + 1) * 32;								// ’®µØ ˙œÚª—Ê…œ∞Î≤ø∑÷µƒ◊Û…œΩ«y◊˘òÀ
				float x2 = x + 29;											// ’®µØ ˙œÚª—Ê…œ∞Î≤ø∑÷µƒ”“œ¬Ω«x◊˘òÀ
				float y2 = y;												// ’®µØ ˙œÚª—Ê…œ∞Î≤ø∑÷µƒ”“œ¬Ω«y◊˘òÀ	
				if (ubomb_level == bomb_level) {
					for (size_t  j = 0; j < walls.size(); j++)
					{
						if (walls[j].first + 32 >= x1 && walls[j].first <= x2 && walls[j].second + 32 >= y1 && walls[j].second <= y2) {
							ubomb_level = i;
							for (size_t i = 0; i < walls.size(); i++)
								if (walls[i].first + 32 >= x + 4 && walls[i].first <= x + 29 && walls[i].second + 32 >= y - 29 && walls[i].second <= y)
									utop = true;
							break;
						}
					}
				}
				x1 = x + 26;											// ’®µØ ˙œÚª—Êœ¬∞Î≤ø∑÷µƒ◊Û…œΩ«x◊˘òÀ
				y1 = y + 29;											// ’®µØ ˙œÚª—Êœ¬∞Î≤ø∑÷µƒ◊Û…œΩ«y◊˘òÀ
				x2 = x + 29;											// ’®µØ ˙œÚª—Êœ¬∞Î≤ø∑÷µƒ”“œ¬Ω«x◊˘òÀ
				y2 = y + 32 + (i + 1) * 32;								// ’®µØ ˙œÚª—Êœ¬∞Î≤ø∑÷µƒ”“œ¬Ω«y◊˘òÀ	
				if (dbomb_level == bomb_level) {
					for (size_t  k = 0; k < walls.size(); k++)
					{
						if (walls[k].first + 32 >= x1 && walls[k].first <= x2 && walls[k].second + 32 >= y1 && walls[k].second <= y2) {
							dbomb_level = i;
							for (size_t i = 0; i < walls.size(); i++)
								if (walls[i].first + 32 >= x + 4 && walls[i].first <= x + 29 && walls[i].second + 32 >= y + 29  && walls[i].second <= y + 58)
									dtop = true;
							break;
						}
					}
				}
				x1 = x - (i + 1) * 32;									// ’®µØ∫·œÚª—Ê◊Û∞Î≤ø∑÷µƒ◊Û…œΩ«x◊˘òÀ
				y1 = y + 26;											// ’®µØ∫·œÚª—Ê◊Û∞Î≤ø∑÷µƒ◊Û…œΩ«y◊˘òÀ
				x2 = x;													// ’®µØ∫·œÚª—Ê◊Û∞Î≤ø∑÷µƒ”“œ¬Ω«x◊˘òÀ
				y2 = y + 29;											// ’®µØ∫·œÚª—Ê◊Û∞Î≤ø∑÷µƒ”“œ¬Ω«y◊˘òÀ
				if (lbomb_level == bomb_level) {
					for (size_t  m = 0; m < walls.size(); m++)
					{
						if (walls[m].first + 32 >= x1 && walls[m].first <= x2 && walls[m].second + 29 >= y1 && walls[m].second <= y2) {
							lbomb_level = i;
								for (size_t i = 0; i < walls.size(); i++)
									if ((walls[i].first + 32 >= x - 29 && walls[i].first <= x && walls[i].second + 32 >= y + 4 && walls[i].second <= y + 26))
										ltop = true;	
							break;
						}
					}
				}
				x1 = x + 29;											// ’®µØ∫·œÚª—Ê”“∞Î≤ø∑÷µƒ◊Û…œΩ«x◊˘òÀ
				y1 = y + 26;											// ’®µØ∫·œÚª—Ê”“∞Î≤ø∑÷µƒ◊Û…œΩ«y◊˘òÀ
				x2 = x + 32 + (i + 1) * 32;								// ’®µØ∫·œÚª—Ê”“∞Î≤ø∑÷µƒ”“œ¬Ω«x◊˘òÀ
				y2 = y + 29;											// ’®µØ∫·œÚª—Ê”“∞Î≤ø∑÷µƒ”“œ¬Ω«y◊˘òÀ	
				if (rbomb_level == bomb_level) {
					for (size_t  n = 0; n < walls.size(); n++)
					{
						if (walls[n].first + 32 >= x1 && walls[n].first <= x2 && walls[n].second + 29 >= y1 && walls[n].second <= y2) {
							rbomb_level = i;
							for (size_t i = 0; i < walls.size(); i++)
								if (walls[i].first + 32 >= x + 29 && walls[i].first <= (x + 58) && walls[i].second + 32 >= y + 4 && walls[i].second <= y + 29)
									rtop = true;
							break;
						}
					}
				}
			}
		}	

	}

	void CBomb::SetBombLevel(int level)
	{
		if (level <= FIRENUM) {
			ubomb_level = dbomb_level = lbomb_level = rbomb_level = bomb_level =level;
		}
	}

	int CBomb::GetBombLevel()
	{
		return bomb_level;
	}

	void CBomb::SetSpace(bool flag)
	{
		isSpace = flag;
	}

	float CBomb::GetX()
	{
		return x;
	}

	float CBomb::GetY()
	{
		return y;
	}

	bool CBomb::GetIsSpace()
	{
		return isSpace;
	}

	bool CBomb::GetIsAlive()
	{
		return isAlive;
	}

	void CBomb::SetIsAlive(bool flag)
	{
		isAlive = flag;
	}

	void CBomb::SetIsEnd(bool flag)
	{
		isEnd = flag;
	}

	bool CBomb::GetIsEnd()
	{
		return isEnd;
	}

	void CBomb::SetIsUse(bool flag)
	{
		isUse = flag;
	}

	bool CBomb::GetIsUse()
	{
		return isUse;
	}

	void CBomb::SetStart(int time)
	{
		start = time;
	}

	void CBomb::SetEnd(int time)
	{
		end = time;
	}

	void CBomb::LoadBitmap()
	{
		bomb.AddBitmap(IDB_BOMB1, RGB(0, 153, 0));
		bomb.AddBitmap(IDB_BOMB2, RGB(0, 153, 0));
		bomb.AddBitmap(IDB_BOMB3, RGB(0, 153, 0));
		bomb.AddBitmap(IDB_BOMB4, RGB(0, 153, 0));
		explode.AddBitmap(IDB_FIRE4, RGB(183, 185, 184));
		explode.AddBitmap(IDB_FIRE3, RGB(183, 185, 184));
		explode.AddBitmap(IDB_FIRE2, RGB(183, 185, 184));
		explode.AddBitmap(IDB_FIRE1, RGB(183, 185, 184));
		uptop.AddBitmap(IDB_FIREU1, RGB(0, 0, 0));
		uptop.AddBitmap(IDB_FIREU2, RGB(0, 0, 0));
		uptop.AddBitmap(IDB_FIREU3, RGB(0, 0, 0));
		uptop.AddBitmap(IDB_FIREU4, RGB(0, 0, 0));
		righttop.AddBitmap(IDB_FIRER1, RGB(0, 0, 0));
		righttop.AddBitmap(IDB_FIRER2, RGB(0, 0, 0));
		righttop.AddBitmap(IDB_FIRER3, RGB(0, 0, 0));
		righttop.AddBitmap(IDB_FIRER4, RGB(0, 0, 0));
		lefttop.AddBitmap(IDB_FIREL1, RGB(0, 0, 0));
		lefttop.AddBitmap(IDB_FIREL2, RGB(0, 0, 0));
		lefttop.AddBitmap(IDB_FIREL3, RGB(0, 0, 0));
		lefttop.AddBitmap(IDB_FIREL4, RGB(0, 0, 0));
		dowmtop.AddBitmap(IDB_FIRED1, RGB(0, 0, 0));
		dowmtop.AddBitmap(IDB_FIRED2, RGB(0, 0, 0));
		dowmtop.AddBitmap(IDB_FIRED3, RGB(0, 0, 0));
		dowmtop.AddBitmap(IDB_FIRED4, RGB(0, 0, 0));
		for (int i = 0; i < FIRENUM; i++) {
			up[i]->AddBitmap(IDB_FIREUD1, RGB(0, 0, 0));
			up[i]->AddBitmap(IDB_FIREUD2, RGB(0, 0, 0));
			up[i]->AddBitmap(IDB_FIREUD3, RGB(0, 0, 0));
			up[i]->AddBitmap(IDB_FIREUD4, RGB(0, 0, 0));
			down[i]->AddBitmap(IDB_FIREUD1, RGB(0, 0, 0));
			down[i]->AddBitmap(IDB_FIREUD2, RGB(0, 0, 0));
			down[i]->AddBitmap(IDB_FIREUD3, RGB(0, 0, 0));
			down[i]->AddBitmap(IDB_FIREUD4, RGB(0, 0, 0));
			left[i]->AddBitmap(IDB_FIRELR1, RGB(0, 0, 0));
			left[i]->AddBitmap(IDB_FIRELR2, RGB(0, 0, 0));
			left[i]->AddBitmap(IDB_FIRELR3, RGB(0, 0, 0));
			left[i]->AddBitmap(IDB_FIRELR4, RGB(0, 0, 0));
			right[i]->AddBitmap(IDB_FIRELR1, RGB(0, 0, 0));
			right[i]->AddBitmap(IDB_FIRELR2, RGB(0, 0, 0));
			right[i]->AddBitmap(IDB_FIRELR3, RGB(0, 0, 0));
			right[i]->AddBitmap(IDB_FIRELR4, RGB(0, 0, 0));
		}
	}

	CBomb::~CBomb()
	{
		for (int i = 0; i < FIRENUM; i++) {
			delete up[i];
			delete down[i];
			delete left[i];
			delete right[i];
		}
	}
}