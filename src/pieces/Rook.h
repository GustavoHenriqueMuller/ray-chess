#ifndef RAY_CHESS_ROOK_H
#define RAY_CHESS_ROOK_H

#include "Piece.h"
#include "../Board.h"

class Rook : public Piece {
public:
    Rook(Position position, COLOR color, const Texture& texture): Piece(position, color, Piece::TYPE::ROOK, texture) {}
    std::vector<Position> GetPossibleMoves(const Board& board) override;

private:
    void AddValidMoves(std::vector<Position>& moves, Position move, const Board& board, int iIncrement, int jIncrement);
};

#endif //RAY_CHESS_ROOK_H
