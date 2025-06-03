#pragma once
#include <SFML/Graphics.hpp>

class Speedometer {
public:
    Speedometer(float x, float y, const sf::Font& font);

    void setSpeed(float speed);
    void draw(sf::RenderWindow& window, const sf::Font& font);

private:
    float maxSpeed = 2000.0f;
    float currentSpeed = 0.0f;
    sf::Vector2f position;

    sf::CircleShape dial;
    sf::RectangleShape needle;
    sf::Text speedText;
};