#ifndef RAY_CHESS_ROOK_H
#define RAY_CHESS_ROOK_H

#include "Piece.h"
#include "../Board.h"

class Rook : public Piece {
public:
    Rook(Position position, COLOR color): Piece(position, color, Piece::TYPE::ROOK) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(std::vector<Move>& moves, Position pos, const Board& board, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_ROOK_H
