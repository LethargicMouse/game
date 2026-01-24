#include "game.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

bool Vector2iComparator::operator()(const sf::Vector2i left,
                                    const sf::Vector2i right) const {
  return std::tie(left.x, left.y) < std::tie(right.x, right.y);
}

inline constexpr sf::Vector2u SCREEN_SIZE = {1920, 1080};
inline constexpr float PLAYER_RADIUS = TILE_SIZE * 0.3;

Game::Game()
    : window(sf::VideoMode(SCREEN_SIZE), GAME_TITLE, sf::Style::None,
             sf::State::Fullscreen),
      player_shape(PLAYER_RADIUS) {
  player_shape.setFillColor(sf::Color::Blue);
  tiles[player_grid_pos] = Tile(TileKind::Floor, player_grid_pos, 0);
  light_sources.insert(player_grid_pos);
  tile_queue.push_back(player_grid_pos);
  regenerate_tiles();
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
  sf::Time delta_time = clock.restart();
  update_player(delta_time);
  // world is better to update after player cuz it recalculates based on
  // player's new pos
  update_world();
}

void Game::handle_events() {
  while (auto event = window.pollEvent()) {
    handle_event(event);
  }
}

void Game::handle_event(std::optional<sf::Event> event) {
  if (event->is<sf::Event::Closed>())
    quit();
  if (const auto *key_pressed = event->getIf<sf::Event::KeyPressed>())
    handle_key_pressed(key_pressed);
}

void Game::handle_key_pressed(const sf::Event::KeyPressed *key_pressed) {
  assert(key_pressed != nullptr);
  if (key_pressed->scancode == sf::Keyboard::Scancode::CapsLock ||
      key_pressed->scancode == sf::Keyboard::Scancode::Escape ||
      key_pressed->scancode == sf::Keyboard::Scancode::Q)
    quit();
  if (key_pressed->scancode == sf::Keyboard::Scancode::Space)
    place_torch();
}

void Game::place_torch() { light_sources.insert(player_grid_pos); }

void Game::quit() { window.close(); }

void Game::draw() {
  window.clear();
  draw_world(player_pos);
  draw_player();
  window.display();
}

inline constexpr float PLAYER_SPEED = 200; // in pixels/sec
inline constexpr float PLAYER_WATER_SPEED = 100;

void Game::draw_player() {
  auto center = window.getSize() / 2U;
  sf::Vector2f pos((float)center.x - PLAYER_RADIUS,
                   (float)center.y - PLAYER_RADIUS);
  player_shape.setPosition(pos);
  window.draw(player_shape);
}

// helpers are conventionally defined in anonymous namespace so that they are
// accessible only in this .cpp file
namespace {
sf::Vector2i pos_on_grid(sf::Vector2f pos) {
  return {(int)round(pos.x / TILE_SIZE), (int)round(pos.y / TILE_SIZE)};
}
} // namespace

void Game::move_player(const sf::Vector2f vector) {
  auto new_pos = player_pos + vector;
  auto new_grid_pos = pos_on_grid(new_pos);
  if (new_grid_pos == player_grid_pos || !tiles[new_grid_pos].is_wall()) {
    player_pos = new_pos;
    player_grid_pos = new_grid_pos;
  }
}

void Game::update_player(sf::Time delta_time) {
  sf::Vector2f velocity;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
    velocity.y -= 1.F;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
    velocity.x -= 1.F;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
    velocity.y += 1.F;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
    velocity.x += 1.F;
  if (velocity == sf::Vector2f())
    return;

  float player_speed = PLAYER_SPEED;
  if (tiles[pos_on_grid(player_pos)].is_water())
    player_speed = PLAYER_WATER_SPEED;
  auto v_final = velocity.normalized() * player_speed * delta_time.asSeconds();
  // applying separately x & y so that the player can glide over walls
  move_player({v_final.x, 0});
  move_player({0, v_final.y});
}

void Game::draw_world(const sf::Vector2f origin) {
  for (auto &[pos, tile] : tiles)
    tile.draw(window, origin);
}

// first 4 are straight, second 4 are diagonal
const std::array<sf::Vector2i, 8> DIRS(
    {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}});

const float SQRT2 = std::sqrt(2.F);
inline constexpr float TORCH_LIGHT = 5;

void Game::update_light(sf::Vector2i pos) {
  float light = 0;
  if (light_sources.contains(pos)) {
    light = TORCH_LIGHT;
  } else {
    for (size_t i = 0; i < 4; ++i) {
      auto straight_neigh = pos + DIRS[i];
      auto diagonal_neigh = pos + DIRS[i + 4];
      if (tiles.contains(straight_neigh) && !tiles[straight_neigh].is_wall())
        light = std::max(light, tiles[straight_neigh].get_light() - 1);
      if (tiles.contains(diagonal_neigh) && !tiles[diagonal_neigh].is_wall())
        light = std::max(light, tiles[diagonal_neigh].get_light() - SQRT2);
    }
  }
  bool is_light_new = true;
  if (light <= 0) {
    auto tile = tiles.find(pos);
    if (tile == tiles.end())
      is_light_new = false;
    else
      tiles.erase(tile);
  } else if (tiles.contains(pos)) {
    if (tiles[pos].get_light() == light)
      is_light_new = false;
    else {
      tiles[pos].set_light(light);
      if (tiles[pos].is_wall()) // light not spreading through walls
        is_light_new = false;
    }
  } else
    tiles[pos] = Tile(random_kind(), pos, light);
  std::cerr << pos.x << ' ' << pos.y << ' ' << light << '\n';
  if (is_light_new)
    for (auto dir : DIRS)
      tile_queue.push_back(pos + dir);
}

inline constexpr size_t MAX_UPDATE = 150;

void Game::regenerate_tiles() {
  for (size_t _ = 0; _ < MAX_UPDATE; ++_) {
    if (tile_queue.empty())
      break;
    auto pos = tile_queue.front();
    tile_queue.pop_front();
    update_light(pos);
  }
}

void Game::update_world() {
  if (player_grid_pos != old_player_grid_pos) {
    light_sources.erase(light_sources.find(old_player_grid_pos));
    light_sources.insert(player_grid_pos);
    tile_queue.push_back(player_grid_pos);
    tile_queue.push_back(old_player_grid_pos);
    old_player_grid_pos = player_grid_pos;
  }
  regenerate_tiles();
}
