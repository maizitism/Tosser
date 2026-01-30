#pragma once
#include <SFML/Graphics.hpp>
#include <ctype.h>

class Trajectory : public sf::Drawable {
public:
    void rebuild(sf::Vector2f p0, sf::Vector2f v0,
        float gravity, float tMax, float dt,
        sf::FloatRect bounds);

    void clear();

private:
    sf::VertexArray points{ sf::PrimitiveType::Points };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
