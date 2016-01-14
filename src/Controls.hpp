//
//  Controls.hpp
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
    int* key;
    int catagory;
    string name;

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
    extern int wait;
    extern int walk;
    extern int explore;
    extern int instantInteract;

    //Open UI
    extern int inventory;
    extern int statsMenu;



    //In Ui
    extern int uiUp;
    extern int uiDown;
    extern int uiLeft;
    extern int uiRight;
    extern int console;

    //In inventory
    extern int drop;
    //extern int dropAll;
    extern int equip;
    extern int sortInv;

    //In chest
    extern int take;
    extern int takeStack;

    extern int fav1;
    extern int fav2;
    extern int fav3;
    extern int fav4;
    extern int fav5;
    extern int fav6;
    extern int fav7;
    extern int fav8;
    extern int fav9;
    extern int fav0;

}

extern vector<KeyBind*> keybindings;

extern vector<int> reservedKeys;

string keyDisplayName(int key);

#endif /* defined(__Underneath__Controls__) */
