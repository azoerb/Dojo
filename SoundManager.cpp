#include "SoundManager.h"

#ifdef USE_SOUND

SoundManager::SoundManager() {
	
    std::string soundBase = "Sounds/sound2-short";
    std::string musicBase = "Music/music";

    // load target sounds
    for (int i = 0; i <= TARGET_SOUND_7; i++) {
        sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
        std::stringstream s;
        s << i;

        if (!soundBuffer->LoadFromFile(soundBase + s.str() + ".wav")) {
            printf("Error loading audio files\n");
        }

        sf::Sound* sound = new sf::Sound();

        sound->SetBuffer(*soundBuffer);

        sounds.push_back(sound);
    }

    // load music
    for (int i = MUSIC_0; i <= MUSIC_0; i++) {
        sf::Music* m = new sf::Music();
        std::stringstream s;
        s << i;

        if (!m->OpenFromFile(musicBase + s.str() + ".wav")) {
            printf("Error loading audio files\n");
        }

        m->SetVolume(15);
        m->SetLoop(true);
        music.push_back(m);
    }

}

SoundManager::~SoundManager() {
    for (int i = 0; i < sounds.size(); i++) {
        if (sounds.at(i)) { delete sounds[i]; }
    }

    for (int i = 0; i < music.size(); i++) {
        if (music.at(i)) { delete music[i]; }
    }
}


bool SoundManager::playSound(int type) {

    if (type < sounds.size()) {
        if (sounds[type]->GetStatus() == sf::Sound::Playing) {
            sounds[type]->Stop();
        }
        sounds[type]->SetLoop(false);
        sounds[type]->Play();
    } else {
        return false;
    }

    return true;
}

bool SoundManager::loopSound(int type) {

    if (type < sounds.size()) {
        if (sounds[type]->GetStatus() == sf::Sound::Playing) {
            return false;
        }
        sounds[type]->SetLoop(true);
        sounds[type]->Play();
    } else {
        return false;
    }

    return true;
}

void SoundManager::stopSound(int type) {

    if (type <= MUSIC_2) {
        sounds[type]->Stop();
    }
}

bool SoundManager::playMusic(int type) {

    if (type < music.size()) {
        if (music[type]->GetStatus() == sf::Music::Playing) {
            return false;
        }
        music[type]->Play();
    } else {
        return false;
    }

    return true;
}

bool SoundManager::stopMusic(int type) {

    if (type < music.size()) {
        music[type]->Stop();
    } else {
        return false;
    }

    return true;
}

#endif