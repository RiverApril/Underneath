//
//  Color.h
//  Underneath
//
//  Created by Braeden Atlee on 4/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Color__
#define __Underneath__Color__

namespace Ui {

    typedef unsigned char Color;

    extern Color C_DARK_BLACK;
    extern Color C_DARK_RED;
    extern Color C_DARK_GREEN;
    extern Color C_DARK_YELLOW;
    extern Color C_DARK_BLUE;
    extern Color C_DARK_MAGENTA;
    extern Color C_DARK_CYAN;
    extern Color C_DARK_WHITE;

    extern Color C_LIGHT_BLACK;
    extern Color C_LIGHT_RED;
    extern Color C_LIGHT_GREEN;
    extern Color C_LIGHT_YELLOW;
    extern Color C_LIGHT_BLUE;
    extern Color C_LIGHT_MAGENTA;
    extern Color C_LIGHT_CYAN;
    extern Color C_LIGHT_WHITE;

}

#define C_LIGHT_GRAY C_DARK_WHITE
#define C_DARK_GRAY C_LIGHT_BLACK
#define C_WHITE C_LIGHT_WHITE
#define C_BLACK C_DARK_BLACK

#endif /* defined(__Underneath__Color__) */
