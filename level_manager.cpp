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

    // First try loading from RLL file
    if (level_index == 0) {
        std::ifstream file("data/levels.rll");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == ';') continue;

                std::vector<std::string> row_strings;
                std::istringstream iss(line);
                std::string row;

                while (std::getline(iss, row, '|')) {
                    row_strings.push_back(row);
                }

                if (row_strings.empty()) continue;

                size_t max_columns = 0;
                std::vector<std::vector<char>> level_data;

                for (const auto& r : row_strings) {
                    level_data.emplace_back();
                    size_t i = 0;

                    while (i < r.size()) {
                        if (std::isdigit(r[i])) {
                            int count = 0;
                            while (i < r.size() && std::isdigit(r[i])) {
                                count = count * 10 + (r[i++] - '0');
                            }
                            if (i >= r.size()) break;

                            char c = r[i++];
                            for (int j = 0; j < count; j++) {
                                level_data.back().push_back(c);
                            }
                        } else {
                            level_data.back().push_back(r[i++]);
                        }
                    }

                    if (level_data.back().size() > max_columns) {
                        max_columns = level_data.back().size();
                    }
                }

                size_t rows = level_data.size();
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
                break;
            }
            file.close();

            if (current_level.get_data()) {
                PlayerManager::getInstance().spawn_player();
                EnemiesManager::getInstance().spawn_enemies();
                derive_graphics_metrics_from_loaded_level();
                timer = MAX_LEVEL_TIME;
                return;
            }
        }
    }

    // Fall back to built-in levels if RLL loading fails
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    size_t rows = LEVELS[level_index].get_rows();
    size_t columns = LEVELS[level_index].get_columns();
    current_level_data = new char[rows*columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = LEVELS[level_index].get_data()[row * columns + column];
        }
    }

    current_level.set_rows(rows);
    current_level.set_columns(columns);
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