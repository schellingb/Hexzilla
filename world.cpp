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

#include "world.h"

sWorld World;
ZL_Surface srfFill, srfBlank, srfEnemy;
static ZL_Surface srfLaser, srfShield;

#define SELECTIONFADE 300

static const ZL_Color colGrid(0, 1, 1, s(.5));
struct sHex;
struct sEnemy;
static vector<sHex*> Hexes;
static vector<sEnemy*> Enemies;
static ZL_ParticleEffect particleSpark;
static ticks_t TimeNextSpawn;
static struct sHexSelection { ZL_Point Pos; ticks_t Time; } HexSelection[10];

static struct sSwarmSelection
{
	ZL_Point Pos;
	ticks_t Time, TimeIn;
	void Draw()
	{
		bool in = World.GameTicks-TimeIn < SELECTIONFADE;
		scalar f = s(World.GameTicks-(in ? TimeIn : Time))/s(SELECTIONFADE);
		srfFill.Draw(Pos.x*HEXW+(Pos.y&1 ? HEXO : 0), Pos.y*HEXH, ZLRGBA(1, .5, .6, (in ? f*.2 : .2-f*.2)));
	}
} SwarmSelection[20];

static struct sLaser
{
	const sThing *From, *To;
	ZL_Vector FromOff, ToOff;
	ticks_t TimeUntil;
	sLaser(const sThing* From, const sThing* To, const ZL_Vector& FromOff, const ZL_Vector& ToOff) : From(From), To(To), FromOff(FromOff), ToOff(ToOff), TimeUntil(World.GameTicks+300) { }
	sLaser() : TimeUntil(0) { }
	void Draw()
	{
		ZL_Vector p = From->Pos+FromOff;
		ZL_Vector v = (To->Pos+ToOff)-=p;
		scalar f = s(World.GameTicks+300-TimeUntil)/s(300);
		//Debug: ZL_Display::DrawLine(From->Pos+FromOff, To->Pos+ToOff, ZLRGBA(1,0,0, 1-(sabs(f-s(0.5))*2)));
		scalar a = 1-sabs(f-s(0.5))*2;
		if (f > 0.5) p += v*(f*2-1);
		srfLaser.Draw(p, v.GetAngle(), v.GetLength()/srfLaser.GetWidth()*a, s(.1), ZLRGBA(From->Color.r, From->Color.g, From->Color.b, a));
	}
} Lasers[10], *LaserIndex = Lasers;

static inline ZL_Point GetGrid(const ZL_Vector Pos)
{
	int y = ((int)((Pos.y+(Pos.y > 0 ? (HEXH*8/15) : (-HEXH*8/15)))/HEXH));
	return ZL_Point(((int)(y & 1 ? Pos.x/HEXW+(Pos.x > 0 ? 0 : -1) : (Pos.x+(Pos.x > 0 ? HEXO : -HEXO))/HEXW)), y);
}

