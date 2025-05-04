#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player(const Vector2 &pos, const float y_velocity)
        : y_velocity(y_velocity),
          pos(pos) {
    }

    [[nodiscard]] Vector2 get_pos() const {
        return pos;
    }

    void set_pos(const Vector2 &pos) {
        this->pos = pos;
    }

    [[nodiscard]] float get_y_velocity() const {
        return y_velocity;
    }

    void set_y_velocity(const float y_velocity) {
        this->y_velocity = y_velocity;;
    }

private:
    float y_velocity = 0;
    Vector2 pos = {};
};

#endif //PLAYER_H
