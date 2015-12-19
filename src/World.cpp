//
//  World.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "World.hpp"
#include "Utility.hpp"
#include "ItemGenerator.hpp"
#include "ItemSpecial.hpp"
#include "ItemTimeActivated.hpp"
#include "MenuGame.hpp"

World::World(string n) {
    levels = vector<string>();
    this->name = n;
}

World::~World() {

    if(currentLevel != nullptr){
        delete currentLevel;
    }

    currentLevel = nullptr;
    currentPlayer = nullptr;
}

namespace WorldLoader {

    //World* loadedWorld;

    bool exists(string name) {
        //debug("Does "+name+" Exist?");

        string dir = WorldsDir + "/" + name + "/";

        bool l = false;

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = fopen((dir + "world" + ".info").c_str(), "rb");
        if (fileWorldInfo != nullptr) {
            l = true;
            fclose(fileWorldInfo);
        }
        //

        //debug(l?"Yes ":"No ");

        return l;
    }

    vector<unsigned char>* readData(FILE* file) {
        fseek(file, 0, SEEK_END);
        size_t fileSize = ftell(file);
        rewind(file);

        unsigned char buffer[fileSize];
        size_t r = fread(buffer, fileSize, 1, file);
        if(r != fileSize){
            throw Utility::FileExceptionLoad("File size was not equal to the expected.");
        }
        /*printf("Length: %ld\n", fileSize);
        for(int i=0;i<fileSize;i++){
            printf("%X, ", buffer[i]);
        }*/
        return new vector<unsigned char>(buffer, buffer+fileSize);
    }

    World* load(World* world, string name, string optionalStartLevel) {

        try {
            debug("Loading World: " + name + "...");

            string dir = WorldsDir + "/" + name + "/";

            //world.info
            FILE* fileWorldInfo;

            fileWorldInfo = fopen((dir + "world.info").c_str(), "rb");
            if (fileWorldInfo != nullptr) {

                try {

                    vector<unsigned char>* data = readData(fileWorldInfo);

                    int* position = new int(0);

                    if(world == nullptr){
                        world = new World(name);
                    }else{
                        world->name = name;
                    }

                    world->seed = Utility::loadType<unsigned int>(data, position);

                    world->worldTime = Utility::loadDouble(data, position);
                    world->worldLastTime = world->worldTime;

                    int levelCount = Utility::loadInt(data, position);

                    string currentLevelName = Utility::loadString(data, position);
                    //int playerUniqueId = Utility::loadInt(data, position);

                    world->currentLevel = nullptr;

                    for (int i = 0; i < levelCount; i++) {
                        string levelName;

                        levelName = Utility::loadString(data, position);

                        world->levels.push_back(levelName);
                        if (levelName.compare(optionalStartLevel) == 0) {
                            currentLevelName = levelName;
                        }
                    }

                    //throw Utility::FileExceptionLoad("No current level");

                    //levelName.lvl
                    FILE* fileLevel;

                    fileLevel = fopen((dir + (currentLevelName) + ".lvl").c_str(), "rb");

                    if (fileLevel != nullptr) {

                        try{

                            vector<unsigned char>* levelData = readData(fileLevel);

                            int* levelPosition = new int(0);

                            Point2 levelSize = Point2::load(levelData, levelPosition);

                            int levelDifficulty = Utility::loadInt(levelData, levelPosition);

                            Level* level = new Level(world, currentLevelName, levelSize, levelDifficulty);

                            level->load(levelData, levelPosition);

                            world->currentLevel = level;

                            delete levelData;

                            delete levelPosition;

                        }catch(int e){
                            debugf("Failed to load level");
                        }

                    }

                    fclose(fileLevel);
                    //

                    if (world->currentLevel != nullptr) {
                        world->currentPlayer = dynamic_cast<EntityPlayer*> (Entity::loadNew(data, position));
                        world->currentLevel->newEntity(world->currentPlayer, false);
                        debugf("Loaded Entity Player with uid: %d", world->currentPlayer->uniqueId);
                    }

                    /*if (world->currentLevel != nullptr) {
                        for (size_t i = 0; i < world->currentLevel->entityCount(); i++) {
                            if (world->currentLevel->entityList[i]->uniqueId == playerUniqueId) {
                                world->currentPlayer = dynamic_cast<EntityPlayer*> (world->currentLevel->entityList[i]);
                                debugf("Setting currentPlayer to entity with uid: %d", world->currentPlayer->uniqueId);
                            }
                        }
                     }*/

                    delete data;

                    delete position;

                }catch(int e){
                    debugf("Failed to load level");
                }
            }
            fclose(fileWorldInfo);
            //


            debug(world == nullptr ? "Load Failed" : "Loaded");

            return world;
        } catch (Utility::FileExceptionLoad e) {
            debug("Load Failed: " + e.description);
        }
        return nullptr;

    }

