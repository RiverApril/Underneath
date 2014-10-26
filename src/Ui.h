//
//  Ui.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Ui__
#define __Underneath__Ui__


namespace Ui {

typedef int color;


const color C_DARK_BLACK = 0xE8;
const color C_DARK_RED = 0x1;
const color C_DARK_GREEN = 0x2;
const color C_DARK_YELLOW = 0x3;
const color C_DARK_BLUE = 0x4;
const color C_DARK_MAGENTA = 0x5;
const color C_DARK_CYAN = 0x6;
const color C_DARK_WHITE = 0x7;

const color C_LIGHT_BLACK = 0x8;
const color C_LIGHT_RED = 0x9;
const color C_LIGHT_GREEN = 0xA;
const color C_LIGHT_YELLOW = 0xB;
const color C_LIGHT_BLUE = 0xC;
const color C_LIGHT_MAGENTA = 0xD;
const color C_LIGHT_CYAN = 0xE;
const color C_LIGHT_WHITE = 0xF;

const color C_LIGHT_GRAY = C_DARK_WHITE;
const color C_DARK_GRAY = C_LIGHT_BLACK;

const color C_WHITE = C_LIGHT_WHITE;
const color C_BLACK = C_DARK_BLACK;


extern const int COLOR_DEFAULT_ENTITY;

void initNCurses();
void initColorPairs();

void exitProgram();

void setColor(color c, int attr = 0);

extern int tick;

extern double ms;

extern int fps;
extern long l;
extern long lastL;
extern long startTime;
extern int frames;

extern bool limitedColorMode;
extern bool unicodeSupport;



class Menu {
public:

    Menu(int inputTimeout);

    virtual ~Menu() {}

    void _openUi();
    void _handleInput(int i);
    void _update();

    virtual void openUi() {}
    virtual void handleInput(int i) {}
    virtual void update() {}

protected:
    bool inputTimeout;

};

class MenuMain : public Menu {
public:

    MenuMain() : Menu(100) {}

    ~MenuMain() {}

    void openUi();
    void handleInput(int in);
    void update();

    int selection = 0;
};

class MenuGame;




extern Menu* currentMenu;

void changeMenu(Menu* newMenu);


}

#endif /* defined(__Underneath__Ui__) */
