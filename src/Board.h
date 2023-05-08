#ifndef RAY_CHESS_BOARD_H
#define RAY_CHESS_BOARD_H

class Board;  // Forward declaration (circular dependency).

#include "pieces/Piece.h"
#include "raylib.h"

#include <map>
#include <string>

class Board {
public:
    Board();
    ~Board();

    void Init();
    Piece* At(const Position& position) const;
    void Set(const Position& position, Piece* piece);
    void Destroy(const Position &position);
    void Clear();
    bool IsPositionWithinBoundaries(const Position& position) const;

private:
    Piece* pieces[8][8];
};

#endif //RAY_CHESS_BOARD_H
