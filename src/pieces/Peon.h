#ifndef RAY_CHESS_PEON_H
#define RAY_CHESS_PEON_H

#include "Piece.h"
#include "../Board.h"

class Peon : public Piece {
public:
    Peon(Position position, PIECE_COLOR color): Piece(position, color, PIECE_TYPE::PEON) {}

    void DoMove(const Move& move) override;
    std::vector<Move> GetPossibleMoves(const Board& board) override;

    bool hasOnlyMadeDoubleWalk = false;

private:
    bool IsPromotionPosition(const Position& position);
    bool CheckEnPassant(const Board& board, const Position& piecePosition, const Position& attackPosition);
};

#endif //RAY_CHESS_PEON_H
