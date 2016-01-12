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



#ifdef usePortAudio
#define implementedAudio

#include <portaudio.h>

namespace Audio {

    struct Sound{
        bool playing = false;


        PaStream* stream;
        FILE* wavfile;
        int numChannels;
        int sampleRate;
        PaSampleFormat sampleFormat;
        int bytesPerSample, bitsPerSample;
    };

}

int paStreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData){

    Audio::Sound* sound = (Audio::Sound*)userData;

    size_t numRead = fread(output, sound->bytesPerSample * sound->numChannels, frameCount, sound->wavfile);
    output = (uint8_t*)output + numRead * sound->numChannels * sound->bytesPerSample;
    frameCount -= numRead;

    if(frameCount > 0) {
        memset(output, 0, frameCount * sound->numChannels * sound->bytesPerSample);
        return paComplete;
    }

    return paContinue;
}

namespace Audio {

    bool initalized = false;

    Sound* bgMusic = nullptr;


    void initAudio(){
        PaError err = Pa_Initialize();
        if(err == paNoError){
            initalized = true;
        }else{
        	debugf("PortAudio error: %s\n", Pa_GetErrorText(err));
        }

        soundMenu = loadAudioFile(AudioDir+"/menu.wav");
        soundTheme = loadAudioFile(AudioDir+"/theme.wav");
    }

    void cleanupAudio(){

        for(Sound* s : soundList){
            if(s){
                fclose(s->wavfile);
                Pa_CloseStream(s->stream);
            }
        }

        PaError err = Pa_Terminate();
        if(err == paNoError){

        }else{
            debugf("PortAudio error: %s\n", Pa_GetErrorText(err));
        }
    }

    void update(){
        
    }

    std::string freadStr(FILE* f, size_t len) {
        std::string s(len, '\0');
        fread(&s[0], 1, len, f);
        return s;
    }

    template<typename T>
    T freadNum(FILE* f) {
        T value;
        fread(&value, sizeof(value), 1, f);
        return value;
    }

    bool readFmtChunk(Sound* sound, uint32_t chunkLen) {
        if(chunkLen < 16){
            return false;
        }
        uint16_t fmttag = freadNum<uint16_t>(sound->wavfile);
        if(!(fmttag == 1 || fmttag == 3)){
            return false;
        }
        sound->numChannels = freadNum<uint16_t>(sound->wavfile);
        if(sound->numChannels <= 0){
            return false;
        }
        printf("%i channels\n", sound->numChannels);
        sound->sampleRate = freadNum<uint32_t>(sound->wavfile);
        printf("%i Hz\n", sound->sampleRate);
        uint32_t byteRate = freadNum<uint32_t>(sound->wavfile);
        uint16_t blockAlign = freadNum<uint16_t>(sound->wavfile);
        sound->bitsPerSample = freadNum<uint16_t>(sound->wavfile);
        sound->bytesPerSample = sound->bitsPerSample / 8;
        if(byteRate != sound->sampleRate * sound->numChannels * sound->bytesPerSample){
            return false;
        }
        if(blockAlign != sound->numChannels * sound->bytesPerSample){
            return false;
        }
        if(fmttag == 1 /*PCM*/) {
            switch(sound->bitsPerSample) {
                case 8: sound->sampleFormat = paInt8; break;
                case 16: sound->sampleFormat = paInt16; break;
                case 32: sound->sampleFormat = paInt32; break;
                default: return false;
            }
            printf("PCM %ibit int\n", sound->bitsPerSample);
        } else {
            if(fmttag != 3){
                return false;
            }
            if(sound->bitsPerSample != 32){
                return false;
            }
            sound->sampleFormat = paFloat32;
            printf("32bit float\n");
        }
        if(chunkLen > 16) {
            uint16_t extendedSize = freadNum<uint16_t>(sound->wavfile);
            if(chunkLen != 18 + extendedSize){
                return false;
            }
            fseek(sound->wavfile, extendedSize, SEEK_CUR);
        }
        return true;
    }



    Sound* loadAudioFile(string path){
        Sound* sound = new Sound();
        sound->wavfile = fopen(path.c_str(), "r");
        if(!sound->wavfile){
            delete sound;
            return nullptr;
        }

        if(freadStr(sound->wavfile, 4) != "RIFF"){
            delete sound;
            return nullptr;
        }
        uint32_t wavechunksize = freadNum<uint32_t>(sound->wavfile);
        if(freadStr(sound->wavfile, 4) != "WAVE"){
            delete sound;
            return nullptr;
        }
        while(true) {
            std::string chunkName = freadStr(sound->wavfile, 4);
            uint32_t chunkLen = freadNum<uint32_t>(sound->wavfile);
            if(chunkName == "fmt ")
                readFmtChunk(sound, chunkLen);
            else if(chunkName == "data") {
                if(sound->sampleRate == 0 || sound->numChannels <= 0 || sound->bytesPerSample <= 0){
                    delete sound;
                    return nullptr;
                }
                printf("len: %.0f secs\n", double(chunkLen) / sound->sampleRate / sound->numChannels / sound->bytesPerSample);
                break;
            } else {
                if(fseek(sound->wavfile, chunkLen, SEEK_CUR) != 0){
                    delete sound;
                    return nullptr;
                }
            }
        }

        soundList.push_back(sound);

        return sound;
    }

    bool playSound(Sound* sound){
        if(!sound){
            return false;
        }
        if(sound->playing){
            return false;
        }
        PaStreamParameters outputParameters;

        outputParameters.device = Pa_GetDefaultOutputDevice();
        if(outputParameters.device == paNoDevice){
            return false;
        }

        outputParameters.channelCount = sound->numChannels;
        outputParameters.sampleFormat = sound->sampleFormat;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;

        PaError ret = Pa_OpenStream(&sound->stream, NULL, &outputParameters, sound->sampleRate, paFramesPerBufferUnspecified, 0, &paStreamCallback, sound);
        
        if(ret != paNoError) {
            fprintf(stderr, "Pa_OpenStream failed: (err %i) %s\n", ret, Pa_GetErrorText(ret));
            if(sound->stream)
                Pa_CloseStream(sound->stream);
            return false;
        }
        
        if(Pa_StartStream(sound->stream) != paNoError){
            return false;
        }
        sound->playing = true;
        return true;
    }

    bool stopSound(Sound* sound){
        if(!sound->playing){
            return false;
        }
        if(Pa_StopStream(sound->stream) != paNoError){
            return false;
        }
        if(Pa_CloseStream(sound->stream) != paNoError){
            return false;
        }
        sound->playing = false;
        return true;
    }

    bool setBgMusic(Sound* sound){
        if(sound == bgMusic){
            return true;
        }else{
            if(bgMusic){
                if(bgMusic->playing){
                    stopSound(bgMusic);
                }
            }
            bgMusic = sound;
            if(!bgMusic->playing){
                playSound(bgMusic);
            }
            return true;
        }

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

