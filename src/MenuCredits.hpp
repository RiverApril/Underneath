//
//  MenuCredits.hpp
//  Underneath
//
//  Created by Braeden Atlee on 12/30/15.
//  Copyright © 2015 Braeden Atlee. All rights reserved.
//

#ifndef MenuCredits_hpp
#define MenuCredits_hpp

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
