#include "Trajectory.hpp"
#include "Math.hpp"

void Trajectory::rebuild(sf::Vector2f p0, sf::Vector2f v0,
    float gravity, float tMax, float dt,
    sf::FloatRect bounds) {

    points.clear();
    sf::Vector2f g{ 0.f, gravity };

    // Tune this: bigger = more compression with distance
    const float depthFactor = 1.5f;

    // Vanishing point: pick what looks right.
    // Option A: use the start point
    // const sf::Vector2f vp = p0;

    // Option B (often nicer): a point "ahead" of p0 in screen space
    const sf::Vector2f vp = p0 + sf::Vector2f(120.f, -80.f);

    for (float t = 0.f; t <= tMax; t += dt) {
        sf::Vector2f p = p0 + v0 * t + 0.5f * g * (t * t);

        // Perspective compression: farther points get pulled toward vp
        const float k = 1.f / (1.f + t * depthFactor); // k in (0..1]
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
