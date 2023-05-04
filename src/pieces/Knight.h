#ifndef RAY_CHESS_KNIGHT_H
#define RAY_CHESS_KNIGHT_H

#include "Piece.h"
#include "../Board.h"

class Knight : public Piece {
public:
    Knight(Position position, TYPE type, const Texture& texture): Piece(position, type, texture) {}

    void Move(const Position& move) override;
    std::vector<Position> GetPossibleMoves(const Board& board) override;
};

#endif //RAY_CHESS_KNIGHT_H
