#include "player.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

inline constexpr float PLAYER_RADIUS = TILE_SIZE * 0.3;
inline constexpr float PLAYER_SPEED = 200; // in pixels/sec

Player::Player(World *world) : shape(PLAYER_RADIUS), world(world) { shape.setFillColor(sf::Color::Blue); }

void Player::draw(sf::RenderWindow &window) {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - PLAYER_RADIUS,
                   (float)center.y - PLAYER_RADIUS);
  shape.setPosition(pos);
  window.draw(shape);
}

sf::Vector2i pos_on_grid(sf::Vector2f pos) {
    return { (int)round(pos.x / TILE_SIZE), (int)round(pos.y / TILE_SIZE) };
}

void Player::update(sf::Time dt) {
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
  auto new_pos = pos + v.normalized() * PLAYER_SPEED * dt.asSeconds();
  auto new_grid_pos = pos_on_grid(new_pos);
  if (new_grid_pos == grid_pos || !world->get_tile(new_grid_pos).is_wall()) {
      pos = new_pos;
      grid_pos = new_grid_pos;
  }
}

sf::Vector2f const &Player::get_pos() const { return pos; }

sf::Vector2i const &Player::get_grid_pos() const { return grid_pos; }
