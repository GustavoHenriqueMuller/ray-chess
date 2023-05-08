#ifndef RAY_CHESS_PIECE_H
#define RAY_CHESS_PIECE_H

class Piece; // Forward declaration (circular dependency).

#include "raylib.h"
#include "../Position.h"
#include "../Board.h"
#include "../Move.h"
#include "PieceEnums.h"

#include <vector>

class Piece {
public:
    Piece(Position position, PIECE_COLOR color, PIECE_TYPE type);

    static PIECE_COLOR GetInverseColor(PIECE_COLOR color);
    static std::string GetPieceCharacterByType(PIECE_TYPE type);

    virtual void DoMove(const Move& move);
    virtual std::vector<Move> GetPossibleMoves(const Board& board) = 0;

    Position GetPosition();
    std::string GetTextureName();
    bool HasMoved();

    const PIECE_COLOR color;
    const PIECE_TYPE type;

protected:
    Position position;
    bool hasMoved = false;

private:
    std::string textureName;
};

#endif //RAY_CHESS_PIECE_H
