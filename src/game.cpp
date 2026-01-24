#include "game.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

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

void Game::place_torch() { light_sources.push_back(player_grid_pos); }

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

namespace {
// squared distance between Vector2i's
unsigned int distance2i(const sf::Vector2i pos_1, const sf::Vector2i pos_2) {
  return ((pos_1.x - pos_2.x) * (pos_1.x - pos_2.x)) +
         ((pos_1.y - pos_2.y) * (pos_1.y - pos_2.y));
}

Tile new_tile(sf::Vector2i pos, unsigned int dist) {
  // get random number in given distribution
  // C++ is fucking garbage of a language
  TileKind kind = random_kind();
  return Tile(kind, pos, dist);
}
} // namespace

const std::array<sf::Vector2i, 4> DIRS({{0, 1}, {1, 0}, {-1, 0}, {0, -1}});

void Game::erase_black_tiles() {
  std::vector<sf::Vector2i> black_poses;
  for (auto &[pos, tile] : tiles) {
    if (tile.is_black())
      black_poses.push_back(pos); // diabolically reusing vectors
  }
  for (auto pos : black_poses)
    tiles.erase(pos);
}

unsigned int Game::get_distance(const sf::Vector2i pos) {
  auto res = distance2i(player_grid_pos, pos);
  for (auto light_source : light_sources)
    res = std::min(res, distance2i(light_source, pos));
  return res;
}

// Purest, first-class cosmic horror, for the sake of your young and innocent
// brain dont even try to understand what the freaking hell is happening here,
// for God left this place long ago
void Game::regenerate_tiles() {
  for (auto &[pos, tile] : tiles)
    tile.make_black();
  std::deque<sf::Vector2i> queue;
  queue.push_back(player_grid_pos);
  tiles[player_grid_pos].set_dist(0);
  for (auto light_source : light_sources) {
    assert(tiles.contains(light_source));
    queue.push_back(light_source);
    tiles[light_source].set_dist(0);
    assert(!tiles[light_source].is_wall());
  }
  while (!queue.empty()) {
    auto pos = queue.front();
    queue.pop_front();
    assert(tiles.contains(pos)); // luckily we are free from ghosts by now
    assert(!tiles[pos].is_black());
    // not generating past walls and such cuz they're not transparent
    if (tiles[pos].is_wall())
      continue;
    for (auto dir : DIRS) {
      auto neighbour = pos + dir;
      auto dist = get_distance(neighbour);
      if (dist >= DARK_DIST)
        continue;
      if (!tiles.contains(neighbour))
        tiles[neighbour] = new_tile(neighbour, dist);
      else if (tiles[neighbour].is_black())
        tiles[neighbour].set_dist(dist);
      else
        continue;
      queue.push_back(neighbour);
    }
  }
  erase_black_tiles();
}

void Game::update_world() {
  if (player_grid_pos != old_player_grid_pos)
    regenerate_tiles();
  old_player_grid_pos = player_grid_pos;
}
