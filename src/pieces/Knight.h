#ifndef RAY_CHESS_KNIGHT_H
#define RAY_CHESS_KNIGHT_H

#include "Piece.h"
#include "../Board.h"

class Knight : public Piece {
public:
    Knight(Position position, COLOR color, const Texture& texture): Piece(position, color, Piece::TYPE::KNIGHT, texture) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_KNIGHT_H
