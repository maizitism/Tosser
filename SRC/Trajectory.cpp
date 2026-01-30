#include "Trajectory.hpp"
#include "Math.hpp"

void Trajectory::rebuild(sf::Vector2f p0, sf::Vector2f v0,
    float gravity, float tMax, float dt,
    sf::FloatRect bounds) {

    points.clear();
    sf::Vector2f g{ 0.f, gravity };

    const float depthFactor = 1.5f;

    const sf::Vector2f vp = p0 + sf::Vector2f(120.f, -80.f);

    for (float t = 0.f; t <= tMax; t += dt) {
        sf::Vector2f p = p0 + v0 * t + 0.5f * g * (t * t);

        // Perspective compression: farther points get pulled toward vp
        float k = 1.f / (1.f + t * depthFactor); // k in (0..1]
        k = std::max(k, 0.08f);
        sf::Vector2f pp = vp + (p - vp) * k;

        if (!bounds.contains(pp)) break;
        points.append(sf::Vertex(pp));
    }
}


void Trajectory::clear() {
    points.clear();
}

void Trajectory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(points, states);
}
