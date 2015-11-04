//
//  DamageType.h
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__DamageType__
#define __Underneath__DamageType__

#include "Global.hpp"
#include "Color.hpp"

typedef int WeaponType;

const WeaponType wepMelee = 0;
const WeaponType wepRanged = 1;
const WeaponType wepMagic = 2;

typedef int DamageType;
const DamageType damNone = 0;
const DamageType damSharp = 1;
const DamageType damBlunt = 2;
const DamageType damPierce = 3;
const DamageType damFire = 4;
const DamageType damIce = 5;
const DamageType damShock = 6;
const DamageType damPoison = 7;
const DamageType damBlood = 8;
const DamageType damDebug = 9;
const DamageType damSuffocation = 10;
const DamageType damExplosion = 11;

string damageTypeName(DamageType d);

Ui::Color damageTypeColor(DamageType d);

#endif /* defined(__Underneath__DamageType__) */
