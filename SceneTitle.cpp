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

static struct sHexSelection { ZL_Point Pos; ticks_t Time; } HexSelection[10];
static ZL_Surface srfLudumDare;
static ZL_Rectf recStart, recHighScores;
static ZL_SynthImcTrack imcTitleMusic;

struct sSceneTitle : public ZL_Scene
{
	sSceneTitle() : ZL_Scene(SCENE_TITLE) { }

	int InitTransitionEnter(ZL_SceneType, void*)
	{
		ZL_Display::PointerX = ZLWIDTH;
		ZL_Display::PointerY = 0;
		memset(HexSelection, 0, sizeof(HexSelection));
		srfLudumDare = ZL_Surface("Data/ludumdare.png").SetDrawOrigin(ZL_Origin::BottomRight);
		imcTitleMusic = ZL_SynthImcTrack(&imcDataTitleMusic).Play().SetSongVolume(0);
		return 400;
	}

	void InitAfterTransition()
	{
		ZL_Display::sigPointerDown.connect(this, &sSceneTitle::OnPointerDown);
		ZL_Display::sigKeyDown.connect(this, &sSceneTitle::OnKeyDown);
	}

	int DeInitTransitionLeave(ZL_SceneType SceneTypeTo)
	{
		ZL_Display::AllSigDisconnect(this);
		return 400;
	}

	void DeInitAfterTransition()
	{
		srfLudumDare = ZL_Surface();
		imcTitleMusic = ZL_SynthImcTrack();
	}

	void OnPointerDown(ZL_PointerPressEvent& e)
	{
		if (recStart.Contains(e)) { imcAccept.Play(); ZL_SceneManager::GoToScene(SCENE_GAME); }
		if (e.y < 30 && e.x < 480) ZL_Application::OpenExternalUrl("https://zillalib.github.io/");
		if (e.y < 30 && e.x > ZLFROMW(400)) ZL_Application::OpenExternalUrl("http://www.ludumdare.com/compo/");
	}

	void OnKeyDown(ZL_KeyboardEvent& e)
	{
		if (e.key == ZLK_ESCAPE) ZL_Application::Quit();
	}

	void Calculate()
	{
		if (HighScores[0].Score)
		{
			recStart = ZL_Rectf(ZLHALFW - 250, ZLHALFH - 80, ZLHALFW + 250, ZLHALFH - 0);
			recHighScores = ZL_Rectf(ZLHALFW - 250, 60, ZLHALFW + 250, ZLHALFH - 100);
		}
		else
		{
			recStart = ZL_Rectf(ZLHALFW - 250, ZLHALFH - 160, ZLHALFW + 250, ZLHALFH - 80);
			recHighScores = ZL_Rectf(0, 0, 0, 0);
		}
	}

