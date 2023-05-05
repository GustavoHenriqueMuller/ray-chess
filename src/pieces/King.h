#ifndef RAY_CHESS_KING_H
#define RAY_CHESS_KING_H

#include "Piece.h"
#include "../Board.h"

class King : public Piece {
public:
    King(Position position, COLOR color, const Texture& texture): Piece(position, color, Piece::TYPE::KING, texture) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_KING_H
