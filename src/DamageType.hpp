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

typedef int ItemWeaponType;

const ItemWeaponType wepMelee = 0;
const ItemWeaponType wepRanged = 1;
const ItemWeaponType wepMagic = 2;

typedef int DamageType;
const DamageType damSharp = 0;
const DamageType damBlunt = 1;
const DamageType damPierce = 2;
const DamageType damFire = 3;
const DamageType damIce = 4;
const DamageType damShock = 5;
const DamageType damPoison = 6;
const DamageType damBlood = 7;
const DamageType damDebug = 8;
const DamageType damSuffocation = 9;
const DamageType damExplosion = 10;

string damageTypeName(DamageType d);

Ui::Color damageTypeColor(DamageType d);

#endif /* defined(__Underneath__DamageType__) */
