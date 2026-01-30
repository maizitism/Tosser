#include "GradientBar.hpp"

GradientBar::GradientBar(sf::Vector2f size, sf::Color top, sf::Color bottom)
    : vertices(sf::PrimitiveType::TriangleStrip, 4) {

    vertices[0].position = { 0.f, 0.f };
    vertices[1].position = { size.x, 0.f };
    vertices[2].position = { 0.f, size.y };
    vertices[3].position = { size.x, size.y };

    setColors(top, bottom);
}

void GradientBar::setColors(sf::Color top, sf::Color bottom) {
    vertices[0].color = top;
    vertices[1].color = top;
    vertices[2].color = bottom;
    vertices[3].color = bottom;
}

void GradientBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(vertices, states);
}
