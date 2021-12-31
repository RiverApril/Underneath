//
//  MenuEscape.hpp
//  Underneath
//
//  Created by Emily Atlee on 5/26/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef MenuEscape_hpp
#define MenuEscape_hpp

#include "Menu.hpp"
#include "MenuGame.hpp"

namespace Ui {
    
    class MenuEscape : public Menu {
    public:
        
        MenuEscape(MenuGame* game);
        
        ~MenuEscape();
        
        
        bool openUi();
        void closeUi();
        void handleInput(int in);
        void update();
        
        int menuTime = 0;
        MenuGame* menuGame = nullptr;
        int selection = 0;
    };
}

#endif /* MenuEscape_hpp */
