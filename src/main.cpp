//
//  main.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/10/15.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "init.hpp"

int main(int argc, char* argv[]) {

    bool* running = init(argc, argv);
    while(*running){
        update();
    }
    return cleanup();
}
