#include "Car.hpp"

Car::Car(const std::string& texturePath, float startX, float startY) {
    texture.loadFromFile(texturePath);
    texture.setSmooth(true); // Enable smooth scaling
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(startX, startY);
}

void Car::setAngle(float angle) {
    sprite.setRotation(angle);
}

void Car::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Car::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}