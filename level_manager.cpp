#include "level_manager.h"
#include <fstream>
#include <sstream>
#include <cctype>

bool LevelManager::is_inside_level(int row, int column) const {
    if (row < 0 || row >= current_level.get_rows()) return false;
    if (column < 0 || column >= current_level.get_columns()) return false;
    return true;
}

bool LevelManager::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& LevelManager::get_collider(Vector2 pos, char look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }

    return get_level_cell(pos.x, pos.y);
}

void LevelManager::reset_level_index() {
    level_index = 0;
}

void LevelManager::load_level(int offset) {
    level_index += offset;

    std::ifstream file("data/levels.rll");
    if (!file.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open levels.rll file");
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Find the requested level in the file
    int current_level_in_file = 0;
    std::string line;
    std::vector<std::vector<char>> level_data;
    size_t max_columns = 0;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;

        if (current_level_in_file == level_index) {
            // Process this level
            std::istringstream iss(line);
            std::string row;

            while (std::getline(iss, row, '|')) {
                level_data.emplace_back();
                size_t i = 0;

                while (i < row.size()) {
                    if (std::isdigit(row[i])) {
                        int count = 0;
                        while (i < row.size() && std::isdigit(row[i])) {
                            count = count * 10 + (row[i++] - '0');
                        }
                        if (i >= row.size()) break;

                        char c = row[i++];
                        for (int j = 0; j < count; j++) {
                            level_data.back().push_back(c);
                        }
                    } else {
                        level_data.back().push_back(row[i++]);
                    }
                }

                if (level_data.back().size() > max_columns) {
                    max_columns = level_data.back().size();
                }
            }
            break;
        }
        current_level_in_file++;
    }
    file.close();

    if (level_data.empty()) {
        // No more levels - game completed
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Load the level data
    size_t rows = level_data.size();
    if (current_level_data) {
        delete[] current_level_data;
    }
    current_level_data = new char[rows * max_columns];

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < max_columns; j++) {
            current_level_data[i * max_columns + j] =
                (j < level_data[i].size()) ? level_data[i][j] : '-';
        }
    }

    current_level.set_rows(rows);
    current_level.set_columns(max_columns);
    current_level.set_data(current_level_data);

    PlayerManager::getInstance().spawn_player();
    EnemiesManager::getInstance().spawn_enemies();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void LevelManager::unload_level() const {
    delete[] current_level_data;
}

char& LevelManager::get_level_cell(size_t row, size_t column) const {
    return current_level.get_data()[row * current_level.get_columns() + column];
}

void LevelManager::set_level_cell(size_t row, size_t column, char chr) const {
    get_level_cell(row, column) = chr;
}