#include "player.h"

Player::Player() : shape(PLAYER_RADIUS) { shape.setFillColor(sf::Color::Blue); }

void Player::draw(sf::RenderWindow &window) {
  auto center = window.getSize() / 2u;
  sf::Vector2f pos((float)center.x - PLAYER_RADIUS,
                   (float)center.y - PLAYER_RADIUS);
  shape.setPosition(pos);
  window.draw(shape);
}
