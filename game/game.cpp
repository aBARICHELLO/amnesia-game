#include "game.h"

Game::Game():
    window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "AMNESIA BRAWL"),
    game_view(sf::FloatRect(0, 0, GAME_WIDTH, GAME_HEIGHT)),
    gravity(0.f, 18.f),
    world(gravity) {

    title_screen = new class TitleScreen();
    levelselect = new class LevelSelect();

    amnesia_blue = sf::Color(14, 77, 203);
    amnesia_red = sf::Color(227, 12, 18);
    amnesia_dark_blue = sf::Color(29, 12, 137);
    amnesia_dark_red = sf::Color(158, 0, 0);

    LoadResources();
}

void Game::Start() {
    window.setFramerateLimit(80);
    // window.setVerticalSyncEnabled(true);

    float countdown = ROUND_TIME; // Round duration
    sf::Clock powerup_clock;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (game_state == GameState::STATE_TITLE) {
                
                powerup_clock.restart();
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                    game_state = GameState::STATE_LEVEL_SELECT;
                }

            } else if (game_state == GameState::STATE_LEVEL_SELECT) {
                // TODO manage deletion and creation of levelselect

                std::cout << levelselect->selection << "\n";
                std::cout << levelselect->selection % 3 + 1 << "mod" << "\n";
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    game_state = GameState::STATE_TITLE;
                }
                if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) {
                    levelselect->selection += 1;
                }
                if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up) {
                    levelselect->selection -= 1;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                    global_clock.restart(); // Restart main clock
                    game_state = GameState::STATE_PLAY;
                    CreateRound(4, levelselect->selection % 3 + 1, world); // Number of players / Map number / world
                    countdown = ROUND_TIME;
                }

            } else if (game_state == GameState::STATE_PLAY) {

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    window.close(); // DEBUG
                    // game_state == GameState::STATE_PAUSE;
                }

            } else if (game_state == GameState::STATE_PAUSE) {

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    game_state = GameState::STATE_PLAY;
                } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                    game_state = GameState::STATE_LEVEL_SELECT;
                }

            }
        }

        switch (game_state) {
            case STATE_TITLE:
                TitleScreen();
                break;
            case STATE_LEVEL_SELECT:
                LevelSelect();
                break;
            case STATE_PLAY:
                GameLoop(countdown, powerup_clock);
                break;
        }
    }
}

