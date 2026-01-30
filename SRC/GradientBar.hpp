#pragma once
#include <SFML/Graphics.hpp>

class GradientBar : public sf::Drawable, public sf::Transformable {
public:
    GradientBar(sf::Vector2f size, sf::Color top, sf::Color bottom);

    void setColors(sf::Color top, sf::Color bottom);

private:
    sf::VertexArray vertices;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