struct sHex : sThing
{
	eHexType Type;
	ZL_Point PosGrid;
	ticks_t TimeBuildStart, TimeBuildEnd;
	ticks_t LastAction, ActionDuration;
	unsigned int Level, MaxLevel;
	scalar Scale;
	sHex(eHexType Type, int x, int y) : sThing(x*HEXW+(y&1 ? HEXO : 0), y*HEXH), PosGrid(x, y), LastAction(0), Level(0)
	{
		SetType(Type);
	}
	void SetType(eHexType NewType)
	{
		if (NewType != HEX_NONE) //NONE is only repair
		{
			if (NewType == HEX_WALL) Color = ZLLUM(.7);
			if (NewType == HEX_FARM) Color = ZL_Color::Green;
			if (NewType == HEX_TURRET) Color = ZL_Color::Red;
			if (NewType == HEX_SHIELD) Color = ZL_Color::Blue;
			scalar f =RAND_RANGE(.7, 1);
			Color.r *= f;
			Color.g *= f;
			Color.b *= f;
			Level = (Type == HEX_WALL || NewType == HEX_WALL ? 1 : Level + 1);
			Type = NewType;
		}
		TimeBuildStart = World.GameTicks;
		TimeBuildEnd = World.GameTicks + 500 + 250*Level;
		Scale = 0;
		MaxHealth = Health = GetLevelHealth();
		if (Type == HEX_TURRET) ActionDuration = GetTurretFireRate();
		if (Type == HEX_FARM)   ActionDuration = GetFarmRate();
	}
	static inline scalar GetTurretRange(int ForLevel);
	inline scalar GetTurretDamage();
	static inline scalar GetShieldRange(int ForLevel);
	inline scalar GetLevelHealth();
	inline ticks_t GetTurretFireRate();
	inline ticks_t GetFarmRate();
	inline bool IsMaxLevel();
	void Calculate();
	scalar GetRadius()
	{
		return (Type != HEX_SHIELD || World.GameTicks < TimeBuildEnd ? HEXW/2 : GetShieldRange(Level));
	}
	bool ShieldCovers(sHex* b)
	{
		return Pos.Near(b->Pos, (GetRadius() + 5 - b->GetRadius()));
	}
	void Draw()
	{
		srfFill.Draw(Pos, Scale, Scale, Color);
	}
	void DrawOverlay()
	{
		if (Type != HEX_WALL)
		{
			//Draw Level
			fntBig.Draw(Pos.x, Pos.y - (HEXH/2)*Scale, ZL_String(Level), s(.5)*Scale, Color, ZL_Origin::Center);
		}
		if (Type == HEX_FARM && LastAction && LastAction+ActionDuration > World.GameTicks)
		{
			//Draw Farming +1
			scalar f = Scale*s(World.GameTicks-LastAction)/s(500);
			fntBig.Draw(Pos.x, Pos.y+30*f, ZL_String("+")<<Level, ZLRGBA(Color.r, Color.g, Color.b, 1-f), ZL_Origin::Center);
		}
		//DEBUG: for (int i = 0; i < 200; i++) ZL_Display::FillCircle(Pos+RandomOffset(), 2, ZL_Color::White);
		//DEBUG: ZL_Display::DrawCircle(Pos, GetRadius(), ZLWHITE, ZLALPHA(.3));
		if (Type == HEX_SHIELD)
		{
			if (World.GameTicks < TimeBuildEnd) return;
			scalar sc = GetShieldRange(Level)/s((256/2)-2);
			srfShield.Draw(Pos, sc, sc, ZLRGBA(0,0,1,Scale+0.2));
			//DEBUG: ZL_Display::DrawCircle(Pos, sc*128, ZLWHITE, ZLALPHA(.3));
		}
		if (World.UIHexThing == this)
		{
			scalar plan;
			const ZL_Color *col = &Color;
			if      (World.UIHexTypeHover == HEX_SHIELD) { plan = GetShieldRange(Type == HEX_SHIELD ? Level + 1 : 1)*(s(.9)+scos(World.GameTicks*s(.006))*s(0.1)); col = &ZL_Color::Blue; }
			else if (World.UIHexTypeHover == HEX_TURRET) { plan = GetTurretRange(Type == HEX_TURRET ? Level + 1 : 1)*(s(.9)+scos(World.GameTicks*s(.006))*s(0.1)); col = &ZL_Color::Red;  }
			else if (World.UIHexTypeHover != HEX_NONE)     plan = 0;
			else if (Type                 == HEX_SHIELD)   plan = GetShieldRange(Level)*(s(0.95)+scos(World.GameTicks*s(.004))*s(0.05));
			else if (Type                 == HEX_TURRET)   plan = GetTurretRange(Level)*(s(0.95)+scos(World.GameTicks*s(.004))*s(0.05));
			else                                           plan = 0;
			if (plan) ZL_Display::DrawCircle(Pos, plan, *col, ZLRGBA(col->r, col->g, col->b, .2));
		}
	}
	void Damage(scalar dmg)
	{
		if (!Health) return;
		Health -= dmg;
		if (Health <= 0)
		{
			imcPlayerExplosion.Play();
			Health = 0;
			World.NumBases--;
			if (World.UIHexThing == this) World.UIShow = UI_CLOSED;
			else if (World.UIShow != UI_CLOSED && GetNeighborCount(HexSelection[0].Pos) == 0) World.UIShow = UI_CLOSED;
		}
	}
	ZL_Vector RandomOffset()
	{
		const scalar h3 = HEXH*s(.3);
		scalar y = RAND_RANGE(-h3*2, h3*2);
		scalar x = RAND_RANGE(-HEXW*s(.48), HEXW*s(.48)) * (y > h3 ? ((h3*2)-y)/h3 : (y < -h3 ? ((h3*2)+y)/h3 : 1));
		return ZL_Vector(x*Scale, y*Scale);
	}
	static bool Sort(const sHex* a, const sHex* b)
	{
		return (a->PosGrid.y == b->PosGrid.y ? a->PosGrid.x < b->PosGrid.x : a->PosGrid.y > b->PosGrid.y);
	}
	static sHex* Get(ZL_Point PosGrid)
	{
		for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex) if (PosGrid == (*itHex)->PosGrid) return *itHex;
		return NULL;
	}
	static int GetNeighborCount(ZL_Point PosGrid)
	{
		int c = 0;
		for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex)
		{
			if ((*itHex)->Health == 0 || (*itHex)->TimeBuildEnd > World.GameTicks) continue;
			ZL_Point other = (*itHex)->PosGrid;
			if  ((other.x != PosGrid.x || other.y != PosGrid.y) &&
			      ((other.y == PosGrid.y && other.x >= PosGrid.x-1 && other.x <= PosGrid.x+1) ||
			       (other.x == PosGrid.x && other.y >= PosGrid.y-1 && other.y <= PosGrid.y+1) ||
			       (other.x == PosGrid.x+(PosGrid.y&1?1:-1) && other.y >= PosGrid.y-1 && other.y <= PosGrid.y+1))) c++;
		}
		return c;
	}
};

