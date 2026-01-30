#pragma once
#include <SFML/Graphics.hpp>
#include <ctype.h>

class Trajectory : public sf::Drawable {
public:
    void rebuild(sf::Vector2f p0, sf::Vector2f v0,
        float gravity, float tMax, float dt,
        sf::FloatRect bounds);

    void rebuild3D(sf::Vector2f p0, sf::Vector2f v0,
        float gravity, float tMax, float dt,
        sf::FloatRect bounds,
        float sizeNear = 6.f,
        float sizeFar = 1.f,
        uint8_t alphaNear = 255,
        uint8_t alphaFar = 50);
    void clear();

private:
    sf::VertexArray points{ sf::PrimitiveType::Points };
    sf::VertexArray tris{ sf::PrimitiveType::Triangles };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