void Game::GameLoop(float& countdown, sf::Clock& powerup_clock) {
    // GAME LOOP
    // Delta time between frames
    sf::Time elapsed_time = global_clock.restart();
    countdown -= elapsed_time.asSeconds();

    world.Step(1.f/60.f, 10, 10);
    window.clear();

    // Draw map
    map->Draw(window);
    
    // Update players
    std::map<std::size_t, Player*>::const_iterator itr = _game_object_manager._game_objects.begin();
    while (itr != _game_object_manager._game_objects.end()) {
        itr->second->rect.setPosition(SCALE * itr->second->body->GetPosition().x, SCALE * itr->second->body->GetPosition().y);
        itr->second->rect.setRotation(itr->second->body->GetAngle() * 180 / b2_pi);
        itr->second->Update(elapsed_time, map->obstacle_array);
        itr->second->Draw(window);
        itr++;
    }
    
    // Update springs
    itr = _game_object_manager._game_objects.begin();
    while (itr != _game_object_manager._game_objects.end()) {
        for (auto spring : map->spring_array) {
            spring.Update(itr, elapsed_time);
            spring.Draw(window);
        }
        ++itr;
    }

    // Player-Player collision
    std::map<std::size_t, Player*>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end() && countdown > 0) { // Stop collisions after game ends
        std::map<std::size_t, Player*>::const_iterator iter2 = _game_object_manager._game_objects.begin();
        while (iter2 != _game_object_manager._game_objects.end()) {
            if (iter->second->rectB.getGlobalBounds().intersects(iter2->second->rectA.getGlobalBounds())) {
                iter->second->score += 1;
                iter->second->jumps_remaining = 1;
                iter2->second->Respawn();
            }
            ++iter2;
        }
        ++iter;
    }

    // Powerup generation
    if (powerup_clock.getElapsedTime().asSeconds() > 5.f && powerup_array.size() == 0 && countdown > 0) {
        powerup_clock.restart();
        PowerUp powerup;
        powerup.rect.setPosition(50 + GenerateRandom(GAME_WIDTH - 70), 50 + GenerateRandom(GAME_HEIGHT - 70));
        powerup_array.push_back(powerup);
        ResetPowerups();
    } else if (powerup_array.size() == 1 && powerup_clock.getElapsedTime().asSeconds() > 10.f) { // Clean not picked up powerups
        powerup_clock.restart();
        powerup_array.pop_back();
    }

    // Powerup drawing
    for (auto powerup : powerup_array) {
        powerup.Update(countdown);
        powerup.Draw(window);
        if (!powerup.alive) {
            powerup_array.pop_back();
        }
    }

    // Powerup collision
    iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end() && countdown > 0) {
        std::vector<PowerUp>::iterator iter2 = powerup_array.begin();
        while (iter2 != powerup_array.end()) {
            if (iter->second->rect.getGlobalBounds().intersects(iter2->rect.getGlobalBounds())) {
                auto powerup = powerup_array[0];
                switch (iter2->effect) {
                    case 1:
                        powerup.Invisibility(iter);
                        iter->second->powered_up = true;
                        break;
                    case 2:
                        powerup.Speed(iter);
                        iter->second->powered_up = true;
                        break;
                    case 3:
                        powerup.Rage(iter);
                        iter->second->powered_up = true;
                        break;
                    case 4:
                        powerup.Floaty(iter);
                        iter->second->powered_up = true;
                        break;
                    case 5:
                        powerup.Immunity(iter);
                        iter->second->powered_up = true;
                        break;
                }
                iter2->alive = false;
                powerup_clock.restart();
            }
            ++iter2;
        }
        ++iter;
    }

    // HUD Updates
    iter = _game_object_manager._game_objects.begin();
    hud->Update(iter, countdown);
    hud->Draw(window);

    // Winner check
    if (countdown < 0) {
        WinnerCheck();
        if (map->win_screen_countdown > 0) {
            map->win_screen_countdown -= elapsed_time.asSeconds();
            if (map->win_screen_countdown <= 0) {
                window.setView(game_view);
                Cleanup();
                game_state = GameState::STATE_TITLE;
            }
        }
    }

    window.display();
}

void Game::TitleScreen() {
    title_screen->Draw(window);
}

void Game::LevelSelect() {
    levelselect->Draw(window);
}

void Game::CreateRound(std::size_t players_num, std::size_t level_number, b2World& world) {
    LoadPlayers(players_num);
    hud = new HUD(players_num);
    map = new Map(level_number, world);
}

void Game::LoadPlayers(std::size_t number_of_players) {
    player1 = new Player();
    player2 = new Player();
    SpawnPlayer(1, player1, amnesia_blue, sf::Keyboard::Key::W, sf::Keyboard::Key::A, sf::Keyboard::Key::D);
    SpawnPlayer(2, player2, amnesia_red, sf::Keyboard::Key::Up, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right);
    if (number_of_players > 2 && number_of_players < 5) {
        player3 = new Player();
        SpawnPlayer(3, player3, amnesia_dark_blue, sf::Keyboard::Key::T, sf::Keyboard::Key::F, sf::Keyboard::Key::H);
        if (number_of_players == 4) {
            player4 = new Player();
            SpawnPlayer(4, player4, amnesia_dark_red, sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::L);
        }
    }
}

