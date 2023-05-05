#ifndef RAY_CHESS_BISHOP_H
#define RAY_CHESS_BISHOP_H

#include "Piece.h"
#include "../Board.h"

class Bishop : public Piece {
public:
    Bishop(Position position, COLOR color, const Texture& texture): Piece(position, color, Piece::TYPE::BISHOP, texture) {}
    std::vector<Position> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(std::vector<Position>& moves, Position move, const Board& board, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_BISHOP_H
