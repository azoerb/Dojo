// Source: http://www.sfml-dev.org/wiki/en/sources/animation

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>


namespace sf {
    class Animation : public sf::Sprite {
    private:
        std::vector<sf::Image*> images;
        int currentFrame;
        int numImgs;
        
    public:
        Animation();
        virtual ~Animation();
        void update();
        void draw(sf::RenderTarget* target);
        bool addFrame(sf::Image* newFrame);
        void clear();
        void init(std::string basePath, std::string fileType, int numImgs);
    };
}
