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

static string damageTypeName(DamageType d) {
    switch (d) {
        case damSharp:
            return "Sharp";
        case damBlunt:
            return "Blunt";
        case damPierce:
            return "Pierce";
        case damFire:
            return "Fire";
        case damIce:
            return "Ice";
        case damShock:
            return "Shock";
        case damPoison:
            return "Poison";
        case damBlood:
            return "Blood";
        case damDebug:
            return "Debug";
        case damSuffocation:
            return "Suffocation";
        case damExplosion:
            return "Explosive";
        default:
            return "Undefined";
    }
}

static Ui::Color damageTypeColor(DamageType d) {
    switch (d) {
        case damSharp:
            return Ui::C_LIGHT_GRAY;
        case damBlunt:
            return Ui::C_LIGHT_GRAY;
        case damFire:
            return Ui::C_LIGHT_YELLOW;
        case damIce:
            return Ui::C_LIGHT_CYAN;
        case damShock:
            return Ui::C_LIGHT_MAGENTA;
        case damPoison:
            return Ui::C_LIGHT_GREEN;
        case damBlood:
            return Ui::C_LIGHT_RED;
        case damDebug:
            return Ui::C_WHITE;
        case damExplosion:
            return Ui::C_LIGHT_YELLOW;
        default:
            return Ui::C_WHITE;
    }
}

#endif /* defined(__Underneath__DamageType__) */