	void Draw()
	{
		scalar scale = s(.4)*ZLWIDTH/s(1280);
		ZL_Vector Pos((ZL_Display::PointerX)/scale, (ZL_Display::PointerY)/scale);
		int gridy = ((int)((Pos.y+(Pos.y > 0 ? (HEXH*8/15) : (-HEXH*8/15)))/HEXH));
		ZL_Point Selection(((int)(gridy & 1 ? Pos.x/HEXW+(Pos.x > 0 ? 0 : -1) : (Pos.x+(Pos.x > 0 ? HEXO : -HEXO))/HEXW)), gridy);
		if (HexSelection[0].Pos != Selection)
		{
			HexSelection[0].Time = ZLTICKS;
			for (int i = (10-1); i > 0; i--) { HexSelection[i] = HexSelection[i-1]; if (HexSelection[i].Pos == Selection) HexSelection[i].Time = 0; }
			HexSelection[0].Pos = Selection;
		}

		ZL_Rect view(-1, (int)(ZLHEIGHT/scale/HEXH+1), (int)(ZLWIDTH/scale/HEXW+1), -1);
		ZL_Vector ang = (ZL_Display::PointerPos()-ZLCENTER).Norm();

		ZL_Display::FillGradient(0, 0, ZLWIDTH, ZLHEIGHT, ZLRGB(0,0,0), ZLRGB(0,0,0), ZLRGB(.1,.1,.1), ZLRGB(.1,.1,.1));

		ZL_Display::PushMatrix();
		ZL_Display::Scale(scale);

		srfBlank.BatchRenderBegin(true);
		scalar huebase = 10+(ZLTICKS*s(0.0005));
		for (int y = view.bottom; y <= view.top; y++)
			for (int x = view.left; x <= view.right; x++)
				srfBlank.Draw(x*HEXW+(y&1 ? HEXO : 0), y*HEXH, ZLHSV(0.5+smod(huebase+(-(y-view.bottom/2)*ang.y-(x-view.right/2)*ang.x)*s(0.1),s(0.4)), 1, 1));
		srfBlank.BatchRenderEnd();

		static const unsigned char titlemap[][32] =
		{
			{ 0,8,0,8,  0,  0,8,8,  0,  8,0,8,  0,  8,8,8,  0,  0,8,0,  0,  8,0,0,  0,  8,0,0,  0,  8,8,8 },
			{  8,0,0,8,  0,  8,0,0,  0,  8,8,0,  0,  0,8,0,  0,  0,8,0,  0,  8,0,0,  0,  8,0,0,  8,  0,0,8 },
			{ 0,8,8,8,  0,  8,8,8,  0,  0,8,0,  0,  0,8,0,  0,  0,8,0,  0,  8,0,0,  0,  8,0,0,  0,  8,8,8 },
			{  8,0,0,8,  0,  8,0,0,  0,  8,8,0,  0,  8,0,0,  0,  0,8,0,  0,  8,0,0,  0,  8,0,0,  8,  0,0,8 },
			{ 0,8,0,8,  0,  0,8,8,  0,  8,0,8,  0,  8,8,8,  0,  0,8,0,  0,  8,8,8,  0,  8,8,8,  0,  8,0,8 }
		};

		srfFill.BatchRenderBegin(true);
		for (int tytop = view.top-3-(((view.top-3)&1)?1:0), ty = 0; ty < 5; ty++)
			for (int tx = 0; tx < 32; tx++)
				if (titlemap[ty][tx])
					srfFill.Draw((1+tx)*HEXW+(ty&1 ? HEXO : 0), (tytop-ty)*HEXH, ZLHSV(smod(ZLTICKS*s(0.0005)-ty*s(0.05),1), 1, 1));

		for (sHexSelection* itHexSelection = HexSelection; itHexSelection != &HexSelection[sizeof(HexSelection)/sizeof(HexSelection[0])]; ++itHexSelection)
		{
			if (!itHexSelection->Time) continue;
			scalar f = (ZLTICKS-itHexSelection->Time > 300 ? 1 : s(ZLTICKS-itHexSelection->Time)/s(300));
			if (itHexSelection != HexSelection) f = 1-f;
			srfFill.Draw(itHexSelection->Pos.x*HEXW+(itHexSelection->Pos.y&1 ? HEXO : 0), itHexSelection->Pos.y*HEXH, ZLALPHA(f*.4));
		}
		srfFill.BatchRenderEnd();

		ZL_Display::PopMatrix();


		ZL_Display::DrawRect(recStart.left-1, recStart.low-1, recStart.right+1, recStart.high+1, ZLALPHA(.5), ZLALPHA(.1));
		ZL_Display::DrawRect(recStart, ZLWHITE, ZLALPHA(.1));
		ZL_Display::DrawRect(recStart.left+1, recStart.low+1, recStart.right-1, recStart.high-1, ZLALPHA(.5), ZLALPHA(.1));
		DrawTextBordered(recStart.Center(), "START GAME", s(1.1), ZLBLACK, ZLWHITE, ZL_Origin::Center);

		if (recHighScores.high)
		{
			ZL_Display::DrawRect(recHighScores.left-1, recHighScores.low-1, recHighScores.right+1, recHighScores.high+1, ZLALPHA(.5), ZLALPHA(.1));
			ZL_Display::DrawRect(recHighScores, ZLWHITE, ZLALPHA(.1));
			ZL_Display::DrawRect(recHighScores.left+1, recHighScores.low+1, recHighScores.right-1, recHighScores.high-1, ZLALPHA(.5), ZLALPHA(.1));
			DrawTextBordered(ZL_Vector(recHighScores.MidX(), recHighScores.high-20), "My Highscores", s(.8), ZLBLACK, ZLWHITE, ZL_Origin::Center);
			scalar y = recHighScores.high - 66;
			ZL_Display::FillRect(recHighScores.left + 20, y+26, recHighScores.right - 20, y+24, ZLWHITE);
			for (sHighScore* itHighScore = &HighScores[0]; itHighScore != &HighScores[sizeof(HighScores)/sizeof(HighScores[0])] && itHighScore->Score && y > recHighScores.low; ++itHighScore, y -= 20)
			{
				fntMain.Draw(recHighScores.left + 25, y, ZL_String(itHighScore-HighScores+1), ZL_Origin::BottomCenter);
				fntMain.Draw(recHighScores.left + 33, y, ". Wave:");
				fntMain.Draw(recHighScores.left + 130, y, ZL_String(itHighScore->WaveCount), ZL_Origin::BottomRight);
				fntMain.Draw(recHighScores.left + 180, y, "Enemies:");
				fntMain.Draw(recHighScores.left + 300, y, ZL_String(itHighScore->Kills), ZL_Origin::BottomRight);
				fntMain.Draw(recHighScores.right - 145, y, "Credits:");
				fntMain.Draw(recHighScores.right - 25, y, ZL_String(itHighScore->TotalCredits), ZL_Origin::BottomRight);
			}
		}

		DrawTextBordered(ZL_Vector(18, 18), "(c) 2013 Bernhard Schelling - Nukular Design", s(.8), ZLBLACK, ZLWHITE, ZL_Origin::BottomLeft);

		srfLudumDare.Draw(ZLFROMW(10), 10);
	}

	void DrawTransition(scalar f, bool IsLeaveTransition)
	{
		imcTitleMusic.SetSongVolume(80-(int)(f*80));
		Draw();
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLLUMA(0, f));
	}

	void DrawTextBordered(const ZL_Vector& p, const char* txt, scalar scale, const ZL_Color& colborder, const ZL_Color& colfill, ZL_Origin::Type origin)
	{
		fntBig.Draw(p.x+1, p.y-1, txt, scale,scale, colborder, origin);
		fntBig.Draw(p.x+1, p.y+1, txt, scale,scale, colborder, origin);
		fntBig.Draw(p.x-1, p.y-1, txt, scale,scale, colborder, origin);
		fntBig.Draw(p.x-1, p.y+1, txt, scale,scale, colborder, origin);
		fntBig.Draw(p.x  , p.y  , txt, scale,scale, colfill, origin);
	}

} SceneTitle;
