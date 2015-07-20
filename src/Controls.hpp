//
//  Controls.h
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Controls__
#define __Underneath__Controls__

#include "Global.hpp"

#define keyCatPlayer 1
#define keyCatUi 2

struct KeyBind{
    int* key = nullptr;
    int catagory = 0;
    string name = "UNDEFINED";

    KeyBind(string name, int* key, int catagory){
        this->name = name;
        this->key = key;
        this->catagory = catagory;
    }
};

namespace Key {

    //In Game
    extern int moveUp;
    extern int moveDown;
    extern int moveLeft;
    extern int moveRight;
    extern int interact;
    extern int secondaryAttack;
    extern int inspect;
    extern int adjustConsole;
    extern int waitUntilHealed;

    //Open UI
    extern int inventory;
    extern int statsMenu;
    extern int debugMenu;



    //In Ui
    extern int uiUp;
    extern int uiDown;
    extern int uiLeft;
    extern int uiRight;

    //In inventory
    extern int drop;
    extern int dropAll;
    extern int equip;
    extern int equipPrimary;
    extern int sortInv;

    //In chest
    extern int take;
    extern int takeStack;
    extern int takeAll;

}

extern vector<KeyBind> keybindings;

extern vector<int> reservedKeys;

#endif /* defined(__Underneath__Controls__) */
