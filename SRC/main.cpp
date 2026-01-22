#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#include "data.hpp"
#include "constants.hpp"

class GradientBar : public sf::Drawable, public sf::Transformable {
public:
    GradientBar(sf::Vector2f size, sf::Color top, sf::Color bottom)
        : vertices(sf::PrimitiveType::TriangleStrip, 4) {
            
        vertices[0].position = { 0.f, 0.f };
        vertices[1].position = { size.x, 0.f };
        vertices[2].position = { 0.f, size.y };
        vertices[3].position = { size.x, size.y };

        setColors(top, bottom);
    }
    void setColors(sf::Color top, sf::Color bottom)
    {
        vertices[0].color = top;
        vertices[2].color = bottom;
        vertices[1].color = top;
        vertices[3].color = bottom;
    }

private:
    sf::VertexArray vertices;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(vertices, states);
    }

};

void advancePowerMarker(sf::RectangleShape& marker, GradientBar& powermeter, MarkerData& md, const float dt) {
    // Sweep md.power between 0 and 1
    if (md.increasing)
        md.power += md.sweepSpeed * dt;   // "units per second"
    else
        md.power -= md.sweepSpeed * dt;

    // Bounce at the ends
    if (md.power >= 1.f) { md.power = 1.f; md.increasing = false; }
    if (md.power <= 0.f) { md.power = 0.f; md.increasing = true; }

    // Convert power -> marker position over the meter
    const sf::Vector2f meterCenter = powermeter.getPosition();
    const float meterTop = meterCenter.y - (Const::PowerMeterH * 0.5f);

    // power=1 at top, power=0 at bottom
    const float y = meterTop + (1.f - md.power) * Const::PowerMeterH;
    const float x = meterCenter.x;

    marker.setPosition({ x, y });
}


int main()
{
    // -- SETUP --
    sf::RenderWindow window(sf::VideoMode({ 960, 540 }), "SFML works!");
    window.setFramerateLimit(120);
  
    // variables
    sf::Clock clock;

    MarkerData markerdata;
    PowerMeterData meterData;

    // shapes
    sf::CircleShape paperball(Const::BallSize);
    GradientBar powermeter({Const::PowerMeterW, Const::PowerMeterH},
        meterData.c_maxforce, meterData.c_minforce);
    sf::RectangleShape powermarker({ Const::PowerMeterW + 15.f, 5.f });


    // layout
    // paper ball
    paperball.setFillColor(sf::Color::White);
    paperball.setPosition({ 750.f, 350.f });
    paperball.setOrigin({ Const::BallSize / 2, Const::BallSize / 2 });
    // power meter
    powermeter.setOrigin({ Const::PowerMeterW / 2, Const::PowerMeterH / 2 });
    powermeter.setPosition({ 900.f, 250.f });
    // power meter indicator
    powermarker.setOrigin({ (Const::PowerMeterW + 15.f) * 0.5f, 5.f * 0.5f });
    powermarker.setFillColor(sf::Color::White);

    advancePowerMarker(powermarker, powermeter, markerdata, 0.f);

    // -- MAIN LOOP --
    while (window.isOpen())
    {   
        // time step
        float dt = clock.restart().asSeconds();

        // events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();            
        }

        // inputs
        bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

        // update
        if (spaceDown) {
            advancePowerMarker(powermarker, powermeter, markerdata, dt);
        }



        // first poll for spacebar input
        // while held, that starts a sweeping meter which between a MAXFORCE and a MINFORCE
        // when released, ball gets thrown with appropriate velocity
        // compute trajectory and let ball travel along it, while slowly getting smaller, simulating a z axis
        // AABB for trashcan
        // if hit, count score or something idk
        // if not, continue its trajectory
        // when OOB, gets reset

        // happy days
       

        // render
        window.clear();
        window.draw(paperball);
        window.draw(powermeter);
        window.draw(powermarker);
        window.display();

    }
}