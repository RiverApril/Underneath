//
//  Color.h
//  Underneath
//
//  Created by Braeden Atlee on 4/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Color__
#define __Underneath__Color__

#if defined(WIN32) && !defined(useSDLLightCurses)
    #define C_DARK_BLACK 0x0 // white when background is black
    #define C_DARK_BLUE 0x1
    #define C_DARK_GREEN 0x2
    #define C_DARK_CYAN 0x3
    #define C_DARK_RED 0x4
    #define C_DARK_MAGENTA 0x5
    #define C_DARK_YELLOW 0x6
    #define C_DARK_WHITE 0x7

    #define C_LIGHT_BLACK 0x8
    #define C_LIGHT_BLUE 0x9
    #define C_LIGHT_GREEN 0xA
    #define C_LIGHT_CYAN 0xB
    #define C_LIGHT_RED 0xC
    #define C_LIGHT_MAGENTA 0xD
    #define C_LIGHT_YELLOW 0xE
    #define C_LIGHT_WHITE 0xF
#else
    #define C_DARK_BLACK 0x0 // white when background is black
    #define C_DARK_RED 0x1
    #define C_DARK_GREEN 0x2
    #define C_DARK_YELLOW 0x3
    #define C_DARK_BLUE 0x4
    #define C_DARK_MAGENTA 0x5
    #define C_DARK_CYAN 0x6
    #define C_DARK_WHITE 0x7

    #define C_LIGHT_BLACK 0x8
    #define C_LIGHT_RED 0x9
    #define C_LIGHT_GREEN 0xA
    #define C_LIGHT_YELLOW 0xB
    #define C_LIGHT_BLUE 0xC
    #define C_LIGHT_MAGENTA 0xD
    #define C_LIGHT_CYAN 0xE
    #define C_LIGHT_WHITE 0xF
#endif

#define C_LIGHT_GRAY C_DARK_WHITE
#define C_DARK_GRAY C_LIGHT_BLACK
#define C_WHITE C_LIGHT_WHITE
#define C_BLACK C_DARK_BLACK

namespace Ui {

    typedef unsigned char Color;

}

#endif /* defined(__Underneath__Color__) */
