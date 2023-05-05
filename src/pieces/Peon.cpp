#include "Peon.h"

// TODO: PROMOTION

void Peon::DoMove(const Move &move) {
    if (move.type == Move::TYPE::DOUBLE_WALK) {
        hasOnlyMadeDoubleWalk = true;
    } else {
        hasOnlyMadeDoubleWalk = false;
    };

    Piece::DoMove(move);
}

std::vector<Move> Peon::GetPossibleMoves(const Board& board) {
    std::vector<Move> moves;

    // If black, can only move down. Else, can only move up.
    Position walk = {position.i + (color == COLOR::C_BLACK ? +1 : -1), position.j};

    if (!board.At(walk)) {
        moves.push_back({Move::TYPE::WALK, walk});
    }

    // Check for moving two cells, if the peon has not been moved.
    Position twoCellWalk = {position.i + (color == COLOR::C_BLACK ? +2 : -2), position.j};

    if (!board.At(twoCellWalk) && !board.At(twoCellWalk) && !this->hasMoved) {
        moves.push_back({Move::TYPE::DOUBLE_WALK, twoCellWalk});
    }

    // Check for attacks (diagonals).
    int attackRow = position.i + (color == COLOR::C_WHITE ? -1 : 1);

    Position attackLeft = {attackRow, position.j - 1};
    Position attackRight = {attackRow, position.j + 1};

    if (board.At(attackLeft) && board.At(attackLeft)->color != color) {
        moves.push_back({Move::TYPE::ATTACK, attackLeft});
    }

    if (board.At(attackRight) && board.At(attackRight)->color != color) {
        moves.push_back({Move::TYPE::ATTACK, attackRight});
    }

    // Check for en passant (left).
    Position enPassantAttackLeft = {attackRow, position.j - 1};

    if (CheckEnPassant(board, {position.i, position.j - 1}, enPassantAttackLeft)) {
        moves.push_back({Move::TYPE::EN_PASSANT, enPassantAttackLeft});
    }

    // Check for en passant (right).
    Position enPassantAttackRight = {attackRow, position.j + 1};

    if (CheckEnPassant(board, {position.i, position.j + 1}, enPassantAttackRight)) {
        moves.push_back({Move::TYPE::EN_PASSANT, enPassantAttackRight});
    }

    return moves;
}

bool Peon::CheckEnPassant(const Board& board, const Position& piecePosition, const Position& attackPosition) {
    Piece* piece = board.At(piecePosition);

    if (!board.At(attackPosition) &&
        piece &&
        piece->color != color &&
        piece->type == Piece::TYPE::PEON
        ) {
        Peon* peon = (Peon*) piece;

        if (peon->hasOnlyMadeDoubleWalk) {
            return true;
        }
    }

    return false;
}