    bool save(World* loadedWorld) {
        debug("Saving World: " + (loadedWorld->name) + "...");
        bool failed = false;

        mkdir(UnderneathDir.c_str(), 0777);
        mkdir(WorldsDir.c_str(), 0777);
        string dir = WorldsDir + "/" + loadedWorld->name.c_str() + "/";
        mkdir(dir.c_str(), 0777);


        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = fopen((dir + "world" + ".info").c_str(), "wb");
        if (fileWorldInfo != nullptr) {
            vector<unsigned char>* data = new vector<unsigned char>();

            Utility::saveType(data, loadedWorld->seed);

            Utility::saveDouble(data, loadedWorld->worldTime);

            Utility::saveInt(data, (int)loadedWorld->levels.size());

            Utility::saveString(data, loadedWorld->currentLevel->getName());
            //Utility::saveInt(data, loadedWorld->currentPlayer->uniqueId);

            for (size_t j = 0; j < loadedWorld->levels.size(); j++) {
                Utility::saveString(data, loadedWorld->levels[j]);
            }

            debug("Saving Player to world.info");
            loadedWorld->currentPlayer->save(data);


            for (size_t j = 0; j < data->size(); j++) {
                fputc(data->at(j), fileWorldInfo);
            }

            delete data;


            //levelName.lvl
            FILE* fileLevel;

            Level* l = loadedWorld->currentLevel;

            remove((dir + (l->getName()) + ".lvl").c_str());


            fileLevel = fopen((dir + (l->getName()) + ".lvl").c_str(), "wb");


            if (fileLevel != nullptr) {
                vector<unsigned char>* data = new vector<unsigned char>();

                l->save(data);

                for (size_t j = 0; j < data->size(); j++) {
                    fputc(data->at(j), fileLevel);
                }

                delete data;
            } else {
                failed = true;
            }
            fclose(fileLevel);
            //

            /*for(int i=0;i<loadedWorld->levels.size();i++){
                Level* l = loadedWorld->levels.at(i);

                //levelName.lvl
                FILE* fileLevel;

                remove((dir+(l->getName())+".lvl").c_str());

                fileLevel = fopen((dir+(l->getName())+".lvl").c_str(), "wb");
                if(fileLevel != nullptr){
                    vector<unsigned char>* data = new string();


                    l->save(data);

                    for(int j=0;j<data->size();j++){
                        fputc(data->at(j), fileLevel);
                    }

                    delete data;
                }else{
                    failed = true;
                    break;
                }
                fclose(fileLevel);
                //
            }*/

        } else {
            failed = true;
        }
        fclose(fileWorldInfo);
        //

        debug(failed ? "Save Failed" : "Saved");

        return false;
    }

    bool deleteWorld(string name) {
        debug("Deleting World: " + name + "...");

        string dir = WorldsDir + "/" + name + "/";

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = fopen((dir + "world.info").c_str(), "rb");
        if (fileWorldInfo != nullptr) {

            try {

                vector<unsigned char>* data = readData(fileWorldInfo);

                int* position = new int(0);

                Utility::loadType<unsigned int>(data, position);
                /*double worldTime = */Utility::loadDouble(data, position); // worldTime
                int levelCount = Utility::loadInt(data, position); // levelCount
                /*string worldName = */Utility::loadString(data, position); // World Name
                //Utility::loadInt(data, position);


                for (int i = 0; i < levelCount; i++) {
                    string levelName = Utility::loadString(data, position);
                    //debugf("levelName: %s", levelName.c_str());
                    remove((dir + levelName + ".lvl").c_str());
                }

                //No need to load player


                delete position;


            }catch(Utility::FileExceptionLoad e){
                debugf("Failed to load level: "+e.description);
            }
            fclose(fileWorldInfo);
        }
        //

        remove((dir + "world" + ".info").c_str());
        remove(dir.c_str());

        debug("Deleted");

        return true;
    }

