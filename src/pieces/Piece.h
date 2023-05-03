#ifndef RAY_CHESS_PIECE_H
#define RAY_CHESS_PIECE_H

#include "raylib.h"

class Piece {
public:
    enum Color {
        PIECE_WHITE,
        PIECE_BLACK
    };

    enum Type {
        PEON,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING
    };

    int i, j;

    Color color;
    Type type;
    Texture texture;
};


#endif //RAY_CHESS_PIECE_H
