//
//  MenuDebug.hpp
//  Underneath
//
//  Created by Emily Atlee on 2/18/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__MenuDebug__
#define __Underneath__MenuDebug__

#include "Menu.hpp"
#include "EntityAlive.hpp"
#include "World.hpp"

#define column0 0
#define column1 (column0+20)
#define column2 (column1+10)
#define column3 (column2+10)

namespace Ui {

    class MenuDebug : public Menu {
    public:

        MenuDebug();

        ~MenuDebug() {
        }

        void handleInput(int in);
        void update();

    };
}

#endif /* defined(__Underneath__MenuDebug__) */
