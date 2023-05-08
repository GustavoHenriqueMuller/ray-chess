#ifndef RAY_CHESS_MOVE_H
#define RAY_CHESS_MOVE_H

#include "Position.h"

enum MOVE_TYPE {
    WALK,
    DOUBLE_WALK,
    ATTACK,
    SHORT_CASTLING,
    LONG_CASTLING,
    EN_PASSANT,
    PROMOTION,
    ATTACK_AND_PROMOTION,
};

struct Move {
    MOVE_TYPE type;
    Position position;
};

#endif //RAY_CHESS_MOVE_H
