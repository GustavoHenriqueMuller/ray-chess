#ifndef RAY_CHESS_PEON_H
#define RAY_CHESS_PEON_H

#include "Piece.h"
#include "../Board.h"

class Peon : public Piece {
public:
    Peon(Position position, TYPE type, const Texture& texture): Piece(position, type, texture) {}
    std::vector<Position> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_PEON_H
