#ifndef RAY_CHESS_PEON_H
#define RAY_CHESS_PEON_H

#include "Piece.h"
#include "../Board.h"

class Peon : public Piece {
public:
    Peon(Position position, COLOR color, const Texture& texture): Piece(position, color, Piece::TYPE::PEON, texture) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_PEON_H
