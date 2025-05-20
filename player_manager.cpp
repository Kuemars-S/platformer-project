#include "player_manager.h"
#include "globals.h"
#include "player.h"
#include "enemy_manager.h"
#include "level.h"
#include "level_manager.h"

void PlayerManager::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int & player_level_score : player_level_scores) {
        player_level_score = 0;
    }
}

void PlayerManager::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int PlayerManager::get_total_player_score() const {
    int sum = 0;

    for (const int player_level_score : player_level_scores) {
        sum += player_level_score;
    }

    return sum;
}

void PlayerManager::spawn_player() {
    player.set_y_velocity(0);

    for (size_t row = 0; row < LevelManager::getInstance().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < LevelManager::getInstance().get_current_level().get_columns(); ++column) {
            if (const char cell = LevelManager::getInstance().get_level_cell(row, column); cell == PLAYER) {
                player.set_pos({static_cast<float>(column),
                                   static_cast<float>(row)});
                LevelManager::getInstance().set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void PlayerManager::kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void PlayerManager::move_player_horizontally(const float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    if (const float next_x = player.get_pos().x + delta; !LevelManager::getInstance().is_colliding({next_x, player.get_pos().y}, WALL)) {
        player.set_pos({next_x, player.get_pos().y});
    }
    else {
        player.set_pos({roundf(player.get_pos().x), player.get_pos().y});
        return;
    }

    // For drawing player animations
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void PlayerManager::update_player_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (LevelManager::getInstance().is_colliding({player.get_pos().x, player.get_pos().y - 0.1f}, WALL) && player.get_y_velocity() < 0) {
        player.set_y_velocity(CEILING_BOUNCE_OFF);
    }

    // Add gravity to player's y-position
    player.set_pos({player.get_pos().x, (player.get_pos().y + player.get_y_velocity())});
    player.set_y_velocity(player.get_y_velocity() + GRAVITY_FORCE);

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_player_on_ground = LevelManager::getInstance().is_colliding({player.get_pos().x, player.get_pos().y + 0.1f}, WALL);
    if (is_player_on_ground) {
        player.set_y_velocity(0);
        player.set_pos({player.get_pos().x, roundf(player.get_pos().y)});
    }
}

void PlayerManager::update_player() {
    update_player_gravity();

    // Interacting with other level elements
    if (LevelManager::getInstance().is_colliding(player.get_pos(), COIN)) {
        LevelManager::getInstance().get_collider(player.get_pos(), COIN) = AIR; // Removes the coin
        increment_player_score();
    }

    if (LevelManager::getInstance().is_colliding(player.get_pos(), EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            LevelManager::getInstance().load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (LevelManager::getInstance().is_colliding(player.get_pos(), SPIKE) || player.get_pos().y > static_cast<float>(LevelManager::getInstance().get_current_level().get_rows())) {
        kill_player();
    }

    // Upon colliding with an enemy...
    if (EnemiesManager::getInstance().is_colliding_with_enemies(player.get_pos())) {
        // ...check if their velocity is downwards...
        if (player.get_y_velocity() > 0) {
            // ...if yes, award the player and kill the enemy
            EnemiesManager::getInstance().remove_colliding_enemy(player.get_pos());
            PlaySound(kill_enemy_sound);

            increment_player_score();
            player.set_y_velocity(-BOUNCE_OFF_ENEMY);
        }
        else {
            // ...if not, kill the player
            kill_player();
        }
    }
}

void PlayerManager::try_jump() {
    if (is_player_on_ground) {
        player.set_y_velocity(-JUMP_STRENGTH);
        is_player_on_ground = false;
    }
}