#include "Board.h"

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