struct sEnemy : sThing
{
	ZL_Vector Vel;
	scalar Speed;
	sEnemy* Follow;
	sHex* Target;
	ticks_t LastFire;
	unsigned int Level;
	sEnemy(ZL_Vector Pos, sEnemy *Follow = NULL) : sThing(Pos.x, Pos.y), Speed(Follow ? Follow->Speed + RAND_RANGE(30,50) : 100), Follow(Follow), Target(NULL), LastFire(0), Level(World.NextWave)
	{
		Health = MaxHealth = GetLevelHealth();
		if (!Follow) Speed *= GetLevelSpeedFactor();
		Color = ZLRGB(1,.5,.6);
		World.NumEnemies++;
	}
	inline scalar GetLevelHealth();
	inline scalar GetLevelSpeedFactor();
	inline scalar GetLevelDamage();
	inline scalar GetLevelRange();
	void Calculate();
	void Draw()
	{
		if (!Health) return;
		//ZL_Display::DrawCircle(Pos, 5, ZLWHITE, (Follow ? ZL_Color::Blue : ZL_Color::Red));
		scalar size = (Follow ? s(.4) : s(.55));
		scalar ang = Vel.GetAngle();
		srfEnemy.Draw(Pos.x + 4, Pos.y - 4, ang, size*s(1.3), size*s(1.3), ZLLUMA(0, .5));
		srfEnemy.Draw(Pos, ang, size, size, Color);
	}
	void Damage(scalar dmg)
	{
		if (!Health) return;
		Health -= dmg;
		if (Health <= 0)
		{
			World.NumEnemies--;
			World.Kills++;
			imcEnemyExplosion.Play();
			particleSpark.Spawn(20, Pos, 0, 3, 3);
			Health = 0;
			for (vector<sEnemy*>::iterator itEnemy = Enemies.begin(); itEnemy != Enemies.end(); ++itEnemy)
				if ((*itEnemy)->Follow == this) (*itEnemy)->Follow = Follow;
		}
	}
	sHex* GetTarget()
	{
		sHex *closest = NULL;
		scalar closestdist = S_MAX;
		bool hasshields = false;
		for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex)
		{
			if (!(*itHex)->Health) continue;
			scalar dist = (Pos.GetDistance((*itHex)->Pos)) - ((*itHex)->GetRadius());
			if (!closest || dist < closestdist) { closest = *itHex; closestdist = dist; }
			if ((*itHex)->Type == HEX_SHIELD && !hasshields) hasshields = true;
		}
		if (closest && hasshields && closest->Type != HEX_SHIELD)
		{
			for (vector<sHex*>::iterator itHexShield = Hexes.begin(); itHexShield != Hexes.end(); ++itHexShield)
				if ((*itHexShield)->Health && (*itHexShield)->Type == HEX_SHIELD && (*itHexShield)->ShieldCovers(closest))
					return *itHexShield;
		}
		return closest;
	}
};

