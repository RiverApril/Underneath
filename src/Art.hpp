//
//  Art.hpp
//  Underneath
//
//  Created by Emily Atlee on 2/1/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Art__
#define __Underneath__Art__

#include "Global.hpp"
#include "Geometry.hpp"

struct Art {

    Art(string name) : name(name){}

    Point2 calcSize();
    Point2 getSize();

    void printAt(Point2 pos);

    void printCenter(int y, int xOffset = 0);

    vector<string> printToVector(int xOffset = 0);

    vector<string> lines;
    Point2 size;
    string name;

};

namespace Arts {

    void loadArts();
    void cleanupArts();

    int loadNew(string location, string name, vector<string> fallback, string ext = "txt", string dot = ".");

    Art* getArt(int index);

    extern vector<Art*> artList;

    extern Art* defaultArt;

    extern vector<int> compassInList;
    extern int compassOut;
    extern Point2 compassInOffset;

    extern int artTitle;
    extern int artGameOver;
    extern int artLevelUp;

    extern int artCoin;
    extern int artSmallKey;

    extern int artScroll;
    extern int artScrollFire;
    extern int artScrollFrost;
    extern int artScrollShock;
    extern int artScrollHand;
    extern int artScrollHeal;
    extern int artScrollPerson;
    extern int artScrollShield;
    extern int artScrollSwap;

    extern int artPotion1;
    extern int artPotion2;
    extern int artPotion3;
    extern int artPotion4;
    
    extern int artBomb;
    extern int artMolotovCocktail;
    extern int artLandMine;
    extern int artEasterEgg;

    extern int artKnife;
    extern int artDagger;
    extern int artCleaver;

    extern int artLongsword;
    extern int artCutlass;
    extern int artKatana;
    extern int artMachete;
    extern int artGladius;
    extern int artScimitar;
    extern int artRapier;
    extern int artShortSword;
    extern int artBroadsword;
    extern int artSaber;
    extern int artClaymore;

    extern int artAxe;
    extern int artDoubleAxe;

    extern int artMace;
    extern int artClub;
    extern int artFlail;
    extern int artWarHammer;

    extern int artSpear;
    extern int artHalberd;

    extern int artLongbow;
    extern int artRecurveBow;
    extern int artCrossbow;


    extern int artBoots;
    extern int artCuirass;
    extern int artGloves;
    extern int artGauntlets;
    extern int artBracers;
    extern int artCowl;

    extern int artRing;
    extern int artJewelRing;

}

#endif /* defined(__Underneath__Art__) */
