//
//  Utility.h
//  Underneath
//
//  Created by Braeden Atlee on 10/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Utility__
#define __Underneath__Utility__

#include "Global.h"


namespace Utility{

	template <typename T> void deleteContentsOfVector(std::vector<T*>* v){

        typename std::vector<T*>::iterator it;
        for(it = v->begin(); it!=v->end();) {
            delete * it;
            it = v->erase(it);
        }
    }
}


#endif /* defined(__Underneath__Utility__) */