void SpawnSwarm(ZL_Vector Pos, int num)
{
	ZL_LOG2("ENEMY", "Sending wave %d: with %d new enemies", World.NextWave, num);
	//remove dead enemies
	for (vector<sEnemy*>::iterator itEnemy = Enemies.begin(); itEnemy != Enemies.end();)
	{
		if ((*itEnemy)->Health) { ++itEnemy; continue; }
		if (World.FirstEnemySpawn == *itEnemy) World.FirstEnemySpawn = NULL;
		bool hasenemyref = false;
		for (sLaser* itLaser = Lasers; itLaser != &Lasers[sizeof(Lasers)/sizeof(Lasers[0])]; ++itLaser)
			if (itLaser->From == *itEnemy || itLaser->To == *itEnemy) { hasenemyref = true; break; }
		if (!hasenemyref) itEnemy = Enemies.erase(itEnemy);
		else ++itEnemy;
	}

	int sub = (num-1)/3;
	sEnemy *lead = new sEnemy(ZL_Vector(Pos.x+RAND_RANGE(-5,5), Pos.y+RAND_RANGE(-5,5)), NULL);
	Enemies.push_back(lead);
	for (int i = 0; num && i < sub; i++)
	{
		sEnemy *sublead = new sEnemy(ZL_Vector(Pos.x+RAND_RANGE(-5,5), Pos.y+RAND_RANGE(-5,5)), lead);
		Enemies.push_back(sublead);
		num--;
		for (int ii = 0; num && ii < 3; ii++, num--) Enemies.push_back(new sEnemy(ZL_Vector(Pos.x+RAND_RANGE(-5,5), Pos.y+RAND_RANGE(-5,5)), sublead));
	}
	if (!World.FirstEnemySpawn) World.FirstEnemySpawn = lead;
}

//BALANCING STUFF
inline ticks_t       sHex::GetFarmRate()                { return 1500 + (2000*Level); } //will give Level $ each Action
inline scalar        sHex::GetLevelHealth()             { return s(50)+(Level*s(15))*(Type == HEX_SHIELD ? s(2) : s(1)); }
inline scalar        sHex::GetTurretRange(int ForLevel) { return (2+ForLevel)*HEXH*s(.4); }
inline scalar        sHex::GetTurretDamage()            { return 10; }
inline ticks_t       sHex::GetTurretFireRate()          { return 600 - (Level*50); } //Fire rate
inline scalar        sHex::GetShieldRange(int ForLevel) { return (2+ForLevel)*HEXH*s(.5); }
inline bool          sHex::IsMaxLevel()                 { return (Level >= (unsigned int)(Type == HEX_FARM ? 3 : 5)); }
inline scalar      sEnemy::GetLevelHealth()             { return 0.5f+((Level>>1)*0.2f); }
inline scalar      sEnemy::GetLevelSpeedFactor()        { return 1.0f+((Level>>1)*0.05f); }
inline scalar      sEnemy::GetLevelDamage()             { return 1.0f+((Level>>1)*0.2f); }
inline scalar      sEnemy::GetLevelRange()              { return 100.0f; }

