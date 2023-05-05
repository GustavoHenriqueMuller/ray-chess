#ifndef RAY_CHESS_GAME_H
#define RAY_CHESS_GAME_H

#include <string>
#include <map>

#include "pieces/Piece.h"
#include "Board.h"
#include "raylib.h"
#include "Move.h"

class Game {
public:
    const static int WINDOW_WIDTH = 640;
    const static int WINDOW_HEIGHT = 640;
    const static int CELL_SIZE = 640 / 8;
    const static std::string ASSETS_PATH;

    const static Color LIGHT_SHADE;
    const static Color DARK_SHADE;

    Game();
    ~Game();

    void Run();
    void SwapTurns();

private:
    void LoadTextures();
    void HandleInput();
    void HandlePromotionInput();
    Move* GetMoveAtPosition(const Position& position);

    void DoMove(const Move& move);
    void DoShortCastling(const Move& move);
    void DoLongCastling(const Move& move);
    void MovePiece(Piece* piece, const Move& move);

    Board board;
    std::map<std::string, Texture> textures;

    Piece::COLOR turn = Piece::COLOR::C_WHITE;
    Piece* selectedPiece = nullptr;
    std::vector<Move> possibleMoves;

    bool inPromotion = false;
};

#endif //RAY_CHESS_GAME_H
