#pragma once
#include <SFML/Graphics.hpp>

struct Line {
    float x, y, z; // 3d center of line
    float X, Y, W; // screen coord
    float curve, spriteX, clip, scale;
    sf::Sprite sprite;

    Line();
    void project(float camX, float camY, float camZ);
    void drawSprite(sf::RenderWindow &app);
};