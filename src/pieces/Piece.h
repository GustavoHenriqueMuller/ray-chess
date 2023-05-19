#ifndef RAY_CHESS_PIECE_H
#define RAY_CHESS_PIECE_H

class Piece; // Forward declaration (circular dependency).

#include "raylib.h"
#include "../Position.h"
#include "../Board.h"
#include "../Move.h"
#include "PieceEnums.h"

#include <string>
#include <vector>

class Piece {
public:
    Piece(Position position, PIECE_COLOR color, PIECE_TYPE type);
    virtual ~Piece() = default;

    static Piece* CreatePieceByType(PIECE_TYPE type, const Position& position, PIECE_COLOR color);
    static PIECE_COLOR GetInverseColor(PIECE_COLOR color);
    static std::string GetPieceCharacterByType(PIECE_TYPE type);

    virtual void DoMove(const Move& move);
    virtual std::vector<Move> GetPossibleMoves(const Board& board) = 0;

    Position GetPosition();
    std::string GetName();
    bool HasMoved();

    const PIECE_COLOR color;
    const PIECE_TYPE type;

protected:
    Position position;
    bool hasMoved = false;

private:
    std::string name;
};

#endif //RAY_CHESS_PIECE_H
