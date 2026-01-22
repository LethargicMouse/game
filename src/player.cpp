#include "player.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

Player::Player() : shape(PLAYER_RADIUS) { shape.setFillColor(sf::Color::Blue); }

void Player::draw(sf::RenderWindow &window) {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - PLAYER_RADIUS,
                   (float)center.y - PLAYER_RADIUS);
  shape.setPosition(pos);
  window.draw(shape);
}

void Player::update() {
  sf::Vector2f v;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
    v.y -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
    v.x -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
    v.y += 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
    v.x += 1.f;
  float dt = 0.001; // TODO get actual delta time
  if (v == sf::Vector2f())
    return;
  pos += v.normalized() * PLAYER_SPEED * dt;
}

sf::Vector2f Player::get_pos() const { return pos; }
