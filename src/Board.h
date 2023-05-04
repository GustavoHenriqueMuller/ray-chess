#ifndef RAY_CHESS_BOARD_H
#define RAY_CHESS_BOARD_H

class Board;  // Forward declaration (circular dependency).

#include "pieces/Piece.h"

class Board {
public:
    Board();
    ~Board();

    Piece* At(const Position& position) const;
    void Set(const Position& position, Piece* piece);
    void Destroy(const Position &position);
    bool IsPositionWithinBoundaries(const Position& position) const;

private:
    Piece* pieces[8][8];
};

#endif //RAY_CHESS_BOARD_H
