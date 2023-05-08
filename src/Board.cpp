#include <string>
#include <map>
#include "Board.h"
#include "pieces/Peon.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"

Board::Board() {
    // Init board.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pieces[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    // Free board.
    Clear();
}

void Board::Init() {
    // Init black pieces (computer).
    for (int j = 0; j < 8; j++) {
        Set({1, j}, new Peon({1, j}, Piece::COLOR::C_BLACK));
    }

    Set({0, 0}, new Rook({0, 0}, Piece::COLOR::C_BLACK));
    Set({0, 7}, new Rook({0, 7}, Piece::COLOR::C_BLACK));

    Set({0, 1}, new Knight({0, 1}, Piece::COLOR::C_BLACK));
    Set({0, 6}, new Knight({0, 6}, Piece::COLOR::C_BLACK));

    Set({0, 2}, new Bishop({0, 2}, Piece::COLOR::C_BLACK));
    Set({0, 5}, new Bishop({0, 5}, Piece::COLOR::C_BLACK));

    Set({0, 3}, new Queen({0, 3}, Piece::COLOR::C_BLACK));
    Set({0, 4}, new King({0, 4}, Piece::COLOR::C_BLACK));

    // Init white pieces (player).
    for (int j = 0; j < 8; j++) {
        Set({6, j}, new Peon({6, j}, Piece::COLOR::C_WHITE));
    }

    Set({7, 0}, new Rook({7, 0}, Piece::COLOR::C_WHITE));
    Set({7, 7}, new Rook({7, 7}, Piece::COLOR::C_WHITE));

    Set({7, 1}, new Knight({7, 1}, Piece::COLOR::C_WHITE));
    Set({7, 6}, new Knight({7, 6}, Piece::COLOR::C_WHITE));

    Set({7, 2}, new Bishop({7, 2}, Piece::COLOR::C_WHITE));
    Set({7, 5}, new Bishop({7, 5}, Piece::COLOR::C_WHITE));

    Set({7, 3}, new Queen({7, 3}, Piece::COLOR::C_WHITE));
    Set({7, 4}, new King({7, 4}, Piece::COLOR::C_WHITE));
}

Piece* Board::At(const Position& position) const {
    if (!IsPositionWithinBoundaries(position)) return nullptr;

    return pieces[position.i][position.j];
}

void Board::Set(const Position& position, Piece *piece) {
    pieces[position.i][position.j] = piece;
}

void Board::Destroy(const Position& position) {
    if (pieces[position.i][position.j]) {
        delete pieces[position.i][position.j]; // @TODO: VER
        pieces[position.i][position.j] = nullptr;
    }
}

void Board::Clear() {
    // Free board.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Destroy({i, j});
        }
    }
}

bool Board::IsPositionWithinBoundaries(const Position &position) const {
    return position.j >= 0 && position.j < 8 && position.i >= 0 && position.i < 8;
}
