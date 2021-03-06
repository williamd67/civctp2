//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : Unit utilities
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// -None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Added unitutil_GetSmallCityMaxSize to figure out the maximum population
//   size a ring one city. - Oct. 6th 2004 Martin G�hmann
// - Added function to compare unit type quality, based on unit cargo capacity
//   or on the units statistics like attack, defense and range. (19-May-2007 Martin G�hmann)
// - Added GetCityLandAttackBonus, GetCityAirAttackBonus and GetCitySeaAttackBonus
//   for battleview window. (07-Mar-2009 Maq)
//
//----------------------------------------------------------------------------
#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __UNIT_UTIL_H__
#define __UNIT_UTIL_H__

class MapPoint;
class SpecialAttackInfoRecord;
class MapPoint;
class Army;
class CellUnitList;
class Unit;
class UnitRecord;
enum SPECATTACK;

void unitutil_Initialize();
sint32 unitutil_MaxActiveDefenseRange();
sint32 unitutil_GetMaxVisionRange();
sint32 unitutil_GetLandCity();
sint32 unitutil_GetSeaCity();
sint32 unitutil_GetCityTypeFor(const MapPoint &pos);
const SpecialAttackInfoRecord *unitutil_GetSpecialAttack(SPECATTACK attack);

void unitutil_GetAverageDefenseBonus(const MapPoint &pos, const Army &attackers, const CellUnitList &defenders, double & city_bonus, double & entrenched_bonus);
void unitutil_GetCityLandAttackBonus(const MapPoint &pos, double & city_landatk_bonus);
void unitutil_GetCityAirAttackBonus(const MapPoint &pos, double & city_airatk_bonus);
void unitutil_GetCitySeaAttackBonus(const MapPoint &pos, double & city_seaatk_bonus);
bool unitutil_GetCityInfo(MapPoint &pos, char * city_name, sint32 & image_index);
bool unitutil_IsUnitBetterThan(sint32 type1, sint32 type2, sint32 gov);

void unitutil_ExecuteMadLaunch(Unit & unit);

sint32 unitutil_GetSmallCityMaxSize();
sint32 unitutil_GetMaxRadius();

double unitutil_GetPositionDefense(const UnitRecord * rec, const bool isEntrenched, const MapPoint pos, const Unit &attacker);

#endif
