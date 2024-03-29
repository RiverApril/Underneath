//
//  Art.cpp
//  Underneath
//
//  Created by Emily Atlee on 2/1/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "Art.hpp"
#include "Math.hpp"
#include "Menu.hpp"
#include "Utility.hpp"
#include "ArtFiles.hpp"


namespace Arts {

    vector<Art*> artList;

    Art* defaultArt;

    vector<int> compassInList;
    int compassOut;
    Point2 compassInOffset;

    int artTitle;
    int artGameOver;
    int artLevelUp;

    int artCoin;
    int artSmallKey;

    int artScroll;
    int artScrollFire;
    int artScrollFrost;
    int artScrollShock;
    int artScrollHand;
    int artScrollHeal;
    int artScrollPerson;
    int artScrollShield;
    int artScrollSwap;

    int artPotion1;
    int artPotion2;
    int artPotion3;
    int artPotion4;

    int artBomb;
    int artMolotovCocktail;
    int artLandMine;
    int artEasterEgg;

    int artKnife;
    int artDagger;
    int artCleaver;

    int artLongsword;
    int artCutlass;
    int artKatana;
    int artMachete;
    int artGladius;
    int artScimitar;
    int artRapier;
    int artShortSword;
    int artBroadsword;
    int artSaber;
    int artClaymore;

    int artAxe;
    int artDoubleAxe;

    int artMace;
    int artClub;
    int artFlail;
    int artWarHammer;

    int artSpear;
    int artHalberd;


    int artLongbow;
    int artRecurveBow;
    int artCrossbow;

    int artBoots;
    int artCuirass;
    int artGloves;
    int artGauntlets;
    int artBracers;
    int artCowl;

    int artRing;
    int artJewelRing;

