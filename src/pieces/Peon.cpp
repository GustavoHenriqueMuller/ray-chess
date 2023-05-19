#include "Peon.h"

void Peon::DoMove(const Move &move) {
    if (move.type == MOVE_TYPE::DOUBLE_WALK) {
        hasOnlyMadeDoubleWalk = true;
    } else {
        hasOnlyMadeDoubleWalk = false;
    };

    Piece::DoMove(move);
}

std::vector<Move> Peon::GetPossibleMoves(const Board& board) {
    std::vector<Move> moves;

    // If black, can only move down. Else, can only move up.
    Position walk = {position.i + (color == PIECE_COLOR::C_BLACK ? +1 : -1), position.j};

    if (!board.At(walk)) {
        moves.push_back({MOVE_TYPE::WALK, walk});
    }

    // Check for moving two cells, if the peon has not been moved.
    Position twoCellWalk = {position.i + (color == PIECE_COLOR::C_BLACK ? +2 : -2), position.j};

    if (!board.At(twoCellWalk) && !this->hasMoved) {
        moves.push_back({MOVE_TYPE::DOUBLE_WALK, twoCellWalk});
    }

    // Check for attacks (diagonals).
    int attackRow = position.i + (color == PIECE_COLOR::C_WHITE ? -1 : 1);

    Position attackLeft = {attackRow, position.j - 1};
    Position attackRight = {attackRow, position.j + 1};

    if (board.At(attackLeft) && board.At(attackLeft)->color != color) {
        moves.push_back({MOVE_TYPE::ATTACK, attackLeft});
    }

    if (board.At(attackRight) && board.At(attackRight)->color != color) {
        moves.push_back({MOVE_TYPE::ATTACK, attackRight});
    }

    // Check for en passant (left).
    Position enPassantAttackLeft = {attackRow, position.j - 1};

    if (CheckEnPassant(board, {position.i, position.j - 1}, enPassantAttackLeft)) {
        moves.push_back({MOVE_TYPE::EN_PASSANT, enPassantAttackLeft});
    }

    // Check for en passant (right).
    Position enPassantAttackRight = {attackRow, position.j + 1};

    if (CheckEnPassant(board, {position.i, position.j + 1}, enPassantAttackRight)) {
        moves.push_back({MOVE_TYPE::EN_PASSANT, enPassantAttackRight});
    }

    // Check for promotion.
    for (Move& move : moves) {
        // Check for promotion if on first row and white, or last row and black.
        if (IsPromotionPosition(move.position)) {
            if (move.type == MOVE_TYPE::ATTACK) {
                move.type = MOVE_TYPE::ATTACK_AND_PROMOTION;
            } else if (move.type == MOVE_TYPE::WALK) {
                move.type = MOVE_TYPE::PROMOTION;
            }
        }
    }

    return moves;
}

bool Peon::IsPromotionPosition(const Position& position) {
    return (position.i == 0 && color == PIECE_COLOR::C_WHITE) ||
           (position.i == 7 && color == PIECE_COLOR::C_BLACK);
}

bool Peon::CheckEnPassant(const Board& board, const Position& piecePosition, const Position& attackPosition) {
    Piece* piece = board.At(piecePosition);

    if (!board.At(attackPosition) &&
        piece &&
        piece->color != color &&
        piece->type == PIECE_TYPE::PEON
        ) {
        Peon* peon = (Peon*) piece;

        if (peon->hasOnlyMadeDoubleWalk && peon == board.GetLastMovedPiece()) {
            return true;
        }
    }

    return false;
}
