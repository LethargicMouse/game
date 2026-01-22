#include "world.h"
#include <SFML/System/Vector2.hpp>
#include <deque>

inline constexpr unsigned int MAX_DEPTH = 7;
inline constexpr unsigned int DIST_MAX = 100;
inline constexpr unsigned int WALL_LIMIT = 50;

bool Vector2iComparator::operator()(const sf::Vector2i a,
                                    const sf::Vector2i b) const {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

World::World(std::mt19937 *rng) : dist(1, DIST_MAX), rng(rng) {
  regenerate_tiles();
}

const std::array<sf::Vector2i, 4> DIRS({{0, 1}, {1, 0}, {-1, 0}, {0, -1}});

Tile World::new_tile(sf::Vector2i pos) {
  // get random number in given distribution
  // C++ is fucking garbage of a language
  auto number = dist(*rng);
  TileKind kind = TileKind::Floor;
  if (number >= WALL_LIMIT)
    kind = TileKind::Wall;
  return Tile(kind, pos);
}

void World::regenerate_tiles() {
  std::deque<std::pair<sf::Vector2i, unsigned int>> queue;
  queue.push_back({{0, 0}, 0});
  tiles[{0, 0}] = Tile(TileKind::Floor, {0, 0});
  while (!queue.empty()) {
    auto [pos, depth] = queue.front();
    queue.pop_front();
    // not generating past walls and such cuz they're not transparent
    if (tiles[pos].is_wall())
      continue;
    if (depth == MAX_DEPTH)
      continue;
    for (auto dir : DIRS) {
      auto neighbour = pos + dir;
      if (!tiles.contains(neighbour)) {
        tiles[neighbour] = new_tile(neighbour);
        queue.push_back({neighbour, depth + 1});
      }
    }
  }
}

void World::draw(sf::RenderWindow &window, const sf::Vector2f origin) {
  for (auto &[_, tile] : tiles)
    tile.draw(window, origin);
}