void NextSpawn()
{
	ZL_Rectf recPlayer(Hexes.front()->Pos.x, Hexes.front()->Pos.y, Hexes.front()->Pos.x, Hexes.front()->Pos.y);
	for (vector<sHex*>::iterator itHexPlayer = Hexes.begin(); itHexPlayer != Hexes.end(); ++itHexPlayer)
		if ((*itHexPlayer)->Health)
		{
			if      ((*itHexPlayer)->Pos.x < recPlayer.left)   recPlayer.left = (*itHexPlayer)->Pos.x;
			else if ((*itHexPlayer)->Pos.x > recPlayer.right)  recPlayer.right = (*itHexPlayer)->Pos.x;
			if      ((*itHexPlayer)->Pos.y > recPlayer.high)   recPlayer.high = (*itHexPlayer)->Pos.y;
			else if ((*itHexPlayer)->Pos.y < recPlayer.low)    recPlayer.low = (*itHexPlayer)->Pos.y;
		}

	ZL_Vector spawnpos;
	int spawndir = 1+(rand()%8);
	if (spawndir > 4) spawndir++;
	scalar spawnlength = RAND_RANGE(HEXH*10, HEXH*20)*s(spawndir&1 ? .707 : 1);
	switch (spawndir)
	{
		case 1: spawnpos = ZL_Vector(recPlayer.left - spawnlength, recPlayer.low - spawnlength); break;
		case 2: spawnpos = ZL_Vector(recPlayer.MidX(), recPlayer.low - spawnlength); break;
		case 3: spawnpos = ZL_Vector(recPlayer.right + spawnlength, recPlayer.low - spawnlength); break;
		case 4: spawnpos = ZL_Vector(recPlayer.left - spawnlength, recPlayer.MidY()); break;
		case 6: spawnpos = ZL_Vector(recPlayer.right + spawnlength, recPlayer.MidY()); break;
		case 7: spawnpos = ZL_Vector(recPlayer.left - spawnlength, recPlayer.high + spawnlength); break;
		case 8: spawnpos = ZL_Vector(recPlayer.MidX(), recPlayer.high + spawnlength); break;
		case 9: spawnpos = ZL_Vector(recPlayer.right + spawnlength, recPlayer.high + spawnlength); break;
	}

	World.NextWave++;
	if      (World.NextWave ==   1) {                                                TimeNextSpawn += 5000;  }
	else if (World.NextWave <=   4) { SpawnSwarm(spawnpos,  5);                      TimeNextSpawn += 6000;  }
	else if (World.NextWave <=   8) { SpawnSwarm(spawnpos, 13);                      TimeNextSpawn += 10000; }
	else if (World.NextWave <=  10) { SpawnSwarm(spawnpos,  5);                      TimeNextSpawn += 2000;  }
	else if (World.NextWave <=  11) { SpawnSwarm(spawnpos, 19);                      TimeNextSpawn += 2000;  }
	else if (World.NextWave <=  19) { SpawnSwarm(spawnpos,  5);                      TimeNextSpawn += 1000;  }
	else if (World.NextWave <=  25) { SpawnSwarm(spawnpos, 13);                      TimeNextSpawn += 5000;  }
	else if (World.NextWave <= 500) { SpawnSwarm(spawnpos,  5+(World.NextWave/100)); TimeNextSpawn += 2200-World.NextWave; }
	else                            { SpawnSwarm(spawnpos,  5+(World.NextWave/100)); TimeNextSpawn += 1700;  }
}

void sHex::Calculate()
{
	Scale = (World.GameTicks > TimeBuildEnd ? 1 : s(World.GameTicks-TimeBuildStart) / s(TimeBuildEnd-TimeBuildStart));
	if (Health < MaxHealth) Scale *= Health/MaxHealth;

	if (World.GameTicks < TimeBuildEnd) return;
	if (Type == HEX_TURRET && World.GameTicks > LastAction + ActionDuration && World.GameTicks > TimeBuildEnd + ActionDuration)
	{
		sEnemy* Target = NULL;
		for (vector<sEnemy*>::iterator itEnemy = Enemies.begin(); itEnemy != Enemies.end(); ++itEnemy)
			if ((*itEnemy)->Health && Pos.Near((*itEnemy)->Pos, GetTurretRange(Level))) { Target = *itEnemy; break; }
		if (Target)
		{
			imcPlayerLaser.Play();
			Target->Damage(GetTurretDamage());
			*LaserIndex = sLaser(this, Target, ZL_Vector(), ZL_Vector());
			if (++LaserIndex == &Lasers[sizeof(Lasers)/sizeof(Lasers[0])]) LaserIndex = Lasers;
			LastAction = World.GameTicks;
		}
		else LastAction += 50;
	}
	else if (Type == HEX_FARM && World.GameTicks > LastAction + ActionDuration && World.GameTicks > TimeBuildEnd + ActionDuration)
	{
		World.Credits += Level;
		World.TotalCredits += Level;
		LastAction = World.GameTicks;
	}
}

