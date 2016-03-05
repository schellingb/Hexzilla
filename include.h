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

#ifndef _HEXZILLA_INCLUDE_
#define _HEXZILLA_INCLUDE_

#include <ZL_Application.h>
#include <ZL_Display.h>
#include <ZL_Surface.h>
#include <ZL_Signal.h>
#include <ZL_Audio.h>
#include <ZL_Font.h>
#include <ZL_Scene.h>
#include <ZL_Timer.h>
#include <ZL_Particles.h>
#include <ZL_Math.h>
#include <ZL_Data.h>
#include <ZL_Network.h>
#include <ZL_SynthImc.h>

#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

//simple sample sound pool helper class
struct ZL_SynthImcSamplePool
{
	ZL_SynthImcTrack tracks[8];
	ZL_SynthImcSamplePool(TImcSongData *songdata) : trackidx(0), lastplay(0) { for (int i = 0; i < 8; i++) tracks[i] = ZL_SynthImcTrack(songdata, false); }
	void Play() { if (lastplay+68 >= ZLTICKS) return; tracks[trackidx].Play(true); if (++trackidx == 8) trackidx = 0; lastplay = ZLTICKS; }
	private:unsigned char trackidx;ticks_t lastplay;
};

//global resources
extern ZL_Font fntMain, fntBig;
extern ZL_Surface srfFill, srfBlank, srfEnemy;
extern ZL_SynthImcSamplePool imcPlayerLaser, imcEnemyLaser;
extern ZL_SynthImcTrack imcEnemyExplosion, imcPlayerExplosion, imcError, imcAccept, imcMenu;
extern TImcSongData imcDataTitleMusic;

#define SCENE_TITLE 1
#define SCENE_GAME 2

#define HEXO s(47)
#define HEXW s(95)
#define HEXH s(110)

//global data
struct sHighScore
{
	unsigned int Score, WaveCount, Kills, TotalCredits, TotalBuilds;
	static void StoreScore();
	static void LoadScores();
};

extern sHighScore HighScores[10];


#endif //_HEXZILLA_INCLUDE_
