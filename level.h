#ifndef LEVEL_H
#define LEVEL_H

class Level {
public:
    Level(const int row, const int col, char data) : rows(row), columns(col), data(&data) {}
    Level() = default;
    ~Level() = default;

    [[nodiscard]] size_t get_rows() const {
        return rows;
    }

    void set_rows(const size_t rows) {
        this->rows = rows;
    }

    [[nodiscard]] size_t get_columns() const {
        return columns;
    }

    void set_columns(const size_t columns) {
        this->columns = columns;
    }

    [[nodiscard]] char * get_data() const {
        return data;
    }

    void set_data(char * const data) {
        this->data = data;
    }

private:
    size_t rows = 0, columns = 0;
    char *data = nullptr;
};

#endif //LEVEL_H