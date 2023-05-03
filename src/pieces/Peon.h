#ifndef RAY_CHESS_PEON_H
#define RAY_CHESS_PEON_H

#include "Piece.h"
#include "../Board.h"

class Peon : public Piece {
public:
    Peon(Position position, Piece::PIECE_COLOR color, const Texture& texture): Piece(position, color, texture) {}

    void Move(const Position& move) override;
    std::vector<Position> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_PEON_H
