// Source: http://www.sfml-dev.org/wiki/en/sources/animation

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

namespace sf {
    /// A simple extension of the sf::Sprite class that allows simple use of animations.
    /// Note that all methods normally usable with a sf::Sprite are also usable here.
    class Animation : public sf::Sprite {
    public:
        Animation();
        virtual ~Animation();
        void Update();
        void Draw(RenderTarget& Target);
        bool AddFrame(sf::Image* NewFrame, unsigned int Length);
        bool InsertFrame(sf::Image* NewFrame, unsigned int Length, unsigned int Position);
        bool DeleteFrame(unsigned int Position);
        void Clear();
        sf::Image* GetFrame(unsigned int Position);
    private:
        std::vector<sf::Image*> Images;
        std::vector<unsigned int> ImageLengths;
        unsigned int CurrentImage;
        unsigned int CurrentFrame;
        unsigned int CurrentImageFrame;
    };
}