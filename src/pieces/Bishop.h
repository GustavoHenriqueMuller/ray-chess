#ifndef RAY_CHESS_BISHOP_H
#define RAY_CHESS_BISHOP_H

#include "Piece.h"
#include "../Board.h"

class Bishop : public Piece {
public:
    Bishop(Position position, TYPE type, const Texture& texture): Piece(position, type, texture) {}

    void Move(const Position& move) override;
    std::vector<Position> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(std::vector<Position>& moves, Position move, const Board& board, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_BISHOP_H
