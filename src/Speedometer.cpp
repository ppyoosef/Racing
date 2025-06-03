#include <SFML/Graphics.hpp>
#include "Speedometer.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>

Speedometer::Speedometer(float x, float y, const sf::Font& font) : position(x, y) {
    dial.setRadius(120);
    dial.setPointCount(100);
    dial.setFillColor(sf::Color(255, 255, 255));
    dial.setOutlineThickness(4);
    dial.setOutlineColor(sf::Color::Red);
    dial.setOrigin(dial.getRadius(), dial.getRadius());
    dial.setPosition(position);

    needle.setSize(sf::Vector2f(70, 4));
    needle.setFillColor(sf::Color::Red);
    needle.setOrigin(10, 2);
    needle.setPosition(position);

    speedText.setFont(font);
    speedText.setCharacterSize(36);
    speedText.setFillColor(sf::Color::Red);
    speedText.setStyle(sf::Text::Bold);
    speedText.setString("0");
    speedText.setOrigin(speedText.getLocalBounds().width / 2, speedText.getLocalBounds().height / 2);
    speedText.setPosition(position.x, position.y + 40); // Below center of dial
}

void Speedometer::setSpeed(float speed) {
    currentSpeed = std::max(0.f, std::min(speed, maxSpeed));
    int displaySpeed = static_cast<int>(currentSpeed / (maxSpeed / 160)); // Map to 0-160

    speedText.setString(std::to_string(displaySpeed));
    speedText.setOrigin(speedText.getLocalBounds().width / 2, speedText.getLocalBounds().height / 2);
}

// void Speedometer::draw(sf::RenderWindow& window) {
//     window.draw(dial);

//     float angle = -120 + (currentSpeed / maxSpeed) * 240;
//     needle.setRotation(angle);
//     window.draw(needle);

//     window.draw(speedText);
// }

void Speedometer::draw(sf::RenderWindow& window, const sf::Font& font) {
    // Draw dial background
    window.draw(dial);

    // Draw tick marks and numbers
    int minSpeed = 0;
    int maxDisplaySpeed = 160;
    int majorStep = 20; // Major tick every 20
    int minorStep = 4;  // Minor tick every 4 (for 5 ticks between majors)
    float startAngle = -120.f;
    float endAngle = 120.f;
    float radius = dial.getRadius();

  

    for (int i = 0; i <= (maxDisplaySpeed / minorStep); ++i) {
        int value = i * minorStep;
        float angle = startAngle + (endAngle - startAngle) * (float)value / maxDisplaySpeed;
        float rad = angle * 3.14159265f / 180.f;
        float x1 = position.x + std::cos(rad) * (radius - 8);
        float y1 = position.y + std::sin(rad) * (radius - 8);
        float x2 = position.x + std::cos(rad) * (radius - ((value % majorStep == 0) ? 20 : 12));
        float y2 = position.y + std::sin(rad) * (radius - ((value % majorStep == 0) ? 20 : 12));

        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), sf::Color::Black),
            sf::Vertex(sf::Vector2f(x2, y2), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);

        // Draw numbers for major ticks
        if (value % majorStep == 0) {
            sf::Text number;
            number.setFont(font);
            number.setCharacterSize(18);
            number.setFillColor(sf::Color::Red);
            number.setString(std::to_string(value));
            float nx = position.x + std::cos(rad) * (radius - 36);
            float ny = position.y + std::sin(rad) * (radius - 36);
            number.setOrigin(number.getLocalBounds().width / 2, number.getLocalBounds().height / 2);
            number.setPosition(nx, ny);
            window.draw(number);
        }
    }

    // Draw needle
    float angle = startAngle + (currentSpeed / maxSpeed) * (endAngle - startAngle);
    needle.setRotation(angle);
    window.draw(needle);

    // Draw speed digits
    window.draw(speedText);
}