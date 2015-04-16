//
//  Icon.h
//  Underneath
//
//  Created by Braeden Atlee on 4/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Icon__
#define __Underneath__Icon__

#include "Geometry.h"
#include "Color.h"

class Level;

class Icon{
    
public:
    Icon(){

    }
    virtual ~Icon(){

    }
    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl){
        return '?';
    }
    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return Ui::C_WHITE;
    }
    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return Ui::C_BLACK;
    }
};

class BasicIcon : public Icon{
    char i = '?';
    Ui::Color fg = Ui::C_DARK_BLACK;
    Ui::Color bg = Ui::C_LIGHT_WHITE;

public:
    BasicIcon(char i, Ui::Color fg, Ui::Color bg){
        this->i = i;
        this->fg = fg;
        this->bg = bg;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl){
        return i;
    }

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fg;
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return bg;
    }
};

class AnimatedIcon : public Icon{
    int speed = 1;
    vector<char> i = {'?'};
    Ui::Color fg = Ui::C_DARK_BLACK;
    Ui::Color bg = Ui::C_LIGHT_WHITE;

public:
    AnimatedIcon(vector<char> i, int speed, Ui::Color fg, Ui::Color bg){
        this->i = i;
        this->speed = speed;
        this->fg = fg;
        this->bg = bg;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl){
        return i[(tick/speed)%i.size()];
    }

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fg;
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return bg;
    }
};

class ColorAnimatedIcon : public Icon{
    int speed = 1;
    vector<char> i = {'?'};
    vector<Ui::Color> fg = {Ui::C_DARK_BLACK};
    vector<Ui::Color> bg = {Ui::C_LIGHT_WHITE};

public:
    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl){
        return i[(tick/speed)%i.size()];
    }

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fg[(tick/speed)%fg.size()];
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return bg[(tick/speed)%bg.size()];
    }
};

#endif /* defined(__Underneath__Icon__) */
