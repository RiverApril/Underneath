//
//  Icon.hpp
//  Underneath
//
//  Created by Emily Atlee on 4/15/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Icon__
#define __Underneath__Icon__

#include "Geometry.hpp"
#include "Color.hpp"

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
        return C_WHITE;
    }
    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return C_BLACK;
    }
};

class BasicIcon : public Icon{
    char i = '?';
    Ui::Color fg = C_DARK_BLACK;
    Ui::Color bg = C_LIGHT_WHITE;

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

class RandomIcon : public Icon{
    vector<char> i = {'?'};
    Ui::Color fg = C_DARK_BLACK;
    Ui::Color bg = C_LIGHT_WHITE;

public:
    RandomIcon(vector<char> i, Ui::Color fg, Ui::Color bg){
        this->i = i;
        this->fg = fg;
        this->bg = bg;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fg;
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return bg;
    }
};

class RandomColorIcon : public Icon{
    vector<char> i = {'?'};
    vector<Ui::Color> fg = {C_DARK_BLACK};
    vector<Ui::Color> bg = {C_LIGHT_WHITE};

public:
    RandomColorIcon(vector<char> i, vector<Ui::Color> fg, vector<Ui::Color> bg){
        this->i = i;
        this->fg = fg;
        this->bg = bg;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl);

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl);
};

class AnimatedIcon : public Icon{
    Ui::Color fg = C_DARK_BLACK;
    Ui::Color bg = C_LIGHT_WHITE;
    bool randomStart = false;

public:
    int speed = 1;
    vector<char> i = {'?'};

    AnimatedIcon(vector<char> i, int speed, Ui::Color fg, Ui::Color bg, bool randomStart = false){
        this->i = i;
        this->speed = speed;
        this->fg = fg;
        this->bg = bg;
        this->randomStart = randomStart;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fg;
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return bg;
    }
};

class ColorAnimatedIcon : public Icon{
    vector<Ui::Color> fg = {C_DARK_BLACK};
    vector<Ui::Color> bg = {C_LIGHT_WHITE};
    bool randomStart = false;

public:
    int speed = 1;
    vector<char> i = {'?'};
    
    ColorAnimatedIcon(vector<char> i, int speed, vector<Ui::Color> fg, vector<Ui::Color> bg, bool randomStart = false){
        this->i = i;
        this->speed = speed;
        this->fg = fg;
        this->bg = bg;
        this->randomStart = randomStart;
    }

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual Ui::Color getFgColor(unsigned long tick, Point2 pos, Level* lvl){
        return fg[(tick/speed)%fg.size()];
    }

    virtual Ui::Color getBgColor(unsigned long tick, Point2 pos, Level* lvl){
        return bg[(tick/speed)%bg.size()];
    }
};

extern vector<Icon*> iconList;

#endif /* defined(__Underneath__Icon__) */