    void loadArts() {

        //mkdir(ArtDir.c_str(), 0777);

        defaultArt = new Art("Default Art");
        defaultArt->lines.push_back("Failed to load art.");

        Arts::artTitle = Arts::loadNew(ArtDir + "/", "title", ArtFiles::title);
        Arts::artGameOver = Arts::loadNew(ArtDir + "/", "gameover", ArtFiles::gameover);
        Arts::artLevelUp = Arts::loadNew(ArtDir + "/", "levelup", ArtFiles::levelup);

        Arts::artCoin = Arts::loadNew(ArtDir + "/", "coin", ArtFiles::coin);
        Arts::artSmallKey = Arts::loadNew(ArtDir + "/", "smallkey", ArtFiles::smallkey);


        Arts::artScroll = Arts::loadNew(ArtDir + "/", "scroll", ArtFiles::scroll);
        Arts::artScrollFire = Arts::loadNew(ArtDir + "/", "scrollFire", ArtFiles::scrollFire);
        Arts::artScrollFrost = Arts::loadNew(ArtDir + "/", "scrollFrost", ArtFiles::scrollFrost);
        Arts::artScrollShock = Arts::loadNew(ArtDir + "/", "scrollShock", ArtFiles::scrollShock);
        Arts::artScrollHand = Arts::loadNew(ArtDir + "/", "scrollHand", ArtFiles::scrollHand);
        Arts::artScrollHeal = Arts::loadNew(ArtDir + "/", "scrollHeal", ArtFiles::scrollHeal);
        Arts::artScrollPerson = Arts::loadNew(ArtDir + "/", "scrollPerson", ArtFiles::scrollPerson);
        Arts::artScrollShield = Arts::loadNew(ArtDir + "/", "scrollShield", ArtFiles::scrollShield);
        Arts::artScrollSwap = Arts::loadNew(ArtDir + "/", "scrollSwap", ArtFiles::scrollSwap);

        Arts::artPotion1 = Arts::loadNew(ArtDir + "/", "potion1", ArtFiles::potion1);
        Arts::artPotion2 = Arts::loadNew(ArtDir + "/", "potion2", ArtFiles::potion2);
        Arts::artPotion3 = Arts::loadNew(ArtDir + "/", "potion3", ArtFiles::potion3);
        Arts::artPotion4 = Arts::loadNew(ArtDir + "/", "potion4", ArtFiles::potion4);

        Arts::artBomb = Arts::loadNew(ArtDir + "/", "bomb", ArtFiles::bomb);
        Arts::artMolotovCocktail = Arts::loadNew(ArtDir + "/", "molotovCocktail", ArtFiles::molotovCocktail);
        Arts::artLandMine = Arts::loadNew(ArtDir + "/", "landMine", ArtFiles::landMine);
        Arts::artEasterEgg = Arts::loadNew(ArtDir + "/", "easterEgg", ArtFiles::easterEgg);

        Arts::artKnife = Arts::loadNew(ArtDir + "/", "knife", ArtFiles::knife);
        Arts::artDagger = Arts::loadNew(ArtDir + "/", "dagger", ArtFiles::dagger);
        Arts::artCleaver = Arts::loadNew(ArtDir + "/", "cleaver", ArtFiles::cleaver);

        Arts::artLongsword = Arts::loadNew(ArtDir + "/", "longsword", ArtFiles::longsword);
        Arts::artCutlass = Arts::loadNew(ArtDir + "/", "cutlass", ArtFiles::cutlass);
        Arts::artKatana = Arts::loadNew(ArtDir + "/", "katana", ArtFiles::katana);
        Arts::artMachete = Arts::loadNew(ArtDir + "/", "machete", ArtFiles::machete);
        Arts::artGladius = Arts::loadNew(ArtDir + "/", "gladius", ArtFiles::gladius);
        Arts::artScimitar = Arts::loadNew(ArtDir + "/", "scimitar", ArtFiles::scimitar);
        Arts::artRapier = Arts::loadNew(ArtDir + "/", "rapier", ArtFiles::rapier);
        Arts::artShortSword = Arts::loadNew(ArtDir + "/", "shortsword", ArtFiles::shortsword);
        Arts::artBroadsword = Arts::loadNew(ArtDir + "/", "broadsword", ArtFiles::broadsword);
        Arts::artSaber = Arts::loadNew(ArtDir + "/", "saber", ArtFiles::saber);
        Arts::artClaymore = Arts::loadNew(ArtDir + "/", "claymore", ArtFiles::claymore);


        Arts::artAxe = Arts::loadNew(ArtDir + "/", "axe", ArtFiles::axe);
        Arts::artDoubleAxe = Arts::loadNew(ArtDir + "/", "doubleaxe", ArtFiles::doubleaxe);

        Arts::artMace = Arts::loadNew(ArtDir + "/", "mace", ArtFiles::mace);
        Arts::artClub = Arts::loadNew(ArtDir + "/", "club", ArtFiles::club);
        Arts::artFlail = Arts::loadNew(ArtDir + "/", "flail", ArtFiles::flail);
        Arts::artWarHammer = Arts::loadNew(ArtDir + "/", "warhammer", ArtFiles::warhammer);

        Arts::artSpear = Arts::loadNew(ArtDir + "/", "spear", ArtFiles::spear);
        Arts::artHalberd = Arts::loadNew(ArtDir + "/", "halberd", ArtFiles::halberd);


        Arts::artLongbow = Arts::loadNew(ArtDir + "/", "longbow", ArtFiles::longbow);
        Arts::artRecurveBow = Arts::loadNew(ArtDir + "/", "recurvebow", ArtFiles::recurvebow);
        Arts::artCrossbow = Arts::loadNew(ArtDir + "/", "crossbow", ArtFiles::crossbow);


        Arts::artBoots = Arts::loadNew(ArtDir + "/", "boots", ArtFiles::boots);
        Arts::artCuirass = Arts::loadNew(ArtDir + "/", "cuirass", ArtFiles::cuirass);
        Arts::artGloves = Arts::loadNew(ArtDir + "/", "gloves", ArtFiles::gloves);
        Arts::artGauntlets = Arts::loadNew(ArtDir + "/", "gauntlets", ArtFiles::gauntlets);
        Arts::artBracers = Arts::loadNew(ArtDir + "/", "bracers", ArtFiles::bracers);
        Arts::artCowl = Arts::loadNew(ArtDir + "/", "cowl", ArtFiles::cowl);

        Arts::artRing = Arts::loadNew(ArtDir + "/", "ring", ArtFiles::ring);
        Arts::artJewelRing = Arts::loadNew(ArtDir + "/", "jewelRing", ArtFiles::jewelRing);

        Point2 compassInSize = Point2Neg1;

        //Compass

        {

            int fullCompassIn = loadNew(ArtDir + "/", "compassInner", ArtFiles::compassInner);

            Art* fullCompassInArt = getArt(fullCompassIn);

            Point2 fullInSize = fullCompassInArt->getSize();

            int w = 0;
            int h = 0;

            for(int i=0;i<fullInSize.x;i++){
                if(fullCompassInArt->lines[0][i] == '#'){
                    w++;
                }else{
                    break;
                }
            }
            compassInSize.x = w;

            for(int j=0;j<fullInSize.y;j++){
                if(fullCompassInArt->lines[j][0] == '#'){
                    h++;
                }else{
                    break;
                }
            }
            compassInSize.y = h;

			if (compassInSize.x > 0 && compassInSize.y > 0) {

				for (int j = compassInSize.y;j <= fullInSize.y;j += compassInSize.y) {
					Art* a = new Art("compassIn part");

					for (int i = 0;i < compassInSize.y;i++) {
						if (j + i < (int)fullCompassInArt->lines.size()) {
							a->lines.push_back(fullCompassInArt->lines[j + i]);
						}
					}

					a->calcSize();
					artList.push_back(a);
					compassInList.push_back(((int)artList.size()) - 1);
				}

			}

        }

        //
        {
            int fullCompassOut = loadNew(ArtDir + "/", "compassOuter", ArtFiles::compassOuter);

            Art* fullCompassOutArt = getArt(fullCompassOut);

            Point2 fullOutSize = fullCompassOutArt->getSize();

            for(int i=0;i<fullOutSize.x;i++){
                if(fullCompassOutArt->lines[0][i] == '#'){
                    compassInOffset.x++;
                }else{
                    break;
                }
            }

            for(int j=0;j<fullOutSize.y;j++){
                if(fullCompassOutArt->lines[j][0] == '#'){
                    compassInOffset.y++;
                }else{
                    break;
                }
            }
            Art* a = new Art("CompassOut");

            for(int i=compassInOffset.y;i<fullOutSize.y;i++){
                a->lines.push_back(fullCompassOutArt->lines[i]);
            }

            a->calcSize();
            artList.push_back(a);
            compassOut = ((int)artList.size())-1;
        }

        //end




    }

