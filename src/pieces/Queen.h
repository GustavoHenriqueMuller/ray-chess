#ifndef RAY_CHESS_QUEEN_H
#define RAY_CHESS_QUEEN_H

#include "Piece.h"
#include "../Board.h"

class Queen : public Piece {
public:
    Queen(Position position, PIECE_COLOR color): Piece(position, color, PIECE_TYPE::QUEEN) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(const Board& board, std::vector<Move>& moves, Position pos, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_QUEEN_H
