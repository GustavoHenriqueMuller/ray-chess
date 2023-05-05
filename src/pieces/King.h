#ifndef RAY_CHESS_KING_H
#define RAY_CHESS_KING_H

#include "Piece.h"
#include "../Board.h"

class King : public Piece {
public:
    King(Position position, TYPE type, const Texture& texture): Piece(position, type, texture) {}
    std::vector<Position> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_KING_H
