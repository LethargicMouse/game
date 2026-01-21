#include "game.h"

Game::Game()
    : window(sf::VideoMode({1920, 1080}), GAME_TITLE, sf::Style::None,
             sf::State::Fullscreen) {}

void Game::main_loop() {
  while (is_running()) {
    update();
    draw();
  }
}

bool Game::is_running() const { return window.isOpen(); }

void Game::update() { handle_events(); }

void Game::handle_events() {
  while (auto event = window.pollEvent())
    handle_event(event);
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
      key_pressed->scancode == sf::Keyboard::Scancode::Q)
    quit();
}

void Game::quit() { window.close(); }

void Game::draw() {
  window.clear();
  window.display();
}
