#include "Line.hpp"
#include <SFML/Graphics.hpp>

extern int width, height, roadW;
extern float camD;

Line::Line() : x(0), y(0), z(0), X(0), Y(0), W(0), curve(0), spriteX(0), clip(0), scale(0) {}

void Line::project(float camX, float camY, float camZ) {
    scale = camD / (z - camZ);
    X = (1 + scale * (x - camX)) * width / 2;
    Y = (1 - scale * (y - camY)) * height / 2;
    W = scale * roadW * width / 2;
}

void Line::drawSprite(sf::RenderWindow &app) {
    sf::Sprite s = sprite;
    int w = s.getTextureRect().width;
    int h = s.getTextureRect().height;

    float destX = X + scale * spriteX * width / 2;
    float destY = Y + 4;
    float destW = w * W / 266;
    float destH = h * W / 266;

    destX += destW * spriteX; // offsetX
    destY += destH * (-1);    // offsetY

    float clipH = destY + destH - clip;
    if (clipH < 0)
        clipH = 0;

    if (clipH >= destH)
        return;
    s.setTextureRect(sf::IntRect(0, 0, w, h - h * clipH / destH));
    s.setScale(destW / w, destH / h);
    s.setPosition(destX, destY);
    app.draw(s);
}