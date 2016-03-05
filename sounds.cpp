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

//------------------------------------------------------------------------------------------------------------------
#define IMCENEMYLASER_IMCSONG_LEN 0x1
#define IMCENEMYLASER_IMCSONG_ROWLENSAMPLES 2594
#define IMCENEMYLASER_IMCSONG_ENVLISTSIZE 2
#define IMCENEMYLASER_IMCSONG_ENVCOUNTERLISTSIZE 3
#define IMCENEMYLASER_IMCSONG_OSCLISTSIZE 9
#define IMCENEMYLASER_IMCSONG_EFFECTLISTSIZE 2
#define IMCENEMYLASER_IMCSONG_VOL 100
unsigned int IMCENEMYLASER_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCENEMYLASER_ImcSongPatternData[] = {
  0x50, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCENEMYLASER_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCENEMYLASER_ImcSongEnvList[] = {
  { 0, 256, 741, 5, 19, 255, true, 255, },
  { 0, 256, 635, 7, 255, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCENEMYLASER_ImcSongEnvCounterList[] = {
 { 0, 0, 238 }, { -1, -1, 256 }, { 1, 0, 254 },
};
TImcSongOscillator IMCENEMYLASER_ImcSongOscillatorList[] = {
  { 9, 48, IMCSONGOSCTYPE_SAW, 0, -1, 124, 1, 2 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 0, 0, 8, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
TImcSongEffect IMCENEMYLASER_ImcSongEffectList[] = {
  { 95, 199, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
  { 7239, 248, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
};
unsigned char IMCENEMYLASER_ImcSongChannelVol[8] = {100, 100, 100, 100, 100, 100, 100, 100 };
unsigned char IMCENEMYLASER_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCENEMYLASER_ImcSongChannelStopNote[8] = {true, false, false, false, false, false, false, false };
TImcSongData imcDataIMCENEMYLASER = {
  IMCENEMYLASER_IMCSONG_LEN, IMCENEMYLASER_IMCSONG_ROWLENSAMPLES, IMCENEMYLASER_IMCSONG_ENVLISTSIZE, IMCENEMYLASER_IMCSONG_ENVCOUNTERLISTSIZE, IMCENEMYLASER_IMCSONG_OSCLISTSIZE, IMCENEMYLASER_IMCSONG_EFFECTLISTSIZE, IMCENEMYLASER_IMCSONG_VOL,
  IMCENEMYLASER_ImcSongOrderTable, IMCENEMYLASER_ImcSongPatternData, IMCENEMYLASER_ImcSongPatternLookupTable, IMCENEMYLASER_ImcSongEnvList, IMCENEMYLASER_ImcSongEnvCounterList, IMCENEMYLASER_ImcSongOscillatorList, IMCENEMYLASER_ImcSongEffectList,
  IMCENEMYLASER_ImcSongChannelVol, IMCENEMYLASER_ImcSongChannelEnvCounter, IMCENEMYLASER_ImcSongChannelStopNote };
ZL_SynthImcSamplePool imcEnemyLaser(&imcDataIMCENEMYLASER);
//------------------------------------------------------------------------------------------------------------------
#define IMCPLAYERLASER_IMCSONG_LEN 0x1
#define IMCPLAYERLASER_IMCSONG_ROWLENSAMPLES 2594
#define IMCPLAYERLASER_IMCSONG_ENVLISTSIZE 2
#define IMCPLAYERLASER_IMCSONG_ENVCOUNTERLISTSIZE 3
#define IMCPLAYERLASER_IMCSONG_OSCLISTSIZE 9
#define IMCPLAYERLASER_IMCSONG_EFFECTLISTSIZE 2
#define IMCPLAYERLASER_IMCSONG_VOL 100
unsigned int IMCPLAYERLASER_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCPLAYERLASER_ImcSongPatternData[] = {
  0x50, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCPLAYERLASER_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCPLAYERLASER_ImcSongEnvList[] = {
  { 0, 256, 123, 5, 19, 255, true, 255, },
  { 0, 256, 92, 10, 255, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCPLAYERLASER_ImcSongEnvCounterList[] = {
 { 0, 0, 238 }, { -1, -1, 256 }, { 1, 0, 248 },
};
TImcSongOscillator IMCPLAYERLASER_ImcSongOscillatorList[] = {
  { 9, 0, IMCSONGOSCTYPE_SAW, 0, -1, 250, 1, 2 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 0, 0, 8, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
TImcSongEffect IMCPLAYERLASER_ImcSongEffectList[] = {
  { 152, 181, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
  { 9271, 194, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
};
unsigned char IMCPLAYERLASER_ImcSongChannelVol[8] = {100, 100, 100, 100, 100, 100, 100, 100 };
unsigned char IMCPLAYERLASER_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCPLAYERLASER_ImcSongChannelStopNote[8] = {true, false, false, false, false, false, false, false };
TImcSongData imcDataIMCPLAYERLASER = {
  IMCPLAYERLASER_IMCSONG_LEN, IMCPLAYERLASER_IMCSONG_ROWLENSAMPLES, IMCPLAYERLASER_IMCSONG_ENVLISTSIZE, IMCPLAYERLASER_IMCSONG_ENVCOUNTERLISTSIZE, IMCPLAYERLASER_IMCSONG_OSCLISTSIZE, IMCPLAYERLASER_IMCSONG_EFFECTLISTSIZE, IMCPLAYERLASER_IMCSONG_VOL,
  IMCPLAYERLASER_ImcSongOrderTable, IMCPLAYERLASER_ImcSongPatternData, IMCPLAYERLASER_ImcSongPatternLookupTable, IMCPLAYERLASER_ImcSongEnvList, IMCPLAYERLASER_ImcSongEnvCounterList, IMCPLAYERLASER_ImcSongOscillatorList, IMCPLAYERLASER_ImcSongEffectList,
  IMCPLAYERLASER_ImcSongChannelVol, IMCPLAYERLASER_ImcSongChannelEnvCounter, IMCPLAYERLASER_ImcSongChannelStopNote };
ZL_SynthImcSamplePool imcPlayerLaser(&imcDataIMCPLAYERLASER);
//------------------------------------------------------------------------------------------------------------------
#define IMCENEMYEXPLOSION_IMCSONG_LEN 0x1
#define IMCENEMYEXPLOSION_IMCSONG_ROWLENSAMPLES 1024
#define IMCENEMYEXPLOSION_IMCSONG_ENVLISTSIZE 3
#define IMCENEMYEXPLOSION_IMCSONG_ENVCOUNTERLISTSIZE 4
#define IMCENEMYEXPLOSION_IMCSONG_OSCLISTSIZE 9
#define IMCENEMYEXPLOSION_IMCSONG_EFFECTLISTSIZE 1
#define IMCENEMYEXPLOSION_IMCSONG_VOL 128
unsigned int IMCENEMYEXPLOSION_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCENEMYEXPLOSION_ImcSongPatternData[] = {
  0x50, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCENEMYEXPLOSION_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCENEMYEXPLOSION_ImcSongEnvList[] = {
  { 0, 256, 69, 8, 16, 255, true, 255, },
  { 0, 256, 137, 4, 15, 255, true, 255, },
  { 0, 256, 184, 5, 19, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCENEMYEXPLOSION_ImcSongEnvCounterList[] = {
 { 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 224 }, { 2, 0, 238 },
};
TImcSongOscillator IMCENEMYEXPLOSION_ImcSongOscillatorList[] = {
  { 6, 221, IMCSONGOSCTYPE_SQUARE, 0, -1, 80, 1, 2 },
  { 8, 0, IMCSONGOSCTYPE_NOISE, 0, 0, 90, 3, 1 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
TImcSongEffect IMCENEMYEXPLOSION_ImcSongEffectList[] = {
  { 16891, 337, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
};
unsigned char IMCENEMYEXPLOSION_ImcSongChannelVol[8] = {102, 100, 100, 100, 100, 100, 100, 100 };
unsigned char IMCENEMYEXPLOSION_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCENEMYEXPLOSION_ImcSongChannelStopNote[8] = {true, false, false, false, false, false, false, false };
TImcSongData imcDataIMCENEMYEXPLOSION = {
  IMCENEMYEXPLOSION_IMCSONG_LEN, IMCENEMYEXPLOSION_IMCSONG_ROWLENSAMPLES, IMCENEMYEXPLOSION_IMCSONG_ENVLISTSIZE, IMCENEMYEXPLOSION_IMCSONG_ENVCOUNTERLISTSIZE, IMCENEMYEXPLOSION_IMCSONG_OSCLISTSIZE, IMCENEMYEXPLOSION_IMCSONG_EFFECTLISTSIZE, IMCENEMYEXPLOSION_IMCSONG_VOL,
  IMCENEMYEXPLOSION_ImcSongOrderTable, IMCENEMYEXPLOSION_ImcSongPatternData, IMCENEMYEXPLOSION_ImcSongPatternLookupTable, IMCENEMYEXPLOSION_ImcSongEnvList, IMCENEMYEXPLOSION_ImcSongEnvCounterList, IMCENEMYEXPLOSION_ImcSongOscillatorList, IMCENEMYEXPLOSION_ImcSongEffectList,
  IMCENEMYEXPLOSION_ImcSongChannelVol, IMCENEMYEXPLOSION_ImcSongChannelEnvCounter, IMCENEMYEXPLOSION_ImcSongChannelStopNote };
ZL_SynthImcTrack imcEnemyExplosion(&imcDataIMCENEMYEXPLOSION, false);
//------------------------------------------------------------------------------------------------------------------
#define IMCPLAYEREXPLOSION_IMCSONG_LEN 0x1
#define IMCPLAYEREXPLOSION_IMCSONG_ROWLENSAMPLES 2594
#define IMCPLAYEREXPLOSION_IMCSONG_ENVLISTSIZE 3
#define IMCPLAYEREXPLOSION_IMCSONG_ENVCOUNTERLISTSIZE 4
#define IMCPLAYEREXPLOSION_IMCSONG_OSCLISTSIZE 9
#define IMCPLAYEREXPLOSION_IMCSONG_EFFECTLISTSIZE 1
#define IMCPLAYEREXPLOSION_IMCSONG_VOL 179
unsigned int IMCPLAYEREXPLOSION_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCPLAYEREXPLOSION_ImcSongPatternData[] = {
  0x50, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCPLAYEREXPLOSION_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCPLAYEREXPLOSION_ImcSongEnvList[] = {
  { 0, 256, 5, 0, 24, 255, true, 255, },
  { 0, 256, 42, 8, 16, 255, true, 255, },
  { 0, 256, 87, 5, 19, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCPLAYEREXPLOSION_ImcSongEnvCounterList[] = {
 { -1, -1, 256 }, { 0, 0, 128 }, { 1, 0, 256 }, { 2, 0, 238 },
};
TImcSongOscillator IMCPLAYEREXPLOSION_ImcSongOscillatorList[] = {
  { 6, 169, IMCSONGOSCTYPE_SQUARE, 0, -1, 80, 1, 2 },
  { 8, 0, IMCSONGOSCTYPE_NOISE, 0, 0, 90, 3, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
TImcSongEffect IMCPLAYEREXPLOSION_ImcSongEffectList[] = {
  { 17272, 373, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 0 },
};
unsigned char IMCPLAYEREXPLOSION_ImcSongChannelVol[8] = {99, 100, 100, 100, 100, 100, 100, 100 };
unsigned char IMCPLAYEREXPLOSION_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCPLAYEREXPLOSION_ImcSongChannelStopNote[8] = {true, false, false, false, false, false, false, false };
TImcSongData imcDataIMCPLAYEREXPLOSION = {
  IMCPLAYEREXPLOSION_IMCSONG_LEN, IMCPLAYEREXPLOSION_IMCSONG_ROWLENSAMPLES, IMCPLAYEREXPLOSION_IMCSONG_ENVLISTSIZE, IMCPLAYEREXPLOSION_IMCSONG_ENVCOUNTERLISTSIZE, IMCPLAYEREXPLOSION_IMCSONG_OSCLISTSIZE, IMCPLAYEREXPLOSION_IMCSONG_EFFECTLISTSIZE, IMCPLAYEREXPLOSION_IMCSONG_VOL,
  IMCPLAYEREXPLOSION_ImcSongOrderTable, IMCPLAYEREXPLOSION_ImcSongPatternData, IMCPLAYEREXPLOSION_ImcSongPatternLookupTable, IMCPLAYEREXPLOSION_ImcSongEnvList, IMCPLAYEREXPLOSION_ImcSongEnvCounterList, IMCPLAYEREXPLOSION_ImcSongOscillatorList, IMCPLAYEREXPLOSION_ImcSongEffectList,
  IMCPLAYEREXPLOSION_ImcSongChannelVol, IMCPLAYEREXPLOSION_ImcSongChannelEnvCounter, IMCPLAYEREXPLOSION_ImcSongChannelStopNote };
ZL_SynthImcTrack imcPlayerExplosion(&imcDataIMCPLAYEREXPLOSION, false);
//------------------------------------------------------------------------------------------------------------------
#define IMCERROR_IMCSONG_LEN 0x1
#define IMCERROR_IMCSONG_ROWLENSAMPLES 2594
#define IMCERROR_IMCSONG_ENVLISTSIZE 2
#define IMCERROR_IMCSONG_ENVCOUNTERLISTSIZE 3
#define IMCERROR_IMCSONG_OSCLISTSIZE 10
#define IMCERROR_IMCSONG_EFFECTLISTSIZE 0
#define IMCERROR_IMCSONG_VOL 100
unsigned int IMCERROR_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCERROR_ImcSongPatternData[] = {
  0x50, 255,  0x50, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCERROR_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCERROR_ImcSongEnvList[] = {
  { 0, 256, 130, 5, 19, 255, true, 255, },
  { 0, 256, 523, 8, 16, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCERROR_ImcSongEnvCounterList[] = {
 { 0, 0, 238 }, { -1, -1, 256 }, { 1, 0, 256 },
};
TImcSongOscillator IMCERROR_ImcSongOscillatorList[] = {
  { 5, 127, IMCSONGOSCTYPE_SQUARE, 0, -1, 100, 1, 1 },
  { 5, 174, IMCSONGOSCTYPE_SQUARE, 0, -1, 222, 2, 1 },
  { 8, 0, IMCSONGOSCTYPE_NOISE, 0, 0, 8, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
unsigned char IMCERROR_ImcSongChannelVol[8] = {100, 100, 100, 100, 100, 100, 100, 100 };
unsigned char IMCERROR_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCERROR_ImcSongChannelStopNote[8] = {true, false, false, false, false, false, false, false };
TImcSongData imcDataIMCERROR = {
  IMCERROR_IMCSONG_LEN, IMCERROR_IMCSONG_ROWLENSAMPLES, IMCERROR_IMCSONG_ENVLISTSIZE, IMCERROR_IMCSONG_ENVCOUNTERLISTSIZE, IMCERROR_IMCSONG_OSCLISTSIZE, IMCERROR_IMCSONG_EFFECTLISTSIZE, IMCERROR_IMCSONG_VOL,
  IMCERROR_ImcSongOrderTable, IMCERROR_ImcSongPatternData, IMCERROR_ImcSongPatternLookupTable, IMCERROR_ImcSongEnvList, IMCERROR_ImcSongEnvCounterList, IMCERROR_ImcSongOscillatorList, NULL,
  IMCERROR_ImcSongChannelVol, IMCERROR_ImcSongChannelEnvCounter, IMCERROR_ImcSongChannelStopNote };
ZL_SynthImcTrack imcError(&imcDataIMCERROR, false);
//------------------------------------------------------------------------------------------------------------------
#define IMCACCEPT_IMCSONG_LEN 0x1
#define IMCACCEPT_IMCSONG_ROWLENSAMPLES 1124
#define IMCACCEPT_IMCSONG_ENVLISTSIZE 3
#define IMCACCEPT_IMCSONG_ENVCOUNTERLISTSIZE 4
#define IMCACCEPT_IMCSONG_OSCLISTSIZE 4
#define IMCACCEPT_IMCSONG_EFFECTLISTSIZE 2
#define IMCACCEPT_IMCSONG_VOL 63
unsigned int IMCACCEPT_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCACCEPT_ImcSongPatternData[] = {
  0x50, 0,    255,  0,    0x52, 0,    255,  0,    0x54, 0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCACCEPT_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCACCEPT_ImcSongEnvList[] = {
  { 0, 150, 144, 1, 23, 255, true, 255, },
  { 0, 256, 699, 8, 16, 255, true, 255, },
  { 0, 256, 172, 8, 16, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCACCEPT_ImcSongEnvCounterList[] = {
 { 0, 0, 92 }, { 1, 0, 256 }, { -1, -1, 256 }, { 2, 0, 256 },
};
TImcSongOscillator IMCACCEPT_ImcSongOscillatorList[] = {
  { 8, 0, IMCSONGOSCTYPE_NOISE, 0, -1, 230, 1, 2 },
  { 8, 200, IMCSONGOSCTYPE_SINE, 0, -1, 124, 3, 2 },
  { 10, 106, IMCSONGOSCTYPE_SQUARE, 0, -1, 82, 2, 2 },
  { 9, 15, IMCSONGOSCTYPE_SQUARE, 0, 2, 52, 2, 2 },
};
TImcSongEffect IMCACCEPT_ImcSongEffectList[] = {
  { 120, 0, 1, 0, IMCSONGEFFECTTYPE_LOWPASS, 2, 0 },
  { 15748, 530, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 2 },
};
unsigned char IMCACCEPT_ImcSongChannelVol[8] = {84, 84, 100, 100, 100, 100, 100, 100 };
unsigned char IMCACCEPT_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCACCEPT_ImcSongChannelStopNote[8] = {true, true, false, false, false, false, false, false };
TImcSongData imcDataIMCACCEPT = {
  IMCACCEPT_IMCSONG_LEN, IMCACCEPT_IMCSONG_ROWLENSAMPLES, IMCACCEPT_IMCSONG_ENVLISTSIZE, IMCACCEPT_IMCSONG_ENVCOUNTERLISTSIZE, IMCACCEPT_IMCSONG_OSCLISTSIZE, IMCACCEPT_IMCSONG_EFFECTLISTSIZE, IMCACCEPT_IMCSONG_VOL,
  IMCACCEPT_ImcSongOrderTable, IMCACCEPT_ImcSongPatternData, IMCACCEPT_ImcSongPatternLookupTable, IMCACCEPT_ImcSongEnvList, IMCACCEPT_ImcSongEnvCounterList, IMCACCEPT_ImcSongOscillatorList, IMCACCEPT_ImcSongEffectList,
  IMCACCEPT_ImcSongChannelVol, IMCACCEPT_ImcSongChannelEnvCounter, IMCACCEPT_ImcSongChannelStopNote };
ZL_SynthImcTrack imcAccept(&imcDataIMCACCEPT, false);
//------------------------------------------------------------------------------------------------------------------
#define IMCMENU_IMCSONG_LEN 0x1
#define IMCMENU_IMCSONG_ROWLENSAMPLES 400
#define IMCMENU_IMCSONG_ENVLISTSIZE 2
#define IMCMENU_IMCSONG_ENVCOUNTERLISTSIZE 3
#define IMCMENU_IMCSONG_OSCLISTSIZE 3
#define IMCMENU_IMCSONG_EFFECTLISTSIZE 0
#define IMCMENU_IMCSONG_VOL 63
unsigned int IMCMENU_ImcSongOrderTable[] = {
  0x000000001,
};
unsigned char IMCMENU_ImcSongPatternData[] = {
  0x50, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
unsigned char IMCMENU_ImcSongPatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
TImcSongEnvelope IMCMENU_ImcSongEnvList[] = {
  { 0, 256, 173, 8, 16, 255, true, 255, },
  { 0, 256, 21, 8, 16, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCMENU_ImcSongEnvCounterList[] = {
 { 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 256 },
};
TImcSongOscillator IMCMENU_ImcSongOscillatorList[] = {
  { 6, 0, IMCSONGOSCTYPE_SINE, 0, -1, 128, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 146, 1, 1 },
  { 9, 0, IMCSONGOSCTYPE_SINE, 0, -1, 112, 1, 2 },
};
unsigned char IMCMENU_ImcSongChannelVol[8] = {76, 84, 100, 100, 100, 100, 100, 100 };
unsigned char IMCMENU_ImcSongChannelEnvCounter[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
bool IMCMENU_ImcSongChannelStopNote[8] = {true, true, false, false, false, false, false, false };
TImcSongData imcDataIMCMENU = {
  IMCMENU_IMCSONG_LEN, IMCMENU_IMCSONG_ROWLENSAMPLES, IMCMENU_IMCSONG_ENVLISTSIZE, IMCMENU_IMCSONG_ENVCOUNTERLISTSIZE, IMCMENU_IMCSONG_OSCLISTSIZE, IMCMENU_IMCSONG_EFFECTLISTSIZE, IMCMENU_IMCSONG_VOL,
  IMCMENU_ImcSongOrderTable, IMCMENU_ImcSongPatternData, IMCMENU_ImcSongPatternLookupTable, IMCMENU_ImcSongEnvList, IMCMENU_ImcSongEnvCounterList, IMCMENU_ImcSongOscillatorList, NULL,
  IMCMENU_ImcSongChannelVol, IMCMENU_ImcSongChannelEnvCounter, IMCMENU_ImcSongChannelStopNote };
ZL_SynthImcTrack imcMenu(&imcDataIMCMENU, false);
//------------------------------------------------------------------------------------------------------------------
#define IMCTITLEMUSIC_IMCSONG_LEN 0xA
#define IMCTITLEMUSIC_IMCSONG_ROWLENSAMPLES 5512
#define IMCTITLEMUSIC_IMCSONG_ENVLISTSIZE 12
#define IMCTITLEMUSIC_IMCSONG_ENVCOUNTERLISTSIZE 16
#define IMCTITLEMUSIC_IMCSONG_OSCLISTSIZE 18
#define IMCTITLEMUSIC_IMCSONG_EFFECTLISTSIZE 12
#define IMCTITLEMUSIC_IMCSONG_VOL 100

unsigned int IMCTITLEMUSIC_ImcSongOrderTable[] = {
  0x022100002, 0x022100022, 0x023100033, 0x023100033, 0x023100011, 0x023100013, 0x023100031, 0x023100030,
  0x023100000, 0x020100000,
};
unsigned char IMCTITLEMUSIC_ImcSongPatternData[] = {
  0,    0,    0x20, 0,    0,    0,    0x27, 0,    0,    0,    0x2B, 0,    0x24, 0,    0,    0,
  0x34, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  0x20, 0,    0x20, 0,    0x20, 0,    0x27, 0,    0x24, 0,    0x2B, 0x20, 0x24, 0,    0,    0,
  0x30, 0,    0x30, 0,    0x30, 0,    0x30, 0,    0x24, 0,    0,    0,    0,    0,    0x22, 0,
  0x44, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  0x40, 0,    0x40, 0,    0x40, 0,    0x37, 0,    0x34, 0,    0x3B, 0,    0x34, 0,    0,    0,
  0x14, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,    0x20, 0,    0,    0,    0,    0,    0,    0,
  0x31, 0,    0,    0,    0x31, 0,    0,    0,    0x50, 0,    0,    0,    0x31, 0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  0x50, 0,    0x52, 0,    0x55, 0,    0,    0x50, 0x52, 0,    0,    0,    0x54, 0,    0x52, 0,
};
unsigned char IMCTITLEMUSIC_ImcSongPatternLookupTable[] = { 0, 3, 6, 6, 6, 6, 7, 10, };
TImcSongEnvelope IMCTITLEMUSIC_ImcSongEnvList[] = {
  { 0, 256, 15, 8, 16, 255, true, 255, },
  { 50, 95, 130, 24, 255, 255, true, 255, },
  { 25, 256, 64, 8, 16, 255, true, 255, },
  { 0, 256, 21, 8, 16, 255, true, 255, },
  { 50, 100, 130, 24, 255, 255, true, 255, },
  { 0, 256, 10, 8, 16, 255, true, 255, },
  { 150, 180, 161, 8, 255, 255, false, 255, },
  { 0, 256, 379, 8, 16, 255, true, 255, },
  { 32, 256, 196, 8, 16, 255, true, 255, },
  { 0, 256, 87, 8, 16, 255, true, 255, },
  { 196, 256, 29, 8, 16, 255, true, 255, },
  { 0, 128, 1046, 8, 16, 255, true, 255, },
};
TImcSongEnvelopeCounter IMCTITLEMUSIC_ImcSongEnvCounterList[] = {
 { 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 50 }, { 2, 0, 256 },
  { 3, 1, 256 }, { 4, 1, 50 }, { 2, 1, 256 }, { 5, 5, 256 },
  { -1, -1, 258 }, { 6, 5, 180 }, { 7, 6, 256 }, { 8, 6, 256 },
  { 9, 7, 256 }, { 10, 7, 256 }, { 9, 7, 256 }, { 11, 7, 128 },
};
TImcSongOscillator IMCTITLEMUSIC_ImcSongOscillatorList[] = {
  { 8, 0, IMCSONGOSCTYPE_SAW, 0, -1, 100, 1, 1 },
  { 7, 0, IMCSONGOSCTYPE_SQUARE, 0, -1, 68, 1, 1 },
  { 7, 0, IMCSONGOSCTYPE_SINE, 0, -1, 255, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SAW, 0, 0, 34, 1, 1 },
  { 9, 0, IMCSONGOSCTYPE_SQUARE, 0, 1, 10, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SAW, 1, -1, 100, 1, 1 },
  { 7, 0, IMCSONGOSCTYPE_SQUARE, 1, -1, 68, 1, 1 },
  { 7, 0, IMCSONGOSCTYPE_SINE, 1, -1, 255, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SAW, 1, 5, 34, 1, 1 },
  { 9, 0, IMCSONGOSCTYPE_SQUARE, 1, 6, 10, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
  { 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_SAW, 5, -1, 110, 1, 8 },
  { 9, 1, IMCSONGOSCTYPE_SAW, 5, -1, 110, 1, 1 },
  { 8, 0, IMCSONGOSCTYPE_NOISE, 6, -1, 127, 1, 11 },
  { 5, 15, IMCSONGOSCTYPE_SINE, 7, -1, 255, 12, 13 },
  { 7, 0, IMCSONGOSCTYPE_SINE, 7, -1, 255, 14, 15 },
};
TImcSongEffect IMCTITLEMUSIC_ImcSongEffectList[] = {
  { 0, 0, 96, 0, IMCSONGEFFECTTYPE_FLANGE, 2, 0 },
  { 177, 169, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 3, 1 },
  { 0, 0, 101, 1, IMCSONGEFFECTTYPE_FLANGE, 5, 0 },
  { 177, 169, 1, 1, IMCSONGEFFECTTYPE_RESONANCE, 6, 1 },
  { 0, 0, 181, 5, IMCSONGEFFECTTYPE_FLANGE, 9, 0 },
  { 76, 2, 1, 5, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
  { 2667, 2629, 1, 5, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
  { 61, 0, 14717, 5, IMCSONGEFFECTTYPE_DELAY, 0, 0 },
  { 128, 0, 6449, 6, IMCSONGEFFECTTYPE_DELAY, 0, 0 },
  { 255, 110, 1, 6, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
  { 227, 0, 1, 6, IMCSONGEFFECTTYPE_HIGHPASS, 1, 0 },
  { 18034, 463, 1, 7, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
};
unsigned char IMCTITLEMUSIC_ImcSongChannelVol[8] = {79, 122, 158, 100, 100, 25, 199, 43 };
unsigned char IMCTITLEMUSIC_ImcSongChannelEnvCounter[8] = {0, 4, 0, 0, 0, 7, 10, 1 };
bool IMCTITLEMUSIC_ImcSongChannelStopNote[8] = {true, true, false, false, false, true, true, true };
TImcSongData imcDataTitleMusic = {
  IMCTITLEMUSIC_IMCSONG_LEN, IMCTITLEMUSIC_IMCSONG_ROWLENSAMPLES, IMCTITLEMUSIC_IMCSONG_ENVLISTSIZE, IMCTITLEMUSIC_IMCSONG_ENVCOUNTERLISTSIZE, IMCTITLEMUSIC_IMCSONG_OSCLISTSIZE, IMCTITLEMUSIC_IMCSONG_EFFECTLISTSIZE, IMCTITLEMUSIC_IMCSONG_VOL,
  IMCTITLEMUSIC_ImcSongOrderTable, IMCTITLEMUSIC_ImcSongPatternData, IMCTITLEMUSIC_ImcSongPatternLookupTable, IMCTITLEMUSIC_ImcSongEnvList, IMCTITLEMUSIC_ImcSongEnvCounterList, IMCTITLEMUSIC_ImcSongOscillatorList, IMCTITLEMUSIC_ImcSongEffectList,
  IMCTITLEMUSIC_ImcSongChannelVol, IMCTITLEMUSIC_ImcSongChannelEnvCounter, IMCTITLEMUSIC_ImcSongChannelStopNote };
//------------------------------------------------------------------------------------------------------------------
