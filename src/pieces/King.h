#ifndef RAY_CHESS_KING_H
#define RAY_CHESS_KING_H

#include "Piece.h"
#include "../Board.h"

class King : public Piece {
public:
    King(Position position, COLOR color): Piece(position, color, Piece::TYPE::KING) {}
    std::vector<Move> GetPossibleMoves(const Board& board) override;

private:
    bool CheckCastling(const Board& board, const Position& position);
};

#endif //RAY_CHESS_KING_H
