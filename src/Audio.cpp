//
//  Audio.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/11/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//


#include "Global.hpp"
#include "Ui.hpp"
#include "Audio.hpp"
#include "Settings.hpp"


namespace Audio{

    vector<Sound*> soundList;

    Sound* soundMenu;
    Sound* soundTheme;

}

/*
#ifdef useYSEAudio
#define implementedAudio

#include <yse.hpp>

namespace Audio{


    struct Sound{
        YSE::sound yseSound;
    };

    Sound* bgMusic = nullptr;


    void initAudio(){
        YSE::System().init();

        soundMenu = loadAudioFile(AudioDir+"/menu.ogg");
        soundTheme = loadAudioFile(AudioDir+"/theme.ogg");
    }

    void cleanupAudio(){
        YSE::System().close();
        for(Sound* sound : soundList){
            delete sound;
        }
    }

    void update(){
        YSE::System().update();
    }

    Sound* loadAudioFile(string path){
        Sound* s = new Sound();
        s->yseSound.create(path.c_str(), &YSE::ChannelMusic(), true);
        if (!s->yseSound.isValid()) {
            debugf("Failed to load sound file: %s", path.c_str());
            delete s;
            return nullptr;
        }
        s->yseSound.setLooping(true);
        soundList.push_back(s);
        return s;
    }

    bool setBgMusic(Sound* s){
        if(s != bgMusic){
            if(bgMusic && bgMusic->yseSound.isPlaying()){
                bgMusic->yseSound.fadeAndStop(2000);
            }
            bgMusic = s;
            if(bgMusic){
                //bgMusic->yseSound.setTime(0);
                YSE::ChannelMusic().setVolume(Settings::musicVolume/100.0f);
                bgMusic->yseSound.play();
                bgMusic->yseSound.setVolume(0, 0);
                bgMusic->yseSound.setVolume(1, 2000);
            }
            return true;
        }
        return false;
    }
}


#endif
*/



#ifdef useSDLAudio
#define implementedAudio

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace Audio{
    
    bool success = false;
    
    struct Sound{
        
        Mix_Music* mixMusic;
        
    };

    Sound* bgMusic = nullptr;
    
}

void musicFinished(){
    debugf("Music fade complete");
    if(Audio::bgMusic){
        debugf("Playing next music");
        Mix_FadeInMusic(Audio::bgMusic->mixMusic, -1, 2000);
        debugf("Fading in music");
    }
}

namespace Audio{


    void initAudio(){
        if(SDL_Init(SDL_INIT_AUDIO) == -1){
            debugf("Failed to initalize SDL Audio. Error: %s\n", SDL_GetError());
            return;
        }
        if(Mix_Init(MIX_INIT_OGG) == -1){
            debugf("Failed to initalize Mix OGG. Error: %s\n", Mix_GetError());
            return;
        }
        if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1){
            debugf("Mix_OpenAudio() Error:: %s\n", Mix_GetError());
            return;
        }
        
        debugf("Initalized Audio");
        success = true;

        soundMenu = loadAudioFile(AudioDir+"/menu.ogg");
        soundTheme = loadAudioFile(AudioDir+"/theme.ogg");
    }

    void cleanupAudio(){
        for(Sound* sound : soundList){
            Mix_FreeMusic(sound->mixMusic);
            delete sound;
        }
    }

    void update(){
        
        
    }

    Sound* loadAudioFile(string path){
        if(!success){
            return nullptr;
        }
        Sound* s = new Sound();
        s->mixMusic = Mix_LoadMUS(path.c_str());
        if(!s->mixMusic){
            debugf("Failed to load file: \"%s\". Error: %s\n", path.c_str(), Mix_GetError());
            delete s;
            return nullptr;
        }
        debugf("Loaded file: \"%s\"", path.c_str());
        
        soundList.push_back(s);
        return s;
    }

    bool setBgMusic(Sound* s){
        if(!success){
            return false;
        }
        if(s != bgMusic){
            Mix_VolumeMusic((int)((Settings::musicVolume/100.0)*MIX_MAX_VOLUME));
            if(bgMusic && Mix_PlayingMusic()){
                Mix_FadeOutMusic(2000);
                Mix_HookMusicFinished(musicFinished);
                debugf("Fading out music...");
            }
            bgMusic = s;
            if(bgMusic && !Mix_PlayingMusic()){
                Mix_FadeInMusic(bgMusic->mixMusic, -1, 2000);
                debugf("Fading in music");
            }
            return true;
        }
        return false;
    }
}



#endif




#ifndef implementedAudio

namespace Audio{
    
    struct Sound{};


    void initAudio(){
        debugf("Sound unimplemented, sound cannot be initalized.");
    }

    void cleanupAudio(){}

    void update(){}

    Sound* loadAudioFile(string path){
        debugf("Sound unimplemented, sound cannot be loaded.");
        return nullptr;
    }

    bool setBgMusic(Sound* s){
        debugf("Sound unimplemented, sound cannot be played.");
        return false;
    }

    void setGlobalVolume(float v){

    }
}

#endif

