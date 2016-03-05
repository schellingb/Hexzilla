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

#ifndef _HEXZILLA_WORLD_
#define _HEXZILLA_WORLD_

#include "include.h"

enum eHexType { HEX_NONE, HEX_WALL, HEX_FARM, HEX_TURRET, HEX_SHIELD };
struct sThing { ZL_Vector Pos; ZL_Color Color; scalar Health, MaxHealth; sThing(scalar x, scalar y) : Pos(x, y) { } };
enum eUIShow { UI_CLOSED, UI_BUILDWALL, UI_BUILDBASE, UI_BUILDUPGRADE };

struct sWorld
{
	int Credits, NumBases, NumEnemies;
	int NextWave;
	unsigned int Kills, TotalCredits, TotalBuilds;
	ZL_Vector ScreenCenter;
	scalar Zoom;
	eUIShow UIShow;
	sThing* UIHexThing;
	ZL_Vector UIPos;
	eHexType UIHexType;
	eHexType UIHexTypeHover;
	bool GamePaused, GameOverBases, GameOverCredits;
	ticks_t GameTicks;
	sThing* FirstEnemySpawn;

	void InitGlobal();
	void Init();
	bool Build(eHexType Type);
	bool Calculate();
	bool UITap();
	void Draw(bool UpdateSelection);
};

extern sWorld World;

#endif //_HEXZILLA_WORLD_
