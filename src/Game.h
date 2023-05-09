#ifndef RAY_CHESS_GAME_H
#define RAY_CHESS_GAME_H

#include <string>
#include <map>

#include "pieces/Piece.h"
#include "Board.h"
#include "raylib.h"
#include "Move.h"

enum GAME_STATE {
    RUNNING,
    WHITE_WINS,
    BLACK_WINS,
    STALEMATE
};

class Game {
public:
    const static int INFO_BAR_HEIGHT = 32;
    const static int WINDOW_WIDTH = 640;
    const static int WINDOW_HEIGHT = WINDOW_WIDTH + INFO_BAR_HEIGHT;
    const static int CELL_SIZE = WINDOW_WIDTH / 8;

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

    void DoMove(Board& targetBoard, const Move& move, bool doPromotion = true, bool swapTurns = true);
    void DoShortCastling(const Board& targetBoard, const Move& move);
    void DoLongCastling(const Board& targetBoard, const Move& move);

    void CalculateAllPossibleMovements();
    void CheckForEndOfGame();
    bool CheckForCheck(const Board& targetBoard);
    void FilterMovesThatAttackOppositeKing();
    void FilterMovesThatLeadToCheck();
    bool IsAnyMovePossible();

    // Game state.
    Board board;
    std::map<std::string, Texture> textures;

    PIECE_COLOR turn = PIECE_COLOR::C_WHITE;
    Piece* selectedPiece = nullptr;
    std::map<Piece*, std::vector<Move>> possibleMovesPerPiece;

    bool inPromotion = false;
    GAME_STATE state = GAME_STATE::RUNNING;

    // Game information (current round and time).
    int round = 1;
    double time = 0;
};

#endif //RAY_CHESS_GAME_H
