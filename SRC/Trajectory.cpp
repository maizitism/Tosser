#include "Trajectory.hpp"
#include "Constants.hpp"
#include <algorithm>

void Trajectory::rebuild(sf::Vector2f p0, sf::Vector2f v0,
    float gravity, float tMax, float dt,
    sf::FloatRect bounds) {
    dots.clear();
    sf::Vector2f g{ 0.f, gravity };
    const sf::Vector2f vp = p0 + sf::Vector2f(Const::vp_x, Const::vp_y);

    // pre-reserve to avoid reallocations
    dots.reserve(static_cast<std::size_t>(tMax / dt) + 1);

    for (float t = 0.f; t <= tMax; t += dt) {
        sf::Vector2f p = p0 + v0 * t + 0.5f * g * (t * t);

        float k = 1.f / (1.f + t * Const::DepthFactor);
        k = std::max(k, Const::t_clamp);
        sf::Vector2f pp = vp + (p - vp) * k;

        if (!bounds.contains(pp)) break;

        sf::CircleShape c(dotRadius);
        c.setFillColor(dotColor);
        c.setOrigin({ dotRadius, dotRadius }); // center it
        c.setPosition(pp);
        dots.push_back(std::move(c));
    }
}

void Trajectory::clear() {
    dots.clear();
}

void Trajectory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& d : dots) target.draw(d, states);
}