    World* create(string name, Abilities<int> playerAbilities) {

        debug("new World()");
        World* world = new World(name);

        debug("new Level()");
        world->currentLevel = new Level(world, "Surface", Point2(100, 100), 0);

        srand(static_cast<unsigned int> (time(NULL)));

        Point2 start;
        Point2 p;
        do {
            start = Point2((rand() % (world->currentLevel->getSize().x-20))+10, (rand() % (world->currentLevel->getSize().y-20))+10);
            world->seed = (unsigned int) rand();
            debug("currentLevel->generate()");
            p = world->currentLevel->generate(genTypeStartArea, world->seed, start, "");
        } while (!(p.x >= 0 && p.y >= 0));

        debug("Successful generation.");
        world->levels.push_back(world->currentLevel->getName());


        debug("new EntityPlayer()");
        world->currentPlayer = new EntityPlayer(name, '@', p, C_WHITE, playerAbilities);
        ItemWeapon* weapon = nullptr;
        do{
            if(weapon != nullptr){
                delete weapon;
            }
            ItemGenerator::WeaponBase* wepBase;
            do{
                wepBase = ItemGenerator::weaponList[rand()%(ItemGenerator::weaponList.size())];
            }while(wepBase == nullptr || wepBase->weaponType != wepMelee);
            weapon = ItemGenerator::createItemWeaponFromBase(wepBase, 0);
        }while((weapon->baseDamage / weapon->useDelay) < 5 || (weapon->baseDamage / weapon->useDelay) > 6);
        world->currentPlayer->setActiveItemWeapon(weapon);
        world->currentPlayer->addItem(new ItemSpecial(specialtyCompass));

        world->currentPlayer->addItems(ItemGenerator::makeLoot(ItemGenerator::lootProfilePlayer, world->currentLevel->getDifficulty(), (rand()%90)+10, 4, 5, 5));

        world->currentLevel->newEntity(world->currentPlayer);

        //save(world);

        return world;
    }

    bool changeLevel(World* world, Point2 entrance, string newName) {

        save(world);

        debugf("Looking for level %s...", newName.c_str());

        for (string level : world->levels) {
            if (level == newName) {
                debug("Level found, loading...");

                Level* oldCurrentLevel = world->currentLevel;

                load(world, world->name, newName);

                if(!world->currentLevel){
                    debug("Failed to load level");
                    world->currentLevel = oldCurrentLevel;
                    continue;
                }

                EntityPlayer* newEntityPlayer = dynamic_cast<EntityPlayer*> (EntityPlayer::clone(world->currentPlayer));
                world->currentPlayer = newEntityPlayer;
                world->currentPlayer->pos = entrance;
                debugf("CurrentPlayer uid: %d", world->currentPlayer->uniqueId);
                forVector(world->currentLevel->entityList, i){
                    if(world->currentLevel->entityList[i]->uniqueId == newEntityPlayer->uniqueId){
                        Entity* e = world->currentLevel->entityList[i];
                        delete e;
                        world->currentLevel->entityList[i] = newEntityPlayer;
                    }
                    if(dynamic_cast<EntityPlayer*>(world->currentLevel->entityList[i])){
                        debugf("EntityPlayer uid: %d", world->currentLevel->entityList[i]->uniqueId);
                    }
                }
                return true;
            }
        }
        debug("Level not found, creating a new one...");
		
        int newDifficulty = world->currentLevel->getDifficulty() + 1;
        Point2 newSize = (world->currentLevel->getSize() * 1.1).truncate();
        EntityPlayer* newEntityPlayer = dynamic_cast<EntityPlayer*> (EntityPlayer::clone(world->currentPlayer));

        world->currentLevel->actuallyRemoveEntityUnsafe(world->currentPlayer, true);
        string oldName = world->currentLevel->getName();
        delete world->currentLevel;
        world->currentLevel = nullptr;

        Level* newLevel = new Level(world, newName, newSize, newDifficulty);

        newLevel->generate(genTypeDungeon, world->seed, entrance, oldName);

        world->currentPlayer = newEntityPlayer;
        world->currentPlayer->pos = entrance;
        newLevel->newEntity(world->currentPlayer, false);

        world->levels.push_back(newLevel->getName());
        world->currentLevel = newLevel;


        return true;

    }
}
