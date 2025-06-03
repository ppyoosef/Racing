#pragma once
#include <SFML/Graphics.hpp>

class Car {
public:
    Car(const std::string& texturePath, float startX, float startY);
    void setAngle(float angle);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
};