//
//  World.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "World.h"
#include "Utility.h"
#include "ItemGenerator.h"


World::World(string n){
    levels = vector<string>();
    this->name = n;
}

World::~World(){


}

namespace WorldLoader {

    //World* loadedWorld;

    bool exists(std::string name){
        //debug("Does "+name+" Exist?");

        std::string dir = WorldsDir+"/"+name+"/";

        bool l = false;

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
        if(fileWorldInfo != nullptr){
            l = true;
        }
        fclose(fileWorldInfo);
        //

        //debug(l?"Yes ":"No ");

        return l;
    }

    unsigned char* readData(FILE* file){
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        unsigned char* buffer = new unsigned char[fileSize];
        fread(buffer, fileSize, 1, file);
        /*printf("Length: %ld\n", fileSize);
        for(int i=0;i<fileSize;i++){
            printf("%X, ", (int)buffer[i]);
        }*/
        return buffer;
    }

    World* load(std::string name, string optionalStartLevel){

        try{
            debug("Loading World: "+name+"...");

            std::string dir = WorldsDir+"/"+name+"/";

            World* world = nullptr;

            //world.info
            FILE* fileWorldInfo;

            fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
            if(fileWorldInfo != nullptr){

                unsigned char* data = readData(fileWorldInfo);

                int* position = new int(0);

                world = new World(name);

                world->worldTime = FileUtility::loadDouble(data, position);

                int levelCount = FileUtility::loadInt8Bit(data, position);

                std::string currentLevelName = FileUtility::loadString(data, position);
                int playerUniqueId = FileUtility::loadInt(data, position);

                world->currentLevel = nullptr;

                for(int i=0;i<levelCount;i++){
                    std::string levelName;

                    levelName = FileUtility::loadString(data, position);

                    world->levels.push_back(levelName);
                    if(levelName.compare(optionalStartLevel) == 0){
                        currentLevelName = levelName;
                    }
                }

                //levelName.lvl
                FILE* fileLevel;

                fileLevel = std::fopen((dir+(currentLevelName)+".lvl").c_str(), "rb");

                if(fileLevel != nullptr){

                    unsigned char* levelData = readData(fileLevel);

                    int* levelPosition = new int(0);

                    Point2 levelSize = Point2::load(levelData, levelPosition);

                    Level* level = new Level(world, currentLevelName, levelSize);

                    level->load(levelData, levelPosition);

                    world->currentLevel = level;

                    delete levelData;
                    
                    delete levelPosition;
                    
                }
                
                fclose(fileLevel);
                //

                if(world->currentLevel != nullptr){
                    for(int i=0;i<world->currentLevel->entityCount();i++){
                        if(world->currentLevel->entityList[i]->uniqueId == playerUniqueId){
                            world->currentPlayer = dynamic_cast<Player*>(world->currentLevel->entityList[i]);
                        }
                    }
                }

                delete data;

                delete position;
            }
            fclose(fileWorldInfo);
            //


        	debug(world==nullptr?"Load Failed":"Loaded");

        	return world;
        }catch(FileUtility::FileExceptionLoad e){
            debug("Load Failed: "+e.description);
        }
        return nullptr;

    }

