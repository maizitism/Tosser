#pragma once
#include <SFML/Graphics.hpp>
#include <ctype.h>

class Trajectory : public sf::Drawable {
public:
    void rebuild(sf::Vector2f p0, sf::Vector2f v0,
        float gravity, float tMax, float dt,
        sf::FloatRect bounds);

    void clear();
    void setDotRadius(float r) { dotRadius = r; }
    void setDotColor(sf::Color c) { dotColor = c; }

private:
    std::vector<sf::CircleShape> dots;
    float dotRadius = 3.f;
    sf::Color dotColor = sf::Color(30, 30, 30, 220);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
