#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

const std::string GAME_TITLE = "the game";

class Game {
public:
  Game()
      : window(sf::VideoMode({1920, 1080}), GAME_TITLE, sf::Style::None,
               sf::State::Fullscreen) {}

  void main_loop() {
    while (is_running()) {
      update();
      draw();
    }
  }

private:
  sf::RenderWindow window;

  bool is_running() const { return window.isOpen(); }

  void update() { handle_events(); }

  void handle_events() {
    while (auto event = window.pollEvent())
      handle_event(event);
  }

  void handle_event(std::optional<sf::Event> event) {
    if (event->is<sf::Event::Closed>())
      quit();
    if (auto key_pressed = event->getIf<sf::Event::KeyPressed>())
      handle_key_pressed(key_pressed);
  }

  void handle_key_pressed(const sf::Event::KeyPressed *key_pressed) {
    assert(key_pressed != nullptr);
    if (key_pressed->scancode == sf::Keyboard::Scancode::CapsLock ||
        key_pressed->scancode == sf::Keyboard::Scancode::Q)
      quit();
  }

  void quit() { window.close(); }

  void draw() {
    window.clear();
    window.display();
  }
};

int main() {
  Game game;
  game.main_loop();
}
