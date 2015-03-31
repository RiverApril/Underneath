//
//  Equipable.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Equipable.h"
#include "Utility.h"

Equipable::Equipable(string name, double weight) : Item(name, weight){

}

void Equipable::save(vector<unsigned char>* data){
    Item::save(data);

    minimumAbilities.save(data);
}

void Equipable::load(unsigned char* data, int* position){
    Item::load(data, position);

    minimumAbilities.load(data, position);
}

Equipable* Equipable::cloneUnsafe(Equipable* oldE, Equipable* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->minimumAbilities = oldE->minimumAbilities;

    return newE;

}

bool Equipable::equalsExceptQty(Item* other){
    Equipable* otherW = dynamic_cast<Equipable*>(other);
    return Item::equalsExceptQty(other)
    &&(otherW)
    &&(otherW->minimumAbilities==minimumAbilities);
}
