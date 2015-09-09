//
//  DamageType.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "DamageType.hpp"

string damageTypeName(DamageType d) {
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
            return "Bleed";
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

Ui::Color damageTypeColor(DamageType d) {
    switch (d) {
        case damSharp:
            return C_LIGHT_GRAY;
        case damBlunt:
            return C_LIGHT_GRAY;
        case damPierce:
            return C_LIGHT_GRAY;
        case damFire:
            return C_LIGHT_YELLOW;
        case damIce:
            return C_LIGHT_CYAN;
        case damShock:
            return C_LIGHT_MAGENTA;
        case damPoison:
            return C_LIGHT_GREEN;
        case damBlood:
            return C_LIGHT_RED;
        case damDebug:
            return C_WHITE;
        case damExplosion:
            return C_LIGHT_YELLOW;
        default:
            return C_WHITE;
    }
}
