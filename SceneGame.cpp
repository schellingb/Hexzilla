/*
  Hexzilla
  Copyright (C) 2013,2016 Bernhard Schelling

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "include.h"
#include "world.h"

static bool HasFocus = true;
static ZL_Surface srfUIBG, srfUIPopUp, srfUIBaseIcon;
static ZL_TextBuffer txtBases, txtCredits, txtWave, txtKills;
static int txtBasesValue, txtCreditsValue, txtWaveValue;
static unsigned int txtKillsValue;
static scalar ZoomTarget;
static scalar FadeGameOver;
static bool GamePaused;

#ifdef __SMARTPHONE__ //hacked in multi touch support
#define ZL_OPT_DO_IMPLEMENTATION
#include <../Opt/ZL_TouchInput.h>
#endif

static const char* arrHints[] =
{
	"Welcome\n"                          "\n"                               "Click to continue",
	"Use the mouse to control\n"         "Move cursor to side to scroll\n"  "Use mousewheel to zoom",
	"This is your first base, a farm\n"  "Click on a connected field to\n"  "build or upgrade.",
	"To improve a base, chose the \n"    "upgrade option. Upgrade costs\n"  "1$. Downgrades are free.",
	"To build new bases you must\n"      "first build a wall on\n"          "a connected field.",
	"Three base types can be built\n"    "on top of walls",
	"Turrets (red)\n"                    "Shoot at nearby enemies\n"        " Upgrade: Fire rate and range",
	"Farms (green)\n"                    "Generates credits over time\n"    " Upgrade: Generated credits",
	"Shields (blue)\n"                   "Protects nearby bases\n"          " Upgrade: Strength and range",
	"This is an enemy swarm. Over\n"     "time, strength and number\n"      "of enemies will increase.",
	"Good Luck!\n"                       "The goal is to hold out\n"        "as long as possible."
};

enum eHints { HINT_WELCOME, HINT_CONTROLS, HINT_FIRSTBASE, HINT_UPGRADE, HINT_BUILD, HINT_BASETYPES, HINT_BASETURRENT, HINT_BASEFARM, HINT_BASESHIELD, HINT_ENEMY, HINT_GOODLUCK };
static unsigned int HintsShown = 0;
//static unsigned int HintsShown = 0xFFFFFFFF; //debug skip hints

struct sShowUI;
sShowUI *ShowUI = NULL, *ShowUINext = NULL;
struct sShowUI
{
	static float anim;
	bool clickable;
	ZL_Vector pos, pointer;
	void Open(const ZL_Vector& pos = World.UIPos)
	{
		this->pos = pos;
		ZL_Timer::EndTransitionFor(&anim);
		ZL_Timer::AddTransitionFloat(&(anim = 0.01f), 1, 400);
		ShowUI = this;
		clickable = true;
		imcMenu.Play();
	}
	static void Closed()
	{
		ShowUI = NULL;
		GamePaused = false;
		if (ShowUINext) { ShowUINext->Open(); ShowUINext = NULL; }
	}
	void Close()
	{
		if (!clickable) return;
		if (!GamePaused)
		{
			World.UIShow = UI_CLOSED;
			World.UIHexTypeHover = HEX_WALL; //WALL type shows no plan circle
		}
		ZL_Timer::EndTransitionFor(&anim);
		ZL_Timer::AddTransitionFloat(&(anim = 1), 0.01f, 400)->sigDone.connect(&Closed);
		clickable = false;
	}
	virtual void DrawContent() = 0;
	void Draw()
	{
		if (clickable) pointer = ZL_Vector(ZL_Display::PointerX - ZLHALFW - (pos.x - World.ScreenCenter.x)/World.Zoom, ZL_Display::PointerY - ZLHALFH - (pos.y - World.ScreenCenter.y)/World.Zoom);
		ZL_Display::PushMatrix();
		ZL_Display::Translate(ZLHALFW + (pos.x - World.ScreenCenter.x)/World.Zoom, ZLHALFH + (pos.y - World.ScreenCenter.y)/World.Zoom);
		ZL_Display::Rotate(scos(ZLTICKS*s(.001))*s(0.02));
		ZL_Display::Scale(anim+ssin(ZLTICKS*s(.001))*s(0.02));
		DrawContent();
		ZL_Display::PopMatrix();
	}
	virtual void ClickContent() = 0;
	void Click()
	{
		pointer = ZL_Vector(ZL_Display::PointerX - ZLHALFW - (pos.x - World.ScreenCenter.x)/World.Zoom, ZL_Display::PointerY - ZLHALFH - (pos.y - World.ScreenCenter.y)/World.Zoom);
		ClickContent();
	}
};
float sShowUI::anim;

struct sUI_BuildBase : sShowUI
{
	ZL_Rectf recTurret, recFarm, recShield;
	sUI_BuildBase() : recTurret(-200, 30, -60, 177), recFarm(-57, 30, 60, 177), recShield(63, 30, 200, 177) { }
	void DrawContent()
	{
		srfUIPopUp.Draw(0, 0);
		fntBig.Draw(0, 198, "SELECT BASE TO BUILD", ZL_Origin::Center);
		ZL_Display::FillRect(-200, 180, 200, 177, ZLWHITE);
		ZL_Display::FillRect(-60, 178, -57, 30, ZLWHITE);
		ZL_Display::FillRect( 60, 178,  63, 30, ZLWHITE);
		if      (recTurret.Contains(pointer)) { if (clickable) World.UIHexTypeHover = HEX_TURRET; ZL_Display::FillCircle(-125, 120, 60, ZLALPHA(.3)); }
		else if (recFarm.Contains(pointer))   { if (clickable) World.UIHexTypeHover = HEX_FARM; ZL_Display::FillCircle(   0, 120, 60, ZLALPHA(.3));   }
		else if (recShield.Contains(pointer)) { if (clickable) World.UIHexTypeHover = HEX_SHIELD; ZL_Display::FillCircle( 125, 120, 60, ZLALPHA(.3)); }
		else if (clickable)                                   { World.UIHexTypeHover = HEX_NONE; }
		srfFill.Draw(-125, 120, s(.6), s(.6), ZL_Color::Red);
		srfFill.Draw(   0, 120, s(.6), s(.6), ZL_Color::Green);
		srfFill.Draw( 125, 120, s(.6), s(.6), ZL_Color::Blue);
		fntBig.Draw(-125, 120, "1$", ZL_Color::Red, ZL_Origin::Center);
		fntBig.Draw(   0, 120, "1$", ZL_Color::Green, ZL_Origin::Center);
		fntBig.Draw( 125, 120, "1$", ZL_Color::Blue, ZL_Origin::Center);
		fntBig.Draw(-125, 55, "TURRET", ZL_Color::Red, ZL_Origin::Center);
		fntBig.Draw(   0, 55, "FARM", ZL_Color::Green, ZL_Origin::Center);
		fntBig.Draw( 125, 55, "SHIELD", ZL_Color::Blue, ZL_Origin::Center);
	}
	void ClickContent()
	{
		if      (recTurret.Contains(pointer)) { if (!World.Build(HEX_TURRET)) { imcError.Play(); return; } else { imcAccept.Play(); } }
		else if (recFarm.Contains(pointer))   { if (!World.Build(HEX_FARM))   { imcError.Play(); return; } else { imcAccept.Play(); } }
		else if (recShield.Contains(pointer)) { if (!World.Build(HEX_SHIELD)) { imcError.Play(); return; } else { imcAccept.Play(); } }
		else { imcMenu.Play(); }
		Close();
	}
} UI_BuildBase;

struct sUI_BuildUpgrade : sShowUI
{
	ZL_Rectf recUpgrade, recWall;
	sUI_BuildUpgrade() : recUpgrade(-200, 30, -2, 177), recWall(1, 30, 200, 177) { }
	void DrawContent()
	{
		srfUIPopUp.Draw(0, 0);
		fntBig.Draw(0, 198, "SELECT UP/DOWNGRADE", ZL_Origin::Center);
		ZL_Display::FillRect(-200, 180, 200, 177, ZLWHITE);
		ZL_Display::FillRect(-2, 178, 2, 4, ZLWHITE);
		bool maxlev = (World.UIHexType == HEX_NONE), canup = (World.UIHexType != HEX_NONE || !World.UIHexThing || World.UIHexThing->Health < World.UIHexThing->MaxHealth);
		if (canup && recUpgrade.Contains(pointer))   { if (clickable) World.UIHexTypeHover = World.UIHexType; ZL_Display::FillCircle(-100, 120, 60, ZLALPHA(.3)); }
		else if (recWall.Contains(pointer))          { if (clickable) World.UIHexTypeHover = HEX_WALL; ZL_Display::FillCircle( 100, 120, 60, ZLALPHA(.3)); }
		else if (clickable)                                          { World.UIHexTypeHover = HEX_NONE; } //shows current status
		const ZL_Color& itemcol = (World.UIHexThing ? World.UIHexThing->Color : ZL_Color::White);
		srfFill.Draw(-100, 120, s(.6), s(.6), ZLRGBA(itemcol.r, itemcol.g, itemcol.b, (canup ? 1 : s(.4))));
		srfFill.Draw( 100, 120, s(.6), s(.6), ZL_Color::White);
		fntBig.Draw(-100, 120, "1$", itemcol, ZL_Origin::Center);
		fntBig.Draw( 100, 120, "0$", ZL_Color::White, ZL_Origin::Center);
		fntBig.Draw(-100, 55, (maxlev ? (canup ? "REPAIR" : "MAX LEVEL") : "UPGRADE"), itemcol, ZL_Origin::Center);
		fntBig.Draw( 100, 55, "WALL", ZL_Color::White, ZL_Origin::Center);
	}
	void ClickContent()
	{
		if      (recUpgrade.Contains(pointer)) { if (!World.Build(World.UIHexType)) { imcError.Play(); return; } else { imcAccept.Play(); } }
		else if (recWall.Contains(pointer))    { if (!World.Build(HEX_WALL))        { imcError.Play(); return; } else { imcMenu.Play(); } }
		else { imcMenu.Play(); }
		Close();
	}
} UI_BuildUpgrade;

struct sUI_BuildWall : sShowUI
{
	ZL_Rectf recWall;
	sUI_BuildWall() : recWall(-200, 30, 200, 177) {}
	void DrawContent()
	{
		srfUIPopUp.Draw(0, 0);
		fntBig.Draw(0, 198, "SELECT BASE TO BUILD", ZL_Origin::Center);
		ZL_Display::FillRect(-200, 180, 200, 177, ZLWHITE);
		if (recWall.Contains(pointer)) ZL_Display::FillCircle(   0, 120, 60, ZLALPHA(.3));
		srfFill.Draw(   0, 120, s(.6), s(.6), ZL_Color::White);
		fntBig.Draw(   0, 120, "1$", ZL_Color::White, ZL_Origin::Center);
		fntBig.Draw(   0, 55, "WALL", ZL_Color::White, ZL_Origin::Center);
	}
	void ClickContent()
	{
		if (recWall.Contains(pointer)) { if (!World.Build(HEX_WALL))  { imcError.Play(); return; } else { imcAccept.Play(); } }
		else { imcMenu.Play(); }
		Close();
	}
} UI_BuildWall;

struct sUI_Pause : sShowUI
{
	ZL_Rectf recQuit, recContinue;
	sUI_Pause() : recQuit(-200, 105, 200, 177), recContinue(-200, 30, 200, 101) { }
	void DrawContent()
	{
		srfUIPopUp.Draw(0, 0);
		fntBig.Draw(0, 198, "GAME PAUSED", ZL_Origin::Center);
		ZL_Display::FillRect(-200, 180, 200, 177, ZLWHITE);
		ZL_Display::FillRect(-200, 101, 200, 105, ZLWHITE);
		if (recQuit.Contains(pointer))   { ZL_Display::FillCircle(0, recQuit.MidY(), 30, ZLALPHA(.3)); }
		else if (recContinue.Contains(pointer))          { ZL_Display::FillCircle(0, recContinue.MidY(), 30, ZLALPHA(.3)); }
		fntBig.Draw(recQuit.Center(), "QUIT", ZL_Color::White, ZL_Origin::Center);
		fntBig.Draw(recContinue.Center(), "CONTINUE", ZL_Color::White, ZL_Origin::Center);
	}
	void ClickContent()
	{
		if (recQuit.Contains(pointer)) { ZL_SceneManager::GoToScene(SCENE_TITLE); }
		if (recContinue.Contains(pointer)) { }
		imcMenu.Play();
		Close();
	}
} UI_Pause;

struct sUI_Hint : sShowUI
{
	eHints CurrentHint;
	void DrawContent()
	{
		srfUIPopUp.Draw(0, 0);
		fntBig.Draw(0, 198, "HINT", ZL_Origin::Center);
		ZL_Display::FillRect(-200, 180, 200, 177, ZLWHITE);
		fntBig.Draw(-193, 142, arrHints[CurrentHint], ZL_Color::White, ZL_Origin::TopLeft);
	}
	void ClickContent()
	{
		if (CurrentHint == HINT_WELCOME) ShowHint(HINT_CONTROLS);
		else if (CurrentHint == HINT_BASETYPES) ShowHint(HINT_BASETURRENT);
		else if (CurrentHint == HINT_BASETURRENT) ShowHint(HINT_BASEFARM);
		else if (CurrentHint == HINT_BASEFARM) ShowHint(HINT_BASESHIELD);
		else { imcMenu.Play(); Close(); }
	}
	void ShowHint(eHints Hint, const ZL_Vector& pos = World.UIPos)
	{
		CurrentHint = Hint;
		HintsShown |= (1<<Hint);
		GamePaused = true;
		Open(pos);
	}
	void OpenUIWithHint(eHints Hint, sShowUI& UI)
	{
		if (HintsShown & (1<<Hint)) UI.Open();
		else { ShowHint(Hint); ShowUINext = &UI; }
	}
} UI_Hint;

struct sSceneGame : public ZL_Scene
{
	sSceneGame() : ZL_Scene(SCENE_GAME) { }

	void InitGlobal()
	{
		srfUIBG = ZL_Surface("Data/uibg.png");
		srfUIPopUp = ZL_Surface("Data/uipopup.png").SetDrawOrigin(ZL_Origin::BottomCenter);
		srfUIBaseIcon = ZL_Surface("Data/uibaseicon.png");

		//disable hints when a high highscore exists (simple hint disabling for pro player)
		if (HighScores[0].WaveCount > 20) HintsShown = 0xFFFFFFFF;

		//#define __SMARTPHONE__ //DEBUG

		#ifdef __SMARTPHONE__ //hacked in multi touch support
		ZL_TouchInput::Init(false);
		#endif
	}

	int InitTransitionEnter(ZL_SceneType, void*)
	{
		ZL_Display::PointerX = ZLHALFW;
		ZL_Display::PointerY = ZLHALFH;
		World.ScreenCenter = ZL_Vector(0, 0);
		World.Zoom = ZoomTarget = 1;
		World.Init();
		txtBases = ZL_TextBuffer(fntBig, ZL_String(txtBasesValue = World.NumBases));
		txtCredits = ZL_TextBuffer(fntBig, ZL_String(txtCreditsValue = World.Credits));
		txtWave = ZL_TextBuffer(fntBig, ZL_String(txtWaveValue = 0));
		txtKills = ZL_TextBuffer(fntBig, ZL_String(txtKillsValue = World.Kills));
		FadeGameOver = 0;
		GamePaused = false;
		if (!(HintsShown & (1<<HINT_WELCOME))) UI_Hint.ShowHint(HINT_WELCOME);
		return 400;
	}

	void InitAfterTransition()
	{
		ZL_Display::sigKeyDown.connect(this, &sSceneGame::OnKeyDown);
		#ifdef __SMARTPHONE__ //hacked in multi touch support
		ZL_TouchInput::ScrollMode = ZL_TouchInput::SCROLL_SINGULAR;
		ZL_TouchInput::DragStartDistance = 50;
		ZL_TouchInput::sigClick.connect(this, &sSceneGame::OnTouchClick);
		ZL_TouchInput::sigScroll.connect(this, &sSceneGame::OnTouchScroll);
		ZL_TouchInput::sigZoom.connect(this, &sSceneGame::OnTouchZoom);
		#else
		ZL_Display::sigPointerDown.connect(this, &sSceneGame::OnPointerDown);
		ZL_Display::sigMouseWheel.connect(this, &sSceneGame::onMouseWheel);
		ZL_Display::sigActivated.connect(this, &sSceneGame::OnActivated);
		#endif
	}

	int DeInitTransitionLeave(ZL_SceneType SceneTypeTo)
	{
		sHighScore::StoreScore();
		ZL_Display::AllSigDisconnect(this);
		#ifdef __SMARTPHONE__ //hacked in multi touch support
		ZL_TouchInput::AllSigDisconnect(this);
		#endif
		return (World.GameOverBases || World.GameOverCredits ? 1000 : 400);
	}

	void Click()
	{
		if (World.GameOverBases || World.GameOverCredits)
		{
			if (FadeGameOver == 1) ZL_SceneManager::GoToScene(SCENE_TITLE);
			return;
		}
		if (ShowUI && ShowUI->clickable)
		{
			ShowUI->Click();
		}
		else if (!GamePaused)
		{
			if (!World.UITap()) imcError.Play();
			else if (World.UIShow == UI_BUILDWALL)    UI_Hint.OpenUIWithHint(HINT_BUILD, UI_BuildWall);
			else if (World.UIShow == UI_BUILDUPGRADE) UI_Hint.OpenUIWithHint(HINT_UPGRADE, UI_BuildUpgrade);
			else if (World.UIShow == UI_BUILDBASE)    UI_Hint.OpenUIWithHint(HINT_BASETYPES, UI_BuildBase);
		}
	}

	#ifdef __SMARTPHONE__ //hacked in multi touch support
	void OnTouchClick(const ZL_Vector& pnt, ZL_TouchInput::eClickResult&)
	{
		if (!ShowUI && (pnt.x < 185 || pnt.x > ZLFROMW(185)) && (pnt.y < 70 || pnt.y > ZLFROMH(70)))
		{
			GamePaused = true;
			UI_Pause.Open(World.ScreenCenter);
			return;
		}
		Click();
	}

	void OnTouchScroll(const ZL_Vector&, const ZL_Vector& scrollamount)
	{
		if (World.GameOverBases || World.GameOverCredits) return;
		World.ScreenCenter -= scrollamount*World.Zoom;
	}

	void OnTouchZoom(const ZL_Vector&, scalar factor, const ZL_Vector& two_finger_zoom_center)
	{
		if (World.GameOverBases || World.GameOverCredits) return;
		if ((World.Zoom <= 0.5 && factor < 1) || (World.Zoom >= 3 && factor > 1)) return;
		World.ScreenCenter += (two_finger_zoom_center-ZLCENTER)*World.Zoom*(-factor+1);
		World.Zoom *= factor;
	}
	#else
	void OnPointerDown(ZL_PointerPressEvent& e)
	{
		if (e.button == 1) Click();
	}

	void onMouseWheel(ZL_MouseWheelEvent& e)
	{
		if (World.GameOverBases || World.GameOverCredits) return;
		if (e.y < 0 && ZoomTarget < 3) ZoomTarget *= s(1.25);
		if (e.y > 0 && ZoomTarget > 0.5) ZoomTarget *= s(0.8);
	}

	void OnActivated(ZL_WindowActivateEvent& e)
	{
		HasFocus = e.mouse_focus;
	}
	#endif

	void OnKeyDown(ZL_KeyboardEvent& e)
	{
		if (World.GameOverBases || World.GameOverCredits)
		{
			if (FadeGameOver == 1) ZL_SceneManager::GoToScene(SCENE_TITLE);
			return;
		}
		if (e.key == ZLK_ESCAPE)
		{
			if (ShowUI) { ShowUI->Close(); imcMenu.Play(); }
			else
			{
				GamePaused = true;
				UI_Pause.Open(World.ScreenCenter);
			}
		}
	}

	void Calculate()
	{
		if (ShowUI && (ShowUI == &UI_Pause || ShowUI == &UI_Hint))
		{
			World.ScreenCenter.x += ZLELAPSEDF(ShowUI->pos.x-World.ScreenCenter.x)*3;
			World.ScreenCenter.y += ZLELAPSEDF(ShowUI->pos.y-World.ScreenCenter.y)*3;
		}
		#ifndef __SMARTPHONE__
		World.Zoom += ZLELAPSEDF(ZoomTarget-World.Zoom)*3;
		#endif
		if (GamePaused || World.GameOverBases || World.GameOverCredits) return;
		#ifndef __SMARTPHONE__
		if (HasFocus && !ZL_Display::MouseDown[0] && !ZL_Display::MouseDown[1])
		{
			int fx = (int)ZLWIDTH/5, fy = (int)ZLHEIGHT/5;
			if (ZL_Display::MouseDown[3]) { fx *= 4; fy *= 4; } //quicker scroll with RMB
			if (ZL_Display::PointerX < fx) World.ScreenCenter.x -= 3*ZLELAPSEDF(fx-ZL_Display::PointerX)*World.Zoom;
			if (ZL_Display::PointerY < fy) World.ScreenCenter.y -= 3*ZLELAPSEDF(fy-ZL_Display::PointerY)*World.Zoom;
			if (ZL_Display::PointerX > ZLFROMW(fx)) World.ScreenCenter.x += 3*ZLELAPSEDF(ZL_Display::PointerX-ZLFROMW(fx))*World.Zoom;
			if (ZL_Display::PointerY > ZLFROMH(fy)) World.ScreenCenter.y += 3*ZLELAPSEDF(ZL_Display::PointerY-ZLFROMH(fy))*World.Zoom;
		}
		#endif
		if (World.Calculate())
		{
			if (txtBasesValue != World.NumBases) txtBases = ZL_TextBuffer(fntBig, ZL_String(txtBasesValue = World.NumBases));
			if (txtCreditsValue != World.Credits) txtCredits = ZL_TextBuffer(fntBig, ZL_String(txtCreditsValue = World.Credits));
			if (txtWaveValue != World.NextWave-1 && World.NextWave) txtWave = ZL_TextBuffer(fntBig, ZL_String(txtWaveValue = World.NextWave-1));
			if (txtKillsValue != World.Kills) txtKills = ZL_TextBuffer(fntBig, ZL_String(txtKillsValue = World.Kills));
			if (ShowUI && ShowUI->clickable && World.UIShow == UI_CLOSED) ShowUI->Close();
			if (World.GameOverBases || World.GameOverCredits) ZL_Timer::AddTransitionFloat(&FadeGameOver, 1);
			if (!ShowUI && !(HintsShown & (1<<HINT_GOODLUCK)))
			{
				if (World.GameTicks > 500 && !(HintsShown & (1<<HINT_FIRSTBASE))) UI_Hint.ShowHint(HINT_FIRSTBASE, ZL_Vector(0,0));
				else if (World.FirstEnemySpawn && !(HintsShown & (1<<HINT_ENEMY))) UI_Hint.ShowHint(HINT_ENEMY, World.FirstEnemySpawn->Pos);
				else if (HintsShown & (1<<HINT_ENEMY)) UI_Hint.ShowHint(HINT_GOODLUCK, ZL_Vector(0,0));
			}
		}
	}

	void Draw()
	{
		ZL_Display::FillGradient(0, 0, ZLWIDTH, ZLHEIGHT, ZLRGB(0,0,0), ZLRGB(0,0,0), ZLRGB(.1,.1,.1), ZLRGB(.1,.1,.1));
		World.Draw(ShowUI && (ShowUI->clickable || ShowUINext) ? false : true);

		if (ShowUI)
		{
			if (ShowUI == &UI_Pause || ShowUI == &UI_Hint) ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLLUMA(0, sShowUI::anim/2));
			ShowUI->Draw();
		}

		srfUIBG.Draw(ZLFROMW(185), ZLFROMH(50));
		fntBig.Draw(ZLFROMW(185-18-3), ZLFROMH(32+3), "$", ZLBLACK);
		fntBig.Draw(ZLFROMW(185-18), ZLFROMH(32), "$", ZLWHITE);

		txtCredits.Draw(ZLFROMW(90-3), ZLFROMH(32+3), ZLBLACK, ZL_Origin::BottomCenter);
		txtCredits.Draw(ZLFROMW(90), ZLFROMH(32), ZLWHITE, ZL_Origin::BottomCenter);

#ifdef __SMARTPHONE__
		srfUIBG.Draw(s(185.0*1.1), ZLFROMH(50), s(-1.1), 1);
		fntBig.Draw(s(16-3), ZLFROMH(33+3), "| |", ZLBLACK);
		fntBig.Draw(s(16  ), ZLFROMH(33  ), "| |", ZLWHITE);
		srfUIBaseIcon.Draw(s(185.0*1.1-38-3), ZLFROMH(35+3), ZLBLACK);
		srfUIBaseIcon.Draw(s(185.0*1.1-38  ), ZLFROMH(35  ), ZLWHITE);
		txtBases.Draw(s(90.0*1.1-3), ZLFROMH(33+3), ZLBLACK, ZL_Origin::BottomCenter);
		txtBases.Draw(s(90.0*1.1  ), ZLFROMH(33  ), ZLWHITE, ZL_Origin::BottomCenter);
#else
		srfUIBG.Draw(185, ZLFROMH(50), -1, 1);
		srfUIBaseIcon.Draw(s(185-38-3), ZLFROMH(35+3), ZLBLACK);
		srfUIBaseIcon.Draw(s(185-38  ), ZLFROMH(35  ), ZLWHITE);
		txtBases.Draw(s(90-3), ZLFROMH(33+3), ZLBLACK, ZL_Origin::BottomCenter);
		txtBases.Draw(s(90), ZLFROMH(33), ZLWHITE, ZL_Origin::BottomCenter);
#endif

		srfUIBG.Draw(ZLFROMW(185), 50, 1, -1);
		srfEnemy.Draw(ZLFROMW(185-30-3), s(21+3),      s(0.6),s(0.6), ZLBLACK);
		fntBig.Draw(  ZLFROMW(185-20-3), s( 9+3), "X", s(1.1),s(1.1), ZLBLACK);
		srfEnemy.Draw(ZLFROMW(185-30  ), s(21  ),      s(0.6),s(0.6), ZLWHITE);
		fntBig.Draw(  ZLFROMW(185-20  ), s( 9  ), "X", s(1.1),s(1.1), ZLWHITE);
		txtKills.Draw(ZLFROMW(90-3), s(11+3), ZLBLACK, ZL_Origin::BottomCenter);
		txtKills.Draw(ZLFROMW(90  ), s(11  ), ZLWHITE, ZL_Origin::BottomCenter);

		srfUIBG.Draw(185, 50, -1, -1);
		fntBig.Draw(s(185-32-3), s(11+3), "~", ZLBLACK);
		fntBig.Draw(s(185-32  ), s(11  ), "~", ZLWHITE);
		txtWave.Draw(s(90-3), s(11+3), ZLBLACK, ZL_Origin::BottomCenter);
		txtWave.Draw(s(90  ), s(11  ), ZLWHITE, ZL_Origin::BottomCenter);

		if (World.GameOverBases || World.GameOverCredits)
		{
			ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLLUMA(0, FadeGameOver/2));
			ZL_Color col = ZLALPHA(FadeGameOver);
			fntBig.Draw(ZLHALFW, ZLHALFH+150, "G A M E   O V E R", 2, 2, col, ZL_Origin::Center);
			ZL_Display::FillRect(ZLHALFW-300, ZLHALFH+100, ZLHALFW+300, ZLHALFH+104, col);
			fntBig.Draw(ZLHALFW, ZLHALFH-150, (World.GameOverBases ? "NO MORE BASES LEFT" : "NO MORE CREDITS LEFT"), 1, 1, col, ZL_Origin::Center);
		}
	}
} SceneGame;
