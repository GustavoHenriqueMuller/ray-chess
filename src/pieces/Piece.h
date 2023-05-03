#ifndef RAY_CHESS_PIECE_H
#define RAY_CHESS_PIECE_H

class Piece; // Forward declaration (circular dependency).

#include "raylib.h"
#include "../Position.h"
#include "../Board.h"

#include <vector>

class Piece {
public:
    enum PIECE_COLOR {
        PIECE_WHITE,
        PIECE_BLACK
    };

    Piece(Position position, PIECE_COLOR color, const Texture& texture): position(position), color(color), texture(texture) {}
    virtual std::vector<Position> GetPossibleMoves(const Board& board) = 0;

    Position position;
    PIECE_COLOR color;
    const Texture& texture;
};

#endif //RAY_CHESS_PIECE_H
