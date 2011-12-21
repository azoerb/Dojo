#pragma once

/* To use sound:
    need OpenAL, libsndfile libraries in project directory
    uncomment next line
 */

//#define USE_SOUND

#ifdef USE_SOUND

#include "Constants.h"
#include <SFML/Audio.hpp>
#include <sstream>

/*
Sources for sound files:

Music:
http://incompetech.com/m/c/royalty-free/index.html?keywords=Arcane

Sound Effects:
Created using Audacity

*/

enum soundType {
    TARGET_SOUND_0 = 0,
    TARGET_SOUND_1,
    TARGET_SOUND_2,
    TARGET_SOUND_3,
    TARGET_SOUND_4,
    TARGET_SOUND_5,
    TARGET_SOUND_6,
    TARGET_SOUND_7
};

enum musicType {
    MUSIC_0 = 0,
};


class SoundManager {
private:
    std::vector<sf::Sound*> sounds;
    std::vector<sf::Music*> music;

public:
    SoundManager();
    ~SoundManager();
    bool playSound(int type);
    bool loopSound(int type);
    void stopSound(int type);
    bool playMusic(int type);
    bool stopMusic(int type);
};

#endif