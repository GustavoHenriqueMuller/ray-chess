#include "Board.h"
#include "pieces/Peon.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"

#include <string>
#include <map>

Board::Board(const Board& other) {
    Clear();

    for (Piece* whitePiece : other.whitePieces) {
        Piece* newPiece = Piece::CreatePieceByType(whitePiece->type, whitePiece->GetPosition(), whitePiece->color);
        Add(newPiece);
    }

    for (Piece* blackPiece : other.blackPieces) {
        Piece* newPiece = Piece::CreatePieceByType(blackPiece->type, blackPiece->GetPosition(), blackPiece->color);
        Add(newPiece);
    }
}

Board::~Board() {
    Clear();
}

void Board::Init() {
    // Init black pieces (computer).
    for (int j = 0; j < 8; j++) {
        Add(new Peon({1, j}, PIECE_COLOR::C_BLACK));
    }

    Add(new Rook({0, 0}, PIECE_COLOR::C_BLACK));
    Add(new Rook({0, 7}, PIECE_COLOR::C_BLACK));

    Add(new Knight({0, 1}, PIECE_COLOR::C_BLACK));
    Add(new Knight({0, 6}, PIECE_COLOR::C_BLACK));

    Add(new Bishop({0, 2}, PIECE_COLOR::C_BLACK));
    Add(new Bishop({0, 5}, PIECE_COLOR::C_BLACK));

    Add(new Queen({0, 3}, PIECE_COLOR::C_BLACK));
    Add(new King({0, 4}, PIECE_COLOR::C_BLACK));

    // Init white pieces (player).
    for (int j = 0; j < 8; j++) {
        Add(new Peon({6, j}, PIECE_COLOR::C_WHITE));
    }

    Add(new Rook({7, 0}, PIECE_COLOR::C_WHITE));
    Add(new Rook({7, 7}, PIECE_COLOR::C_WHITE));

    Add(new Knight({7, 1}, PIECE_COLOR::C_WHITE));
    Add(new Knight({7, 6}, PIECE_COLOR::C_WHITE));

    Add(new Bishop({7, 2}, PIECE_COLOR::C_WHITE));
    Add(new Bishop({7, 5}, PIECE_COLOR::C_WHITE));

    Add(new Queen({7, 3}, PIECE_COLOR::C_WHITE));
    Add(new King({7, 4}, PIECE_COLOR::C_WHITE));
}

Piece* Board::At(const Position& position) const {
    if (!IsPositionWithinBoundaries(position)) return nullptr;

    for (Piece* whitePiece : whitePieces) {
        if (whitePiece->GetPosition().i == position.i && whitePiece->GetPosition().j == position.j) {
            return whitePiece;
        }
    }

    for (Piece* blackPiece : blackPieces) {
        if (blackPiece->GetPosition().i == position.i && blackPiece->GetPosition().j == position.j) {
            return blackPiece;
        }
    }

    return nullptr;
}

void Board::Add(Piece* piece) {
    if (piece->color == PIECE_COLOR::C_WHITE) {
        whitePieces.push_back(piece);
    } else {
        blackPieces.push_back(piece);
    }
}

void Board::Destroy(const Position& position) {
    for (unsigned int i = 0; i < whitePieces.size(); i++) {
        if (whitePieces[i]->GetPosition().i == position.i && whitePieces[i]->GetPosition().j == position.j) {
            delete whitePieces[i];
            whitePieces.erase(whitePieces.begin() + i);
            return;
        }
    }

    for (unsigned int i = 0; i < blackPieces.size(); i++) {
        if (blackPieces[i]->GetPosition().i == position.i && blackPieces[i]->GetPosition().j == position.j) {
            delete blackPieces[i];
            blackPieces.erase(blackPieces.begin() + i);
            return;
        }
    }
}

void Board::Clear() {
    for (auto& whitePiece : whitePieces) {
        delete whitePiece;
    }

    for (auto& blackPiece : blackPieces) {
        delete blackPiece;
    }

    whitePieces.clear();
    blackPieces.clear();
}

std::vector<Piece*> Board::GetPiecesByColor(PIECE_COLOR color) const {
    if (color == PIECE_COLOR::C_WHITE) {
        return whitePieces;
    } else {
        return blackPieces;
    }
}

Piece* Board::GetLastMovedPiece() const {
    return At(lastMovedPiecePosition);
}

bool Board::IsPositionWithinBoundaries(const Position &position) const {
    return position.j >= 0 && position.j < 8 && position.i >= 0 && position.i < 8;
}

void Board::DoMove(Piece* piece, const Move& move) {
    // Delete piece, if attack or en passant.
    if (move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION) {
        Destroy(move.position);
    } else if (move.type == MOVE_TYPE::EN_PASSANT) {
        Destroy({piece->GetPosition().i, move.position.j});
    }

    // Move piece. In case of castling, also move rook.
    if (move.type == MOVE_TYPE::SHORT_CASTLING) {
        DoShortCastling(piece, move);
    } else if (move.type == MOVE_TYPE::LONG_CASTLING) {
        DoLongCastling(piece, move);
    } else {
        // Swap positions.
        piece->DoMove(move);
    }

    lastMovedPiecePosition = piece->GetPosition();
}

void Board::DoShortCastling(Piece* selectedPiece, const Move& move) {
    Piece* rook = At({selectedPiece->GetPosition().i, 7});

    selectedPiece->DoMove(move);
    rook->DoMove({MOVE_TYPE::WALK, rook->GetPosition().i, rook->GetPosition().j - 2});
}

void Board::DoLongCastling(Piece* selectedPiece, const Move& move) {
    Piece* rook = At({selectedPiece->GetPosition().i, 0});

    selectedPiece->DoMove(move);
    rook->DoMove({MOVE_TYPE::WALK, rook->GetPosition().i, rook->GetPosition().j + 3});
}

bool Board::MoveLeadsToCheck(Piece* piece, const Move& move) {
    // Copy current board and current selected piece.
    Board boardCopy = *this;
    Piece* pieceInCopiedBoard = boardCopy.At(piece->GetPosition());

    // Perform the move.
    boardCopy.DoMove(pieceInCopiedBoard, move);

    return boardCopy.IsInCheck(piece->color);
}

bool Board::IsInCheck(PIECE_COLOR color) {
    std::vector<Piece*> enemyPieces = GetPiecesByColor(Piece::GetInverseColor(color));

    for (Piece* piece : enemyPieces) {
        for (const Move& move : piece->GetPossibleMoves(*this)) {
            Piece* pieceAtMovePosition = At(move.position);

            bool movePositionContainsMyKing = pieceAtMovePosition &&
                                              pieceAtMovePosition->color == color &&
                                              pieceAtMovePosition->type == PIECE_TYPE::KING;

            bool moveIsAttack = move.type == MOVE_TYPE::ATTACK || move.type == MOVE_TYPE::ATTACK_AND_PROMOTION;

            // If the enemy piece is attacking my king, the king is in check.
            if (movePositionContainsMyKing && moveIsAttack) {
                return true;
            }
        }
    }

    return false;
}