void sEnemy::Calculate()
{
	if (World.GameTicks > LastFire + 1000)
	{
		if (Follow == NULL) Target = GetTarget();
		sHex* FireTarget = Target;
		for (sEnemy* pFollow = Follow; pFollow; pFollow = pFollow->Follow) FireTarget = pFollow->Target;
		if (FireTarget && FireTarget->Health && Pos.Near(FireTarget->Pos, GetLevelRange()))
		{
			imcEnemyLaser.Play();
			FireTarget->Damage(GetLevelDamage());
			*LaserIndex = sLaser(this, FireTarget, ZL_Vector(), FireTarget->RandomOffset());
			if (++LaserIndex == &Lasers[sizeof(Lasers)/sizeof(Lasers[0])]) LaserIndex = Lasers;
			LastFire = World.GameTicks;
		}
		else LastFire += 50;
	}
	else if (Follow == NULL && (Target == NULL || Target->Health == 0)) Target = GetTarget();
	ZL_Vector dir = ((Follow ? Follow->Pos : (Target ? Target->Pos : ZL_Vector(Pos.x+RAND_VARIATION(.5), Pos.y+RAND_VARIATION(.5))))-Pos).Norm();
	dir.x += RAND_VARIATION(.5);
	dir.y += RAND_VARIATION(.5);
	Vel = (Vel+dir*ZLELAPSEDF(4)).Norm();
	Pos += Vel*Speed*ZLELAPSED;

	int i;
	ZL_Point Grid = GetGrid(Pos);
	for (i = 0; i < 20; i++) { if (SwarmSelection[i].Pos == Grid) { if (SwarmSelection[i].Time != World.GameTicks) { SwarmSelection[i].Time = World.GameTicks; } return; } }
	for (i = 0; i < 20; i++) { if (SwarmSelection[i].Time < World.GameTicks) { SwarmSelection[i].Pos = Grid; SwarmSelection[i].Time = SwarmSelection[i].TimeIn = World.GameTicks; return; } }
}

static int FarmsLeft()
{
	int c = 0;
	for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex)
		if ((*itHex)->Health && (*itHex)->Type == HEX_FARM) c++;
	return c;
}

static void UpdateSelection()
{
	ZL_Point Selection = GetGrid(ZL_Vector(World.ScreenCenter.x + (-ZLHALFW+ZL_Display::PointerX)*World.Zoom, World.ScreenCenter.y + (-ZLHALFH+ZL_Display::PointerY)*World.Zoom));
	if (HexSelection[0].Pos != Selection)
	{
		HexSelection[0].Time = World.GameTicks;
		for (int i = (10-1); i > 0; i--) { HexSelection[i] = HexSelection[i-1]; if (HexSelection[i].Pos == Selection) HexSelection[i].Time = 0; }
		HexSelection[0].Pos = Selection;
	}
}

void sWorld::InitGlobal()
{
	srfBlank = ZL_Surface("Data/blank.png").SetDrawOrigin(ZL_Origin::Center);
	srfFill = ZL_Surface("Data/fill.png").SetDrawOrigin(ZL_Origin::Center);
	srfLaser = ZL_Surface("Data/laser.png").SetDrawOrigin(ZL_Origin::CenterLeft).SetRotateOrigin(ZL_Origin::CenterLeft);
	srfEnemy = ZL_Surface("Data/enemy.png").SetDrawOrigin(ZL_Origin::Center);
	srfShield = ZL_Surface("Data/shield.png").SetDrawOrigin(ZL_Origin::Center);
	particleSpark = ZL_ParticleEffect(400, 200);
	particleSpark.AddParticleImage(ZL_Surface("Data/spark.png").SetColor(ZLRGB(1, .5, .6)), 100);
	particleSpark.AddBehavior(new ZL_ParticleBehavior_LinearMove(30, 20, 0, PI2, true));
	particleSpark.AddBehavior(new ZL_ParticleBehavior_LinearImageProperties());
}

