#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode({200, 200}), "test");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);
  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }
    window.clear();
    window.draw(shape);
    window.display();
  }
}
