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
            return Ui::C_LIGHT_GRAY;
        case damBlunt:
            return Ui::C_LIGHT_GRAY;
        case damPierce:
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
