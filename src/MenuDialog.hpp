//
//  MenuDialog.hpp
//  Underneath
//
//  Created by Braeden Atlee on 12/30/15.
//  Copyright © 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuDialog__
#define __Underneath__MenuDialog__

#include "Menu.hpp"

namespace Ui {

    class MenuDialog : public Menu {
    public:

        MenuDialog(vector<string> messageList, vector<string> dialogOptions, function<void(MenuDialog*, int)> afterClose, bool escapable = true, int defaultSelect = 0);

        ~MenuDialog() {
        }

        void handleInput(int in);
        void update();

        int selected;
        function<void(MenuDialog*, int)> afterClose;
        bool escapable;
        vector<string> messageList;
        vector<string> dialogOptions;

        
    };
}

#endif /* MenuDialog_hpp */