    void cleanupArts() {
        delete defaultArt;
        for (Art* a : artList) {
            delete a;
        }
        artList.clear();
    }

    int loadNew(string location, string name, vector<string> fallback, string ext, string dot) {
        string path = location + name + dot + ext;

        Art* art = new Art(name);


        FILE* file;
        file = fopen(path.c_str(), "r");
        if (file != NULL) {
            int c;
            string line = "";
            do {
                c = fgetc(file);
                if (c == '\n') {
                    art->lines.push_back(line);
                    line = "";
                } else {
                    line += (char) c;
                }
            } while (c != EOF);
            //art->lines.push_back(line);
            fclose(file);
        } else {
            debug("Failed to load: \"" + path + "\" using default.");
            art->lines = fallback;
        }
        art->calcSize();
        for(int i=0;i<art->lines.size();i++){
            while(art->lines[i].size() < art->getSize().x){
                art->lines[i] += " ";
            }
        }
        artList.push_back(art);
        return ((int) artList.size()) - 1;
    }

    Art* getArt(int index) {
        if (index >= 0 && index < (int)artList.size()) {
            return artList[index];
        } else {
            return defaultArt;
        }
    }

}

Point2 Art::calcSize() {
    int w = 0;
    for (string line : lines) {
        w = max(w, (int) line.length());
    }
    size.x = w;
    size.y = (int) lines.size();
    return size;
}

Point2 Art::getSize() {
    return size;
}

void Art::printAt(Point2 pos) {
    for (int i = 0; i < lines.size(); i++) {
        mvaddstr(pos.y + i, pos.x, lines[i].c_str());
    }
}

void Art::printCenter(int y, int xOffset) {
    int w = getSize().x;
    for (int i = 0; i < (int)lines.size(); i++) {
        mvprintw(y + i, (Ui::terminalSize.x / 2)+xOffset-((w - 1) / 2), lines[i].c_str());
    }
}

vector<string> Art::printToVector(int xOffset){
    vector<string> s;
    int w = getSize().x;
    for (string line : lines) {
        s.push_back(line + string(w-line.length(), ' '));
    }
    return s;
}
