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




#ifdef useSDLAudio
#define implementedAudio

#include <SDL2_mixer/SDL_mixer.h>




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

    bool playSound(Sound* s){
        debugf("Sound unimplemented, sound cannot be played.");
        return false;
    }

    bool stopSound(Sound* s){
        debugf("Sound unimplemented, sound cannot be stopped.");
        return false;
    }

    bool setBgMusic(Sound* s){
        debugf("Sound unimplemented, sound cannot be played.");
        return false;
    }

    void setGlobalVolume(float v){

    }
}

#endif

