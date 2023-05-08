#include <string>
#include <map>
#include "Board.h"
#include "pieces/Peon.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"

void Board::Init() {
    // Init black pieces (computer).
    for (int j = 0; j < 8; j++) {
        Add(new Peon({1, j}, Piece::COLOR::C_BLACK));
    }

    Add(new Rook({0, 0}, Piece::COLOR::C_BLACK));
    Add(new Rook({0, 7}, Piece::COLOR::C_BLACK));

    Add(new Knight({0, 1}, Piece::COLOR::C_BLACK));
    Add(new Knight({0, 6}, Piece::COLOR::C_BLACK));

    Add(new Bishop({0, 2}, Piece::COLOR::C_BLACK));
    Add(new Bishop({0, 5}, Piece::COLOR::C_BLACK));

    Add(new Queen({0, 3}, Piece::COLOR::C_BLACK));
    Add(new King({0, 4}, Piece::COLOR::C_BLACK));

    // Init white pieces (player).
    for (int j = 0; j < 8; j++) {
        Add(new Peon({6, j}, Piece::COLOR::C_WHITE));
    }

    Add(new Rook({7, 0}, Piece::COLOR::C_WHITE));
    Add(new Rook({7, 7}, Piece::COLOR::C_WHITE));

    Add(new Knight({7, 1}, Piece::COLOR::C_WHITE));
    Add(new Knight({7, 6}, Piece::COLOR::C_WHITE));

    Add(new Bishop({7, 2}, Piece::COLOR::C_WHITE));
    Add(new Bishop({7, 5}, Piece::COLOR::C_WHITE));

    Add(new Queen({7, 3}, Piece::COLOR::C_WHITE));
    Add(new King({7, 4}, Piece::COLOR::C_WHITE));
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
    if (piece->color == Piece::COLOR::C_WHITE) {
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
    for (unsigned int i = 0; i < whitePieces.size(); i++) {
        delete whitePieces[i];
        whitePieces.erase(whitePieces.begin() + i);
    }

    for (unsigned int i = 0; i < blackPieces.size(); i++) {
        delete blackPieces[i];
        blackPieces.erase(blackPieces.begin() + i);
    }
}

std::vector<Piece*> Board::GetPiecesByColor(Piece::COLOR color) {
    if (color == Piece::COLOR::C_WHITE) {
        return whitePieces;
    } else {
        return blackPieces;
    }
}

bool Board::IsPositionWithinBoundaries(const Position &position) const {
    return position.j >= 0 && position.j < 8 && position.i >= 0 && position.i < 8;
}