void Game::SpawnPlayer(std::size_t number, Player* player, sf::Color color, sf::Keyboard::Key jump, sf::Keyboard::Key left, sf::Keyboard::Key right) {
    player->number = number;
    player->jump = jump;
    player->left = left;
    player->right = right;
    player->sprite.setColor(color);
    CreatePlayer(world, player, spawn_locations[number-1].x, spawn_locations[number-1].y);
}

// Load game sprites
void Game::LoadResources() {
    // Default spawn locations
    spawn_locations.push_back(b2Vec2(GAME_WIDTH/3, GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH - GAME_WIDTH/3, GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH/6, GAME_HEIGHT - GAME_HEIGHT/3));
    spawn_locations.push_back(b2Vec2(GAME_WIDTH - GAME_WIDTH/6, GAME_HEIGHT - GAME_HEIGHT/3));

    background_music.openFromFile(BACKGROUND_SONG);
    background_music.play();
}

void Game::CreatePlayer(b2World& world, Player* player, int x, int y) {
    const float density = 1;
    player->bodydef.type = b2_dynamicBody;
    player->bodydef.position.Set(x / SCALE, y / SCALE);
    player->body = world.CreateBody(&player->bodydef);

    // Width and height subtracted by one, so the rect can intersect with the ground
    player->shape.SetAsBox(23 / SCALE, 23 / SCALE); // Main player body
    player->body->CreateFixture(&player->shape, density);

    _game_object_manager.Add(player->number, player);
}

void Game::WinnerCheck() {
    // Iterate to get the highest score
    auto iter = _game_object_manager._game_objects.begin();
    auto winner = iter->second;
    auto winner_score = iter->second->score;
    while (iter != _game_object_manager._game_objects.end()) {
        if (iter->second->score > winner_score) {
            winner = iter->second;
            winner_score = iter->second->score;
        }
        ++iter;
    }

    // Update winner text
    hud->winner_text.setOrigin(hud->winner_text.getLocalBounds().width / 2, hud->winner_text.getLocalBounds().height / 2);
    hud->winner_text.setPosition(winner->rect.getPosition().x, winner->rect.getPosition().y - 100);
    hud->winner_text.setString("Player  " + std::to_string(winner->number) + "  wins!");
    // TODO add amnesia logo
    window.draw(hud->winner_text);

    // Center view to winner
    sf::View winner_view;
    winner_view.setSize(GAME_WIDTH / 2, GAME_HEIGHT / 2);

    float halfviewX = GAME_WIDTH / 2 * 0.5f;
    float halfviewY = GAME_HEIGHT / 2 * 0.5f;

    // Keep the camera inbounds
    // X Axis
    if (winner->rect.getPosition().x < halfviewX) {
        winner->rect.setPosition(halfviewX, winner->rect.getPosition().y);
    } else if (winner->rect.getPosition().x > GAME_WIDTH - halfviewX) {
        winner->rect.setPosition(GAME_WIDTH - halfviewX, winner->rect.getPosition().y);
    }
    
    // Y Axis
    if (winner->rect.getPosition().y < halfviewY) {
        winner->rect.setPosition(winner->rect.getPosition().x, halfviewY);
    } else if (winner->rect.getPosition().y > GAME_HEIGHT - halfviewY) {
        winner->rect.setPosition(winner->rect.getPosition().x, GAME_HEIGHT - halfviewY);
    }

    winner_view.setCenter(winner->rect.getPosition());
    window.setView(winner_view);
}

void Game::ResetPowerups() {
    std::map<std::size_t, Player*>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end()) {
        if (iter->second->powered_up) {
            PowerUp::ResetPowerupEffects(iter);
            iter->second->powered_up = false;
        }
        ++iter;
    }
}

void Game::Cleanup() {
    delete map;
    auto i = 1;
    std::map<std::size_t, Player*>::const_iterator iter = _game_object_manager._game_objects.begin();
    while (iter != _game_object_manager._game_objects.end()) {
        _game_object_manager.Remove(i);
        ++i;
        ++iter;
    }
}

GameObjectManager Game::_game_object_manager;
