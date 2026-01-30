#include "PowerMeter.hpp"
#include "constants.hpp"
#include "data.hpp"

PowerMeter::PowerMeter()
    : meter({ Const::PowerMeterW, Const::PowerMeterH },
        PowerMeterData::c_maxforce, PowerMeterData::c_minforce) {

    meter.setOrigin({ Const::PowerMeterW * 0.5f, Const::PowerMeterH * 0.5f });

    marker.setSize({ Const::PowerMeterW + 15.f, 5.f });
    marker.setOrigin({ (Const::PowerMeterW + 15.f) * 0.5f, 5.f * 0.5f });
    marker.setFillColor(sf::Color::White);

    updateMarkerPosition();
}

void PowerMeter::setPosition(sf::Vector2f pos) {
    meter.setPosition(pos);
    updateMarkerPosition();
}

sf::Vector2f PowerMeter::getPosition() const {
    return meter.getPosition();
}

void PowerMeter::update(float dt) {
    // This keeps your old sweep logic but localized:
    if (increasing) power01 += MarkerData::sweepSpeed * dt;
    else            power01 -= MarkerData::sweepSpeed * dt;

    if (power01 >= 1.f) { power01 = 1.f; increasing = false; }
    if (power01 <= 0.f) { power01 = 0.f; increasing = true; }

    updateMarkerPosition();
}

float PowerMeter::power() const {
    return power01;
}

void PowerMeter::updateMarkerPosition() {
    const sf::Vector2f c = meter.getPosition();
    const float top = c.y - (Const::PowerMeterH * 0.5f);

    const float y = top + (1.f - power01) * Const::PowerMeterH; // 1 at top, 0 at bottom
    marker.setPosition({ c.x, y });
}

void PowerMeter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(meter, states);
    target.draw(marker, states);
}
