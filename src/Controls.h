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
	const int moveUp = KEY_UP;
	const int moveDown = KEY_DOWN;
	const int moveLeft = KEY_LEFT;
	const int moveRight = KEY_RIGHT;
	const int interact = 'g';
	const int adjustConsole = '\\';
	const int wait5 = 'h';

	//Open UI
    const int inventory = 'i';
	const int statsMenu = 's';
	const int debugMenu = 'd';


	//In inventory
	const int drop = 'd';
	const int dropAll = 'D';
	const int equip = 'e';
	const int useOnWorld = 'u';

}

#endif /* defined(__Underneath__Controls__) */
