#include "Trajectory.hpp"

void Trajectory::rebuild(sf::Vector2f p0, sf::Vector2f v0,
    float gravity, float tMax, float dt,
    sf::FloatRect bounds) {
    points.clear();
    sf::Vector2f g{ 0.f, gravity };

    for (float t = 0.f; t <= tMax; t += dt) {
        sf::Vector2f p = p0 + v0 * t + 0.5f * g * (t * t);
        if (!bounds.contains(p)) break;
        points.append(sf::Vertex(p));
    }
}

void Trajectory::clear() {
    points.clear();
}

void Trajectory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(points, states);
}