    bool save(World* loadedWorld){
        debug("Saving World: "+(loadedWorld->name)+"...");
        bool failed = false;

        mkdir(UnderneathDir.c_str(), 0777);
        mkdir(WorldsDir.c_str(), 0777);
        std::string dir = WorldsDir+"/"+loadedWorld->name.c_str()+"/";
        mkdir(dir.c_str(), 0777);

        std::remove((dir+"world"+".info.backup").c_str());
        std::rename((dir+"world"+".info").c_str(),
                    (dir+"world"+".info.backup").c_str());
        std::remove((dir+"world"+".info").c_str());

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "wb");
        if(fileWorldInfo != nullptr){
            vector<unsigned char>* data = new vector<unsigned char>();

            FileUtility::saveDouble(data, loadedWorld->worldTime);

            FileUtility::saveInt8Bit(data, (int8_t)loadedWorld->levels.size());

            FileUtility::saveString(data, loadedWorld->currentLevel->getName());
            FileUtility::saveInt(data, loadedWorld->currentPlayer->uniqueId);

            for(int j=0;j<loadedWorld->levels.size();j++){
            	FileUtility::saveString(data, loadedWorld->levels.at(j));
            }

            for(int j=0;j<data->size();j++){
                fputc(data->at(j), fileWorldInfo);
            }

            delete data;


            //levelName.lvl
            FILE* fileLevel;

            Level* l = loadedWorld->currentLevel;

            std::remove((dir+(l->getName())+".lvl.backup").c_str());
            std::rename((dir+(l->getName())+".lvl").c_str(),
                        (dir+(l->getName())+".lvl.backup").c_str());
            std::remove((dir+(l->getName())+".lvl").c_str());


            fileLevel = fopen((dir+(l->getName())+".lvl").c_str(), "wb");


            if(fileLevel != nullptr){
                vector<unsigned char>* data = new vector<unsigned char>();

                l->save(data);

                for(int j=0;j<data->size();j++){
                    fputc(data->at(j), fileLevel);
                }

                delete data;
            }else{
                failed = true;
            }
            fclose(fileLevel);
            //

            /*for(int i=0;i<loadedWorld->levels.size();i++){
                Level* l = loadedWorld->levels.at(i);

                //levelName.lvl
                FILE* fileLevel;

                std::remove((dir+(l->getName())+".lvl.backup").c_str());
                std::rename((dir+(l->getName())+".lvl").c_str(),
                            (dir+(l->getName())+".lvl.backup").c_str());
                std::remove((dir+(l->getName())+".lvl").c_str());

                fileLevel = fopen((dir+(l->getName())+".lvl").c_str(), "wb");
                if(fileLevel != nullptr){
                    std::vector<unsigned char>* data = new std::string();
                    

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

        }else{
            failed = true;
        }
        fclose(fileWorldInfo);
        //

        debug(failed?"Save Failed":"Saved");

        return false;
    }

    World* create(std::string name, Abilities<int> playerAbilities){

        World* world = new World(name);

        world->currentLevel = new Level(world, "Floor0", Point2(300, 300));

        srand(static_cast<unsigned int>(time(NULL)));

        Point2 start;
        Point2 p;
        do{
            start = Point2(rand()%world->currentLevel->getSize().x, rand()%world->currentLevel->getSize().y);
        	p = world->currentLevel->generate(rand(), start, "");
        }while(!(p.x >= 0 && p.y >= 0));

        world->levels.push_back(world->currentLevel->getName());


        world->currentPlayer = new Player(name, '@', p, Ui::C_WHITE, playerAbilities);
        world->currentPlayer->setActiveWeapon(ItemGenerator::createWeapon("", ItemGenerator::combat2Training, damMelee, false));
        world->currentPlayer->setActiveWeapon(ItemGenerator::createWeapon("", ItemGenerator::combat2Training, damRanged, false));

        for(int i=0;i<10;i++){
            int q = rand()%100;
            for(int j=0;j<q;j++){
                world->currentPlayer->pickupItem(new Item("Test Item "+to_string(i), .1));
            }
        }
        

        world->currentLevel->newEntity(world->currentPlayer);

        //save(world);

        return world;
    }

    bool deleteWorld(std::string name){
        debug("Deleting World: "+name+"...");

        std::string dir = WorldsDir+"/"+name+"/";

        //world.info
        FILE* fileWorldInfo;

        fileWorldInfo = std::fopen((dir+"world"+".info").c_str(), "rb");
        if(fileWorldInfo != nullptr){

            unsigned char* data = readData(fileWorldInfo);

            int* position = new int(0);

            FileUtility::loadDouble(data, position);
            int levelCount = FileUtility::loadInt8Bit(data, position);
            FileUtility::loadString(data, position);
            FileUtility::loadInt(data, position);


            for(int i=0;i<levelCount;i++){
                std::string levelName;

                levelName = FileUtility::loadString(data, position);

                std::remove((dir+levelName+".lvl").c_str());
                std::rename((dir+levelName+".lvl.backup").c_str(), (dir+levelName+".lvl.deleted").c_str());
            }
            
            delete position;
        }
        fclose(fileWorldInfo);
        //

        std::remove((dir+"world"+".info").c_str());
        std::remove((dir+"world"+".info.backup").c_str());

        debug("Deleted");

        return true;
    }

    bool changeLevel(World* world, Point2 entrance, string newName){

        save(world);

        for(string level : world->levels){
            if(level == newName){
                load(world->name, newName);
                world->currentPlayer->pos = entrance;
                return true;
            }
        }
		/*
        world->currentLevel = new Level(world, "Floor0", Point2(300, 300));

        srand(static_cast<unsigned int>(time(NULL)));

        Point2 start = Point2(rand()%world->currentLevel->getSize().x, rand()%world->currentLevel->getSize().y);

        Point2 p = world->currentLevel->generate(rand(), start, "");
        world->levels.push_back(world->currentLevel->getName());


        world->currentPlayer = new Player(name, '@', p, Ui::C_WHITE);
        world->currentPlayer->setActiveWeapon(new Weapon(3, "Training Sword"));

        for(int i=0;i<10;i++){
            world->currentPlayer->inventory.push_back(new Item("Test Item "+to_string(i)));
        }


        world->currentLevel->newEntity(world->currentPlayer);
        */

        Level* newLevel = new Level(world, newName, world->currentLevel->getSize());

		srand(static_cast<unsigned int>(time(NULL)));
        newLevel->generate(rand(), entrance, "");
        world->levels.push_back(newLevel->getName());

        newLevel->newEntity(world->currentPlayer);

        world->currentPlayer->pos = entrance;

        world->currentLevel = newLevel;

        return true;

    }
}