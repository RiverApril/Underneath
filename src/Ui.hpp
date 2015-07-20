//
//  Ui.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Ui__
#define __Underneath__Ui__

#define escapeColorCode '\377'

#include "Global.hpp"
#include "Geometry.hpp"
#include "Color.hpp"
#include "Enchantment.hpp"

class World;

extern vector<string> consoleBuffer;

void debug(string s);
void debugf(string format, ...);
void console(string s);
void consolef(string format, ...);
string formatString(string format, ...);
string plural(string n);

class EntityPlayer;
class Inventory;

namespace Ui {


    extern const Color COLOR_DEFAULT_ENTITY;

    void initNCurses();
    void initColorPairs();

    void exitProgram();

    void setTerminalSizeVar();

    void setColor(Color fg, Color bg, int attr = 0);
    void setColor(Color fg);
    int getColorPair(Color fg, Color bg);

    //Color codeToColor(char c);

    int printEnchantments(int a, vector<Enchantment> enchantments, int columnX);

    bool addChColor(char c, bool* lookingForCode);

    int stringLengthWithColored(string s);

    int printMultiLineColoredString(int y, int x, string s, int maxX = -1);

    int printMultiLineString(int y, int x, string s, int maxX = -1);

    void drawInventory(World* currentWorld, EntityPlayer* player, int selectedY/*, int scrollOffset*/, Inventory* secondaryInv = nullptr, string invDisplayName = "", bool selectedLeft = true);

    extern unsigned long tick;

    /*extern double ms;

    extern int fps;
    extern long l;
    extern long lastL;
    extern long startTime;
    extern int frames;*/

    extern bool limitedColorMode;

    extern Point2 terminalSize;

    extern bool consoleInputMode;
    extern string consoleInput;
    extern int consoleScroll;


}

#endif /* defined(__Underneath__Ui__) */
