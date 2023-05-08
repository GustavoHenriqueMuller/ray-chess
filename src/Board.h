#ifndef RAY_CHESS_BOARD_H
#define RAY_CHESS_BOARD_H

class Board;  // Forward declaration (circular dependency).

#include "pieces/Piece.h"
#include "pieces/PieceEnums.h"
#include "raylib.h"

#include <map>
#include <string>
#include <vector>

class Board {
public:
    void Init();
    Piece* At(const Position& position) const;
    void Add(Piece* piece);
    void Destroy(const Position &position);
    void Clear();
    bool IsPositionWithinBoundaries(const Position& position) const;

    std::vector<Piece*> GetPiecesByColor(PIECE_COLOR color);

private:
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;
};

#endif //RAY_CHESS_BOARD_H
