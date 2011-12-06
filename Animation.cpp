#include "Animation.h"

namespace sf {
    Animation::Animation() {
        numImgs = 0;
        currentFrame = 0;
        images.clear();
    }

    Animation::~Animation() {
        printf("~Animation()\n");
        clear();
    }

    void Animation::update() {
        currentFrame++;

        if (currentFrame >= images.size() * NUM_PASSES) {
            currentFrame = 0;
        }
                
        SetImage(*images.at(currentFrame / NUM_PASSES));
    }

    void Animation::draw(sf::RenderTarget* target) {
        target->Draw(*this);
        update();
    }

    bool Animation::addFrame(sf::Image* newFrame) {
        images.push_back(newFrame);
        
        if (images.size() == 1) {
            SetSubRect(sf::Rect<int>(0, 0, newFrame->GetWidth(), newFrame->GetHeight()));
            SetImage(*images.at(0));
        }
        numImgs++;
        return true;
    }
    
    void Animation::init(std::string basePath, int numImgs) {
        // So we don't have to hard code in all of the filenames
        for (int i = 1; i <= numImgs; i++) {
            std::stringstream ss;
            ss << i;
            
            sf::Image* image = new sf::Image();
            if (!image->LoadFromFile(basePath + ss.str() + ".png")) {
                printf("Error loading resource - init\n");
            }
            if (!addFrame(image)) {
                printf("Error adding frame to animation - init\n");
            }
        }
    }

    void Animation::clear() {
        printf("Animation::Clear()\n");
        // Make sure we free up the images' memory
        for (int i = 0; i < numImgs; i++) {
            if (images.at(i)) { delete images.at(i); }
        }
    }
    
    void Animation::reset() {
        currentFrame = 0;
    }
}
