#ifndef RAY_CHESS_ROOK_H
#define RAY_CHESS_ROOK_H

#include "Piece.h"
#include "../Board.h"

class Rook : public Piece {
public:
    Rook(Position position, PIECE_COLOR color): Piece(position, color, PIECE_TYPE::ROOK) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(const Board& board, std::vector<Move>& moves, Position pos, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_ROOK_H
