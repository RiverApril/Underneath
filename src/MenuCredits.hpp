//
//  MenuCredits.hpp
//  Underneath
//
//  Created by Braeden Atlee on 12/30/15.
//  Copyright © 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuCredits__
#define __Underneath__MenuCredits__

#include "Menu.hpp"

namespace Ui {

    class MenuCredits : public Menu {
    public:

        MenuCredits();

        ~MenuCredits() {
        }

        void handleInput(int in);
        void update();
        
    };
}

#endif /* MenuCredits_hpp */
