//
//  Controls.h
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Controls__
#define __Underneath__Controls__

#include "Global.h"

namespace Key{

	//In Game
    extern int moveUp;
    extern int moveDown;
    extern int moveLeft;
    extern int moveRight;
    extern int interact;
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

    //In chest
    extern int take;
    extern int takeStack;
    extern int takeAll;

}

#endif /* defined(__Underneath__Controls__) */
