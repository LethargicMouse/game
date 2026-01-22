#include "game.h"
#include <SFML/Window/Keyboard.hpp>

const sf::Vector2u SCREEN_SIZE = {1920, 1080};

Game::Game()
    : window(sf::VideoMode(SCREEN_SIZE), GAME_TITLE, sf::Style::None,
             sf::State::Fullscreen),
      player(), world(&player.get_pos()) {}

void Game::main_loop() {
  while (is_running()) {
    update();
    draw();
  }
}

bool Game::is_running() const { return window.isOpen(); }

void Game::update() {
  handle_events();
  // get the duration of last game tick
  sf::Time dt = clock.restart();
  player.update(dt);
  // world is better to update after player cuz it recalculates based on
  // player's new pos
  world.update();
}

void Game::handle_events() {
  while (auto event = window.pollEvent()) {
    handle_event(event);
  }
}

void Game::handle_event(std::optional<sf::Event> event) {
  if (event->is<sf::Event::Closed>())
    quit();
  if (auto key_pressed = event->getIf<sf::Event::KeyPressed>())
    handle_key_pressed(key_pressed);
}

void Game::handle_key_pressed(const sf::Event::KeyPressed *key_pressed) {
  assert(key_pressed != nullptr);
  if (key_pressed->scancode == sf::Keyboard::Scancode::CapsLock ||
      key_pressed->scancode == sf::Keyboard::Scancode::Escape ||
      key_pressed->scancode == sf::Keyboard::Scancode::Q) {
    quit();
    return;
  }
}

void Game::quit() { window.close(); }

void Game::draw() {
  window.clear();
  world.draw(window, player.get_pos());
  player.draw(window);
  window.display();
}
