#include "player.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

inline constexpr float PLAYER_RADIUS = TILE_SIZE * 0.5;
inline constexpr float PLAYER_SPEED = 100; // in pixels/sec

Player::Player() : shape(PLAYER_RADIUS) { shape.setFillColor(sf::Color::Blue); }

void Player::draw(sf::RenderWindow &window) {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - PLAYER_RADIUS,
                   (float)center.y - PLAYER_RADIUS);
  shape.setPosition(pos);
  window.draw(shape);
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
  pos += v.normalized() * PLAYER_SPEED * dt.asSeconds();
}

sf::Vector2f Player::get_pos() const { return pos; }
