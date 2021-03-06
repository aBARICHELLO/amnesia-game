#pragma once

#include "stdafx.h"
#include "entity.h"
#include "obstacle.h"

class Player : public Entity {
public:
    Player();
    void Draw(sf::RenderWindow& window);
    void Update(sf::Time elapsed_time, std::vector<Obstacle> &obstacle_array);
    void Respawn();

    std::size_t number;
    int score = 0;
    float movement_speed = 1000.f;
    float max_speed = 9.f;
    float jump_impulse = 15.f;
    std::size_t jumps_remaining = 1;

    bool powered_up = false;
    bool alive = true;

    sf::Clock jump_clock;

    sf::Texture default_texture;
    sf::Texture immune_texture;
    sf::Texture inverted_texture;
    sf::RectangleShape rectA;
    sf::RectangleShape rectB;
    sf::Keyboard::Key jump;
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
};
