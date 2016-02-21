//
//  DamageType.hpp
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__DamageType__
#define __Underneath__DamageType__

#include "Global.hpp"
#include "Color.hpp"

enum WeaponType{wepMelee, wepRanged, wepMagic};

enum DamageType{damNone, damSharp, damBlunt, damPierce, damFire, damIce, damShock, damPoison, damBlood, damDebug, damSuffocation, damExplosion, damQty};

string damageTypeName(DamageType d);

Ui::Color damageTypeColor(DamageType d);

#endif /* defined(__Underneath__DamageType__) */
