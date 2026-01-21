#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

const std::string GAME_TITLE = "the game";

class Game {
public:
  Game();

  void main_loop();

private:
  sf::RenderWindow window;

  bool is_running() const;

  void update();

  void handle_events();

  void handle_event(std::optional<sf::Event>);

  void handle_key_pressed(const sf::Event::KeyPressed *key_pressed);

  void quit();

  void draw();
};
