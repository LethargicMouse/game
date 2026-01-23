#include "game.h"
#include <SFML/Window/Keyboard.hpp>

inline constexpr sf::Vector2u SCREEN_SIZE = {1920, 1080};
inline constexpr float PLAYER_RADIUS = TILE_SIZE * 0.3;

Game::Game()
    : window(sf::VideoMode(SCREEN_SIZE), GAME_TITLE, sf::Style::None,
             sf::State::Fullscreen),
      player_shape(PLAYER_RADIUS), world(&player_grid_pos) {
  player_shape.setFillColor(sf::Color::Blue);
}

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
  update_player(dt);
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
  }
}

void Game::quit() { window.close(); }

void Game::draw() {
  window.clear();
  world.draw(window, player_pos);
  draw_player();
  window.display();
}

inline constexpr float PLAYER_SPEED = 200;
inline constexpr float PLAYER_WATER_SPEED = 100;

void Game::draw_player() {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - PLAYER_RADIUS,
                   (float)center.y - PLAYER_RADIUS);
  player_shape.setPosition(pos);
  window.draw(player_shape);
}

sf::Vector2i pos_on_grid(sf::Vector2f pos) {
  return {(int)round(pos.x / TILE_SIZE), (int)round(pos.y / TILE_SIZE)};
}

void Game::update_player(sf::Time dt) {
  sf::Vector2f v;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
    v.y -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
    v.x -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
    v.y += 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
    v.x += 1.f;
  if (v == sf::Vector2f())
    return;

  float player_speed = PLAYER_SPEED; // in pixels/sec
  if (world.get_tile(pos_on_grid(player_pos)).is_water()) {
    player_speed = PLAYER_WATER_SPEED;
  }
  auto new_pos = player_pos + v.normalized() * player_speed * dt.asSeconds();
  auto new_grid_pos = pos_on_grid(new_pos);
  if (new_grid_pos == player_grid_pos ||
      !world.get_tile(new_grid_pos).is_wall()) {
    player_pos = new_pos;
    player_grid_pos = new_grid_pos;
  }
}
