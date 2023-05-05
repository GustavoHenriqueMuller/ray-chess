#ifndef RAY_CHESS_MOVE_H
#define RAY_CHESS_MOVE_H

#include "Position.h"

struct Move {
    enum TYPE {
        WALK,
        DOUBLE_WALK,
        ATTACK,
        CASTLING,
        EN_PASSANT,
        PROMOTION
    };

    TYPE type;
    Position position;
};

#endif //RAY_CHESS_MOVE_H
