#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "SFML works!");
    const sf::Texture texture("ASSETS/snow.png");
    sf::Sprite sprite(texture);


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                // left key is pressed: move our character
                sprite.move({ -1.f, 0.f });
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                // left key is pressed: move our character
                sprite.move({ 1.f, 0.f });
            }

        }

        window.clear();
        window.draw(sprite);
        window.display();


    }
}