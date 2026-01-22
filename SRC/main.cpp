#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#define BALLSIZE 50.f
#define GRAVITY 980.f

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
        vertices[1].color = bottom;
        vertices[2].color = top;
        vertices[3].color = bottom;
    }

private:
    sf::VertexArray vertices;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(vertices, states);
    }

};

int main()
{
    // initialise window
    sf::RenderWindow window(sf::VideoMode({ 960, 540 }), "SFML works!");
    
    // initialise all shapes
    sf::CircleShape paperball(BALLSIZE);
    


    // set initial layout of everything
    // paper ball
    paperball.setFillColor(sf::Color::White);
    paperball.setOrigin({ BALLSIZE / 2, BALLSIZE / 2 });
    paperball.setPosition({ 750.f, 350.f });
    // power meter


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
               
            }
        }

        // first poll for spacebar input
        // while held, that starts a sweeping meter which between a MAXFORCE and a MINFORCE
        // when released, ball gets thrown with appropriate velocity
        // when OOB, gets reset
       

        window.clear();
        window.draw(paperball);
        window.display();

    }
}