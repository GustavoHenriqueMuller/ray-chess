#ifndef RAY_CHESS_QUEEN_H
#define RAY_CHESS_QUEEN_H

#include "Piece.h"
#include "../Board.h"

class Queen : public Piece {
public:
    Queen(Position position, TYPE type, const Texture& texture): Piece(position, type, texture) {}
    std::vector<Position> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(std::vector<Position>& moves, Position move, const Board& board, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_QUEEN_H
