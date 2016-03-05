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

ZL_Font fntMain, fntBig;
sHighScore HighScores[10];

struct sHexzilla : public ZL_Application
{
	sHexzilla() : ZL_Application(60) { }

	virtual void Load(int argc, char *argv[])
	{
		if (!LoadReleaseDesktopDataBundle()) return;
		if (!ZL_Display::Init("Hexzilla", 1280, 720, ZL_DISPLAY_ALLOWRESIZEHORIZONTAL)) return;
		ZL_Display::ClearFill(ZL_Color::White);
		ZL_Display::SetAA(true);
		ZL_Audio::Init();
		ZL_Timer::Init();
		ZL_Application::SettingsInit("Hexzilla");
		sHighScore::LoadScores();

		fntMain = ZL_Font("Data/fntMain.png");
		fntBig = ZL_Font("Data/typomoderno.ttf.zip", 40);

		World.InitGlobal();

		ZL_SceneManager::Init(SCENE_TITLE);
	}
} Hexzilla;

void sHighScore::StoreScore()
{
	if (World.NextWave <= 2) return;
	sHighScore cur = { 0, (unsigned int)(World.NextWave-1), World.Kills, World.TotalCredits, World.TotalBuilds };
	cur.Score = (cur.WaveCount * 10) + (cur.Kills) + (cur.TotalCredits);
	for (sHighScore* itHighScore = &HighScores[0]; itHighScore != &HighScores[(sizeof(HighScores)/sizeof(HighScores[0]))]; ++itHighScore)
	{
		if (itHighScore->Score > cur.Score) continue;
		for (sHighScore* itHighScoreShift = &HighScores[(sizeof(HighScores)/sizeof(HighScores[0]))-1]; itHighScoreShift != itHighScore; itHighScoreShift--)
			*itHighScoreShift = *(itHighScoreShift-1);
		*itHighScore = cur;
		ZL_Application::SettingsSet("HIGHSCORES", ZL_Base64::Encode(&HighScores, sizeof(HighScores)));
		ZL_Application::SettingsSynchronize();
		return;
	}
}

void sHighScore::LoadScores()
{
	ZL_String scoresBase64 = ZL_Application::SettingsGet("HIGHSCORES");
	if (!scoresBase64.length() || !ZL_Base64::Decode(scoresBase64, &HighScores, sizeof(HighScores)))
		memset(HighScores, 0, sizeof(HighScores));
}
