#include "Animation.h"

namespace sf {
    Animation::Animation() {
        clear();
    }

    Animation::~Animation() {
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
            SetSubRect(sf::Rect<int>(0,0,newFrame->GetWidth(), newFrame->GetHeight()));
            SetImage(*images.at(currentFrame));
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
            image->LoadFromFile(basePath + ss.str() + ".png");
            addFrame(image);
        }
    }

    void Animation::clear() {
        // Make sure we free up the images' memory
        for (int i = 0; i < numImgs; i++) {
            if (images[i]) { delete images[i]; }
        }
        
        images.clear();
        currentFrame = 0;
        numImgs = 0;
    }
    
    void Animation::reset() {
        currentFrame = 0;
    }
}