void sWorld::Init()
{
	memset(HexSelection, 0, sizeof(HexSelection));
	memset(SwarmSelection, 0, sizeof(SwarmSelection));
	for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex) delete *itHex;
	Hexes.clear();
	for (vector<sEnemy*>::iterator itEnemy = Enemies.begin(); itEnemy != Enemies.end(); ++itEnemy) delete *itEnemy;
	Enemies.clear();

	GameOverBases = GameOverCredits = false;

	NextWave = 0;
	GameTicks = TimeNextSpawn = 0;

	Hexes.push_back(new sHex(HEX_FARM, 0, 0));
	//Hexes.push_back(new sHex(HEX_TURRET, 2, 1));

	TotalBuilds = NumBases = (int)Hexes.size();
	TotalCredits = Credits = 1;
	Kills = 0;
}

bool sWorld::Calculate()
{
	GameTicks += ZLELAPSEDTICKS;
	if (GameTicks > TimeNextSpawn) NextSpawn();

	for (vector<sEnemy*>::iterator itEnemy = Enemies.begin(); itEnemy != Enemies.end(); ++itEnemy) if ((*itEnemy)->Health) (*itEnemy)->Calculate();
	for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex) if ((*itHex)->Health) (*itHex)->Calculate();

	if (NumBases == 0)
	{
		GameOverBases = true;
		UIShow = UI_CLOSED;
	}
	else if (Credits == 0 && FarmsLeft() == 0)
	{
		GameOverCredits = true;
		UIShow = UI_CLOSED;
	}
	return true;
}

bool sWorld::UITap()
{
	//if (UIShow != UI_CLOSED) return false; //already showing ui for another hex
	UpdateSelection();
	sHex *hex = sHex::Get(HexSelection[0].Pos);
	if (hex && !hex->Health) hex = NULL;
	if (!hex && sHex::GetNeighborCount(HexSelection[0].Pos) == 0) return false;
	if (hex && GameTicks < hex->TimeBuildEnd) return false;
	UIHexType = (hex ? (hex->IsMaxLevel() ? HEX_NONE : hex->Type) : HEX_NONE);
	UIHexThing = hex;
	UIPos = ZL_Vector(HexSelection[0].Pos.x*HEXW+(HexSelection[0].Pos.y&1 ? HEXO : 0), HexSelection[0].Pos.y*HEXH);
	UIShow = (hex ? (hex->Type == HEX_WALL ? UI_BUILDBASE : UI_BUILDUPGRADE) : UI_BUILDWALL);
	return true;
}

bool sWorld::Build(eHexType Type)
{
	sHex *hex = sHex::Get(HexSelection[0].Pos);
	if (!hex || !hex->Health)
	{
		if (Type != HEX_WALL) return false; //must build wall on empty space
		if (Credits < 1) return false; //must have credits to build wall
		if (Credits == 1 && FarmsLeft() == 0) return false; //cannot build a new wall with last credit when no farms left
		if (sHex::GetNeighborCount(HexSelection[0].Pos) == 0) return false; //must build next to other base
		if (hex)
		{
			//build new wall over dead hex
			hex->SetType(HEX_WALL);
		}
		else
		{
			//build new wall
			Hexes.push_back(new sHex(HEX_WALL, HexSelection[0].Pos.x, HexSelection[0].Pos.y));
			sort(Hexes.begin(), Hexes.end(), sHex::Sort);
		}
		Credits--; NumBases++;
	}
	else
	{
		if (Type == HEX_WALL)
		{
			if (hex->Type == HEX_WALL) return false; //cannot downgrade wall itself
			if (Credits < 1 && hex->Type == HEX_FARM && FarmsLeft() == 1) return false; //cannot downgrade last farm
		}
		else
		{
			if (Credits < 1) return false; //must have credits to upgrade
			if (Type != HEX_FARM && Credits == 1 && FarmsLeft() == 0) return false; //cannot build anything but farm with last credit when no farms left
			if (hex->Health == hex->MaxHealth && hex->IsMaxLevel()) return false; //max level no repair needed
		}

		if      (Type == HEX_WALL) { hex->SetType(HEX_WALL); } //free downgrade
		else if (Type == HEX_NONE) { hex->SetType(HEX_NONE); Credits--; } //repair at maxlevel
		else                       { hex->SetType(Type); Credits--; } //upgrade from wall or same type
	}
	TotalBuilds++;
	return true;
}

