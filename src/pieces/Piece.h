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
        TYPE_WHITE,
        TYPE_BLACK
    };

    Piece(Position position, TYPE type, const Texture& texture): position(position), type(type), texture(texture) {}

    virtual void Move(const Position& move) = 0;
    virtual std::vector<Position> GetPossibleMoves(const Board& board) = 0;

    Position GetPosition();

    const TYPE type;
    const Texture& texture;

protected:
    Position position; // TODO: TALVEZ TIRAR A POSIÇÃO?
    bool hasMoved = false;
};

#endif //RAY_CHESS_PIECE_H
