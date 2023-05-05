#ifndef RAY_CHESS_PIECE_H
#define RAY_CHESS_PIECE_H

class Piece; // Forward declaration (circular dependency).

#include "raylib.h"
#include "../Position.h"
#include "../Board.h"

#include <vector>

class Piece {
public:
    enum TYPE {
        PEON,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING
    };

    enum COLOR {
        C_WHITE,
        C_BLACK
    };

    Piece(Position position, COLOR color, TYPE type, const Texture& texture): position(position), color(color), type(type), texture(texture) {}

    virtual void Move(const Position& move);
    virtual std::vector<Position> GetPossibleMoves(const Board& board) = 0;

    Position GetPosition();

    const COLOR color;
    const TYPE type;
    const Texture& texture;

protected:
    Position position;
    bool hasMoved = false;
};

#endif //RAY_CHESS_PIECE_H