void sWorld::Draw(bool DoUpdateSelection)
{
	scalar w = ZLWIDTH*Zoom*HALF, h = ZLHEIGHT*Zoom*HALF;
	ZL_Rectf ortho(ScreenCenter.x - w, ScreenCenter.y - h, ScreenCenter.x + w, ScreenCenter.y + h);
	ZL_Display::PushOrtho(ortho);
	ZL_Rect view((int)(ortho.left/(int)HEXW-1), (int)(ortho.high/(int)HEXH+1), (int)(ortho.right/(int)HEXW+1), (int)(ortho.low/(int)HEXH-1));

	srfBlank.SetColor(colGrid).BatchRenderBegin();
	for (int y = view.bottom; y <= view.top; y++)
		for (int x = view.left; x <= view.right; x++)
			srfBlank.Draw(x*HEXW+(y&1 ? HEXO : 0), y*HEXH);
	srfBlank.BatchRenderEnd();

	scalar o = s(HEXH+HEXW)*Zoom;
	ortho.left -= o; ortho.right += o; ortho.low -= o; ortho.high += o;

	srfFill.BatchRenderBegin(true);
	if (SwarmSelection[0].Time)
		for (sSwarmSelection* itSwarmSelection = SwarmSelection; itSwarmSelection != &SwarmSelection[sizeof(SwarmSelection)/sizeof(SwarmSelection[0])]; ++itSwarmSelection)
			itSwarmSelection->Draw();

	for (vector<sHex*>::iterator itHex = Hexes.begin(); itHex != Hexes.end(); ++itHex)
		if ((*itHex)->Health && (*itHex)->Pos.x > ortho.left && (*itHex)->Pos.x < ortho.right && (*itHex)->Pos.y > ortho.low && (*itHex)->Pos.y < ortho.high)
			(*itHex)->Draw();
	srfFill.BatchRenderEnd();

	srfEnemy.BatchRenderBegin(true);
	for (vector<sEnemy*>::iterator itEnemy = Enemies.begin(); itEnemy != Enemies.end(); ++itEnemy)
		if ((*itEnemy)->Health && (*itEnemy)->Pos.x > ortho.left && (*itEnemy)->Pos.x < ortho.right && (*itEnemy)->Pos.y > ortho.low && (*itEnemy)->Pos.y < ortho.high)
			(*itEnemy)->Draw();
	srfEnemy.BatchRenderEnd();

	for (vector<sHex*>::iterator itHexShield = Hexes.begin(); itHexShield != Hexes.end(); ++itHexShield)
		if ((*itHexShield)->Health)
			(*itHexShield)->DrawOverlay();

	srfLaser.BatchRenderBegin(true);
	for (sLaser* itLaser = Lasers; itLaser != &Lasers[sizeof(Lasers)/sizeof(Lasers[0])]; ++itLaser)
		if (itLaser->TimeUntil > GameTicks) itLaser->Draw();
	srfLaser.BatchRenderEnd();

	particleSpark.Draw();

	if (DoUpdateSelection) UpdateSelection();
	srfFill.BatchRenderBegin(true);
	for (sHexSelection* itHexSelection = HexSelection; itHexSelection != &HexSelection[sizeof(HexSelection)/sizeof(HexSelection[0])]; ++itHexSelection)
	{
		if (!itHexSelection->Time) continue;
		scalar f = (World.GameTicks-itHexSelection->Time > SELECTIONFADE ? 1 : s(World.GameTicks-itHexSelection->Time)/s(SELECTIONFADE));
		if (itHexSelection != HexSelection) f = 1-f;
		srfFill.Draw(itHexSelection->Pos.x*HEXW+(itHexSelection->Pos.y&1 ? HEXO : 0), itHexSelection->Pos.y*HEXH, ZLALPHA(f*.4));
	}
	srfFill.BatchRenderEnd();

	ZL_Display::PopOrtho();
}
