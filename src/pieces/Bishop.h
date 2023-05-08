#ifndef RAY_CHESS_BISHOP_H
#define RAY_CHESS_BISHOP_H

#include "Piece.h"
#include "../Board.h"

class Bishop : public Piece {
public:
    Bishop(Position position, PIECE_COLOR color): Piece(position, color, PIECE_TYPE::BISHOP) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(const Board& board, std::vector<Move>& moves, Position move, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_BISHOP_H
