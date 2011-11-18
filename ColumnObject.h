// This is a parent object for Goals and Targets

#pragma once

#include "Constants.h"
#include <SFML/Graphics.hpp>

#define vec2 sf::Vector2f


class ColumnObject {
protected:
    sf::Sprite sprite;
    int column;
    int size; // Dimensions of the image
    
public:
    ColumnObject(sf::Image* img, int col) {
        sprite.SetImage(*img);
        size = img->GetHeight();
        sprite.SetCenter(size / 2, size / 2);
        
        column = col;
        
        switch (column) {
            case 0:
                sprite.SetColor(sf::Color(255, 100, 0, 255));
                break;
            case 1:
                sprite.SetColor(sf::Color(0, 0, 255, 255));
                break;
            case 2:
                sprite.SetColor(sf::Color(0, 255, 0, 255));
                break;
            case 3:
                sprite.SetColor(sf::Color(255, 0, 0, 255));
                break;
            default:
                break;
        }
    }
    
    vec2 getPosition() {
        return sprite.GetPosition();
    }
    
    int getSize() {
        return size;
    }

	int getColumn() {
		return column;
	}
};