//
//  Potion.h
//  Underneath
//
//  Created by Braeden Atlee on 3/2/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Potion__
#define __Underneath__Potion__

#include "Global.h"
#include "Item.h"
#include "Effect.h"

class Potion : public Item {
public:

	static Potion* cloneUnsafe(Potion* oldE, Potion* newE = nullptr);

	Potion() : Item(){

	}

	Potion(string name, double weight) : Item(name, weight){

	}
	
	Potion(Effect effect, string name, double weight) : Item(name, weight){
        this->effects = {effect};
	}

	Potion(vector<Effect> effects, string name, double weight) : Item(name, weight){
		this->effects = effects;
	}

	virtual int getItemTypeId(){
		return ITEM_TYPE_POTION;
	}

	virtual void save(vector<unsigned char>* data);

	virtual void load(unsigned char* data, int* position);

	virtual bool equalsExceptQty(Item* other){
		Potion* otherW = dynamic_cast<Potion*>(other);
		return Item::equalsExceptQty(other)
			&& (otherW)
			&& (effects == otherW->effects);
	}

	vector<Effect> effects;

	virtual string getExtendedName(){
		return name;
	}

protected:
};

#endif /* defined(__Underneath__Potion__) */
